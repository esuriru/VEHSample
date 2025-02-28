add_executable(veh_sample_placeholder_crash_handler)

target_compile_options(veh_sample_placeholder_crash_handler PRIVATE 
    "-fno-exceptions")
target_sources(veh_sample_placeholder_crash_handler PRIVATE 
    "src/main.cpp")

target_compile_definitions(veh_sample_placeholder_crash_handler PRIVATE
    -DCRASH_LOG_FILENAME=\"crash_log.txt\"
)

target_compile_definitions(veh_sample_clang_console PRIVATE
    -DCRASH_HANDLER_FILENAME=\"veh_sample_placeholder_crash_handler.exe\"
    -DCRASH_LOG_FILENAME=\"crash_log.txt\"
)
