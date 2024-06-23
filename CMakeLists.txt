cmake_minimum_required(VERSION 3.27)
project(OSHomeWork1 C)

set(CMAKE_C_STANDARD 11)

add_executable(OSHomeWork1 ex2.c
        Headers/string_utils.h
        string_utils.c
        Headers/command_executor.h
        command_executor.c
        Headers/alias_manager.h
        alias_manager.c
        Headers/constants.h
        Headers/process_handler.h
        "process_handler.c"
        Headers/process_manager.h
        process_manager.c
)
