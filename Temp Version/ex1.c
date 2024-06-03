// 323073734
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "alias.h"

// Defines
#define MAX_LINE 1024
#define MAX_ARGS 4

// Function Declarations
void remove_first_last_char(char *arg);
void remove_multiple_spaces(char *input);
void remove_spaces_around_equals(char *input);
void parse_arguments(char *input, char *args[], int *num_args);
void execute_command(char *input);
void execute_child_process(char *args[]);
void shell_exit();
void execute_script(char *script_file);
void update_prompt();

// Global Variables
int success_commands = 0;
int total_script_lines = 0;
int total_apostrophes = 0;

/**
 * Removes the first and last characters from a given string
 * @param arg
 */
void remove_first_last_char(char *arg) {
    int len = strlen(arg);

    if (len <= 2) {
        // String is too short to remove first and last characters
        return;
    }

    if ((arg[0] == arg[len - 1]) &&  (arg[len - 1] == 34 || arg[len - 1] == 39)) {
        // Shift characters to the left to overwrite the first character
        for (int i = 0; i < len - 1; i++) {
            arg[i] = arg[i + 1];
        }

        // Replace the last character with null terminator
        arg[len - 2] = '\0';
        total_apostrophes++;
    }

}

/**
 * Removes multiple consecutive spaces from the input string,
 * leaving only single spaces between words.
 * @param str The input string.
 */
void remove_multiple_spaces(char *input) {
    int i = 0, x = 0;

    while (input[i]) {
        // Remove multiple spaces
        if (input[i] == ' ' && (i > 0 && input[i-1] == ' ')) {
            i++;
            continue;
        }

        input[x++] = input[i++];
    }
    input[x] = '\0';

    /*
    // Remove trailing space if present
    if (x > 0 && input[x-1] == ' ') {
        input[x-1] = '\0';
    }
    */
}

/**
 * Removes spaces before and after the '=' sign in the input string.
 * @param str The input string.
 */
void remove_spaces_around_equals(char *input) {
    int i = 0, x = 0;
    int len = strlen(input);

    while (input[i]) {
        // Check for spaces before '='
        if (i+1 < len && input[i] == ' ' && input[i+1] == '=') {
            // Skip space before '='
            i++;
        }

        // Copy '=' sign
        if (input[i] == '=') {
            input[x++] = input[i++];

            // Check for spaces after '='
            if (input[i] == ' ') {
                // Skip space after '='
                i++;
            }

            continue;
        }

        input[x++] = input[i++];
    }
    input[x] = '\0';
}

/**
 * Parses the input line into arguments.
 * @param input.
 * @param args Array of the arguments.
 * @param num_args Pointer to store the number of arguments.
 */
void parse_arguments(char *input, char *args[], int *num_args) {
    *num_args = 0; // Initialize the number of arguments to 0
    char *token = strtok(input, " "); // Tokenize the input string

    while (token != NULL && *num_args < MAX_ARGS + 1) {
        // Check if the token starts with a quote
        if (token[0] == '"' || token[0] == '\'')
        {
            // If the token ends with the same quote
            if (token[strlen(token) - 1] == '"' || token[strlen(token) - 1] == '\'')
            {
                args[(*num_args)++] = token; // Add the token as an argument
            }
            else
            {
                // If the quote is not closed, concatenate the token with the next one
                char *quote = strdup(token); // Create a copy of the token
                while ((token = strtok(NULL, " ")) != NULL) {
                    strcat(quote, " "); // Add space before appending the next token
                    strcat(quote, token); // Concatenate the token
                    // If the concatenated string ends with a quote, break the loop
                    if (token[strlen(token) - 1] == '"' || token[strlen(token) - 1] == '\'')
                    {
                        args[(*num_args)++] = quote; // Add the concatenated string as an argument
                        break;
                    }
                }
            }
        }
        else
        {
            args[(*num_args)++] = token; // Add the token as an argument
        }
        token = strtok(NULL, " "); // Move to the next token
    }
    args[*num_args] = NULL; // Add a NULL terminator to mark the end of the argument list
}

/**
 * Executes the given command by parsing and forking a new process.
 * @param input.
 */
void execute_command(char *input) {
    // Check for alias print
    if (strcmp(input, "alias") == 0) {
        print_aliases();
        success_commands++;
        return;
    }

    if (strncmp(input, "alias ", 6) == 0) {
        remove_spaces_around_equals(input);
        char *equal_sign = strchr(input, '=');
        if (equal_sign != NULL) {
            *equal_sign = '\0';
            char *name = input + 6;
            char *command = equal_sign + 1;

            // Remove surrounding quotes if they exist
            if (command[0] == '\'' || command[0] == '"') {
                command++;
                command[strlen(command) - 1] = '\0';
            }

            add_alias(name, command);
            success_commands++;
        } else {
            printf("Invalid alias format. Use alias name='command'.\n");
        }
        return;
    }

    if (strcmp(input, "unalias") == 0) {
        printf("Usage: unalias name\n");
        return;
    }

    if (strncmp(input, "unalias ", 8) == 0) {
        char *name = input + 8;
        remove_alias(name);
        success_commands++;
        return;
    }

    // If not an alias, execute the command
    char *args[MAX_ARGS];
    int num_args;
    parse_arguments(input, args, &num_args);
    if (num_args > MAX_ARGS)
    {
        printf("Error: Maximum number of arguments exceeded.\n");
        return;
    }

    // Check if the input matches any existing aliases
    char *alias_command = get_alias_command(args[0]);
    if (alias_command != NULL) {
        execute_command(alias_command);
        return;
    }

    if (strcmp(args[0], "exit_shell") == 0) {
        shell_exit();
    }

    if (strcmp(args[0], "source") == 0) {
        execute_script(args[1]);
        return;
    }

    if (strcmp(args[0], "echo") == 0) {
        remove_first_last_char(args[1]);
    }
    execute_child_process(args);
}

/**
 * Executes the given command in a child process.
 * @param args The command arguments.
 */
void execute_child_process(char *args[]) {
    int status;
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        // Child process
        if (execvp(args[0], args) < 0) {
            perror("exec");
            exit(1);
        }
    } else {
        // Parent process
        wait(&status);
        // If child process succeed
        if(WEXITSTATUS(status) == 0)
            success_commands++;
    }
}

/**
 * Executes the script file.
 * @param script_file The path to the script file.
 */
void execute_script(char *script_file) {
    FILE *fp;
    char line[MAX_LINE];
    int fist_line = 1;

    // Open the script file
    fp = fopen(script_file, "r");
    if (fp == NULL) {
        perror("source");
        return;
    }

    success_commands++;

    // Read and execute each line in the script file
    while (fgets(line, sizeof(line), fp) != NULL) {
        // printf("%s", line);
        // If first line, skip because of the bash header
        if (fist_line)
        {
            fist_line = 0;
            continue;
        }

        // Remove newline character
        line[strcspn(line, "\n")] = 0;

        // Empty line
        if (strlen(line) == 0)
            continue;

        // Execute the command
        execute_command(line);

        // Increment script line counter
        total_script_lines++;
    }

    // Close the script file
    fclose(fp);
}

/**
 * Exits the shell.
 */
void shell_exit() {
    printf("Exiting shell...\n");
    printf("%i\n", total_apostrophes);
    exit(0);
}

/**
 * Updates the shell prompt with the current information.
 */
void update_prompt() {
    printf("#cmd:%d|#alias:%d|#script lines:%d> ", success_commands, active_aliases, total_script_lines);
}

/**
 * The main function of the shell program.
 * It continuously reads input, parses it, and executes commands.
 */
int main() {
    char input[MAX_LINE];

    while (1) {
        update_prompt();
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        input[strcspn(input, "\n")] = 0; // Removes newline character
        remove_multiple_spaces(input);
        execute_command(input);
        total_script_lines++;
    }
    return 0;
}
