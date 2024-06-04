Shell Project
Authored by Michael Babiy
323073734

#Description

This project implements a custom shell program that can execute commands, manage aliases, and run script files.
The shell supports various features such as command execution, alias management, script execution, and tracking of command statistics.

# Features

- Command Execution: The shell can execute commands by parsing the input, forking a new process, and executing the command in the child process.
- Alias Management: Users can create, view, and remove aliases for frequently used commands.
- Script Execution: The shell can execute script files (.sh files) by reading and executing each line in the script.
- Statistics Tracking: The shell keeps track of the number of successfully executed commands, active aliases, and lines executed from script files.
- Error Handling: The shell provides error messages for invalid inputs, such as unbalanced quotes, invalid alias formats, and non-script files.

# Program Database

- alias_list: A linked list of Alias structs, representing the list of aliases.
- active_aliases: An integer keeping track of the number of active aliases.

# Program Files

- main.c: Contains the main function and the entry point of the program.
- command_executor.c: Implements the functions for executing commands, scripts, and managing child processes.
- alias_manager.c: Handles the management of aliases, including adding, removing, retrieving aliases, and freeing memory.
- string_utils.c: Contains utility functions for string manipulation, such as removing quotes, spaces, and parsing arguments.
- Headers/: This directory contains header files for the corresponding .c files, defining structs, constants, and function declarations.

# Compilation

To compile the program, use the following command:
gcc -Wall ex1.c alias_manager.c string_utils.c command_executor.c -o ex1

# Execution

Run the compiled program with:
./ex1

# Usage

1. Executing Commands: Enter a command at the prompt, and the shell will execute it. For example: ls -l.
2. Managing Aliases:
   - Create a new alias: alias name='command'
   - View all aliases: alias
   - Remove an alias: unalias name
3. Executing Scripts: Use the source command followed by the script file path to execute a script file. For example: source script.sh.

# Output

The shell prompt displays the following information:

#cmd:X|#alias:Y|#script lines:Z>

- X: The number of successfully executed commands.
- Y: The number of active aliases.
- Z: The number of lines executed from script files.

When exiting the shell, it prints the total number of apostrophes (single or double quotes) encountered during execution.

# Note

- The shell supports a maximum of MAX_ARGS arguments for a command (defined in constants.h).
- The maximum line length for user input or script lines is MAX_LINE (defined in constants.h).

# Function Descriptions

main.c

- update_prompt(): Updates and prints the shell prompt with the current command, alias, and script line counts.
- main(): The entry point of the program. It runs an infinite loop that reads user input, updates the prompt, and executes the command.

command_executor.c

- execute_command(char *input): Executes the given command by parsing and forking a new process. It handles alias management, script execution, and error handling.
- execute_child_process(char *args[]): Executes the given command in a child process using fork and execvp.
- execute_script(char *script_file): Executes the script file by reading and executing each line in the file.
- shell_exit(): Exits the shell by printing the total number of apostrophes, freeing all aliases, and terminating the program.

alias_manager.c

- add_alias(char *name, char *command): Adds a new alias to the alias list. If the alias name already exists, it updates the command associated with that alias.
- remove_alias(char *name): Removes an alias from the alias list by its name.
- get_alias_command(char *name): Returns the command associated with an alias. If the alias is not found, it returns NULL.
- print_aliases(): Prints all aliases in the alias list, displaying their names and associated commands.
- free_all_aliases(): Frees the memory allocated for all aliases in the alias list.

string_utils.c

- remove_first_last_char(char *arg): Removes the first and last characters from a given string if they are enclosing quotes.
- remove_multiple_spaces(char *input): Removes multiple consecutive spaces from the input string, leaving only single spaces between words.
- remove_spaces_around_equals(char *input): Removes spaces before and after the = sign in the input string.
- parse_arguments(char *input, char *args[], int *num_args): Parses the input line into arguments, handling quoted strings correctly.
- has_balanced_quotes(const char *input): Checks if the input string has balanced quotes (single or double).
- concatenate_arguments(int num_args, char *args[]): Concatenates all the arguments into a single string.
- split_alias(const char *input, char *command, char *name): Splits the input into an alias name and command.
- has_space_around_equal(const char *str): Checks if the input string has a space before or after the equal sign.
- remove_quotes(char *str): Removes the quotes from the input string.
- has_quotes(char *args[]): Checks if the args array contains any quoted strings.
