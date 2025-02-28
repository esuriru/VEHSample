add_executable(veh_sample_clang_console)

target_compile_options(veh_sample_clang_console PRIVATE "-fno-exceptions")
target_link_libraries(veh_sample_clang_console PRIVATE "dbghelp")
target_sources(veh_sample_clang_console PRIVATE "src/main.cpp")

add_custom_command(
    TARGET veh_sample_clang_console
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:veh_sample_placeholder_crash_handler> 
            $<TARGET_FILE_DIR:veh_sample_clang_console>
    COMMAND_EXPAND_LISTS
)
