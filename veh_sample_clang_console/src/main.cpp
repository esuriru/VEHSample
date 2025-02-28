// For `fopen`
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <dbghelp.h>
#include <stdio.h>

// Declare memory to be used for the stacktrace buffer, so when
// the error is not having enough memory left, we will stil be able to
// print out the stack trace.
namespace
{
    void* stack_trace_buffer[100];
}

__attribute__((noinline)) void stack_overflow() 
{
    stack_overflow();
}

LONG CALLBACK exception_handler(PEXCEPTION_POINTERS exception_info) 
{
    printf("Exception caught: 0x%lX\n", 
        exception_info->ExceptionRecord->ExceptionCode);

#ifdef DEBUG
    if (exception_info->ExceptionRecord->ExceptionCode == 
        EXCEPTION_BREAKPOINT)
    {
        printf("%s", "Breakpoint caught by VEH");

        return EXCEPTION_CONTINUE_EXECUTION;
    }
    else 
    if (exception_info->ExceptionRecord->ExceptionCode == 
        EXCEPTION_ACCESS_VIOLATION) 
#endif
    {
#ifdef DEBUG
        printf("%s", "Access violation caught by VEH\n");
#endif

        // Capture stack trace
        USHORT frames = CaptureStackBackTrace(0, 100, stack_trace_buffer, NULL);

        // Initialize symbol handling
        SymInitialize(GetCurrentProcess(), NULL, TRUE);

        // Print stack trace
        FILE* file = fopen(CRASH_LOG_FILENAME, "w");
        if (!file)
        {
            printf("Could not create %s\n", CRASH_LOG_FILENAME);
        }

        fputs("Stacktrace\n", file);

        for (USHORT i = 0; i < frames; ++i) 
        {
            DWORD64 address = (DWORD64)stack_trace_buffer[i];
            SYMBOL_INFO symbol = { sizeof(SYMBOL_INFO), 0, 0, 0, 0, 0 };

            if (SymFromAddr(GetCurrentProcess(), address, 0, &symbol)) 
            {
                fprintf(file, "%s - 0x%p\n", symbol.Name, (void*)address);

                IMAGEHLP_LINE64 line = { sizeof(IMAGEHLP_LINE64) };
                DWORD displacement;
                if (SymGetLineFromAddr64(
                    GetCurrentProcess(), 
                    address, 
                    &displacement, 
                    &line))
                {
                    fprintf(file, "%s:%lu\n", line.FileName, line.LineNumber);
                }

            }
            else
            {
                fprintf(file, "0x%p (Symbol information not available)\n",
                    (void*)address);
            }
        }

        fclose(file);

        SymCleanup(GetCurrentProcess());

        const char* crash_handler_filename = CRASH_HANDLER_FILENAME;

        system(crash_handler_filename); 
        TerminateProcess(GetCurrentProcess(), 1);
    }

    return EXCEPTION_CONTINUE_SEARCH;
}

int main()
{
    AddVectoredExceptionHandler(1, exception_handler);

    printf("%s", "Causing access violation\n");

    int* ptr = nullptr;
    *ptr = 10;

    printf("%s", "Causing stack overflow\n");

    stack_overflow();

    return 0;
}
