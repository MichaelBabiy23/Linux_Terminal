#!/bin/bash
# 323073734

# Compile the C code
gcc -Wall alias_manager.c string_utils.c command_executor.c process_handler.c process_manager.c ex2.c -o ex2

# Run the compiled program
./ex2
