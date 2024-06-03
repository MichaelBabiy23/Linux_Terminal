# Shell Command Executor

# Author
Michael Babiy
323073734

# Description
The Shell Command Executor is a simple shell program that allows users to enter commands, including aliases, and execute them in a child process.
The shell supports basic command execution, alias management, and script execution.

# Features:
- Command Execution: Executes shell commands in a child process.
- Alias Management: Allows creating, listing, and removing command aliases.
- Script Execution: Supports executing commands from a script file.
- Input Parsing: Handles input parsing with special handling for quoted arguments.

# Program Files
- 'ex1.c' (Main program): Contains the main function and the loop for reading user inputs.
- 'command_executor.c' (Command Executor): Contains functions for executing commands and scripts.
- 'alias_manager.c' (Alias Manager): Contains functions for managing command aliases.
- 'string_utils.c' (String Utilities): Contains utility functions for string manipulation.
- 'constants.h' (Constants): Contains definitions for constants used throughout the program.
- 'command_executor.h' (Command Executor Header): Header file for 'command_executor.c'.
- 'alias_manager.h' (Alias Manager Header): Header file for 'alias_manager.c'.
- 'string_utils.h' (String Utilities Header): Header file for 'string_utils.c'.

# How to Compile and Run

# Compilation
To compile the program, run the following command:
gcc -Wall ex1.c alias_manager.c string_utils.c command_executor.c -o ex1

# Running
To run the program, use the following command:
./ex1

# Input
- Interactive Shell: The program reads commands from the user input in an interactive shell.

# Output
- Command Execution: Outputs the result of executing commands.
- Alias Management: Outputs alias information.
- Error Messages: Outputs error messages when an error occurs.