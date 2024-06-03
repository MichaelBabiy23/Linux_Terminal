#!/bin/bash
# 323073734

# Compile the C code
gcc -Wall ex1.c alias_manager.c string_utils.c command_executor.c -o ex1

# Run the compiled program
./ex1
