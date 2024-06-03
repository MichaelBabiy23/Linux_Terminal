# README

## Files
- ex1.c: The main C source file containing the implementation.
- run_me.sh: A shell script to compile and run the C program.

## How to Use
1. Make sure you have GCC installed.
2. Run the shell script:
3. The script will compile the C code and run the resulting executable.

## Error Handling
The program includes error handling for:
- Memory allocation using `malloc`.
- Process creation using `fork`.
- Command execution using `execl`.

Each error prints an error message using `perror` and "ERR", then exits with a failure status.
