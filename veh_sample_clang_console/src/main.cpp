#include <windows.h>
#include <iostream>

LONG CALLBACK exception_handler(PEXCEPTION_POINTERS exception_info) 
{
    if (exception_info->ExceptionRecord->ExceptionCode == 
        EXCEPTION_ACCESS_VIOLATION) 
    {
        std::cout << "Access violation caught by VEH" << std::endl;

        system(CRASH_HANDLER_FILENAME); 
        TerminateProcess(GetCurrentProcess(), 1);
    }

    return EXCEPTION_CONTINUE_SEARCH;
}

int main()
{
    std::cout << "Causing access violation" << std::endl;

    AddVectoredExceptionHandler(1, exception_handler);

    // Intentional access violation
    int* ptr = nullptr;
    *ptr = 10;

    return 0;
}