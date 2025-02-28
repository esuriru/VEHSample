cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(VEHSample LANGUAGES CXX)

if(NOT WIN32)
    message(FATAL_ERROR "Project only works on Windows")
endif()

if(NOT ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    message(FATAL_ERROR "Project needs to be compiled using Clang")
endif()

add_subdirectory(veh_sample_clang_console)
add_subdirectory(veh_sample_placeholder_crash_handler)
