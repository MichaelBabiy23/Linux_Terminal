// 323073734
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "Headers/alias_manager.h"
#include "Headers/string_utils.h"
#include "Headers/command_executor.h"
#include "Headers/constants.h"

extern int success_commands;
extern int total_script_lines;

/**
 * Executes the given command by parsing and forking a new process.
 * @param input.
 */
void execute_command(char *input) {
    remove_multiple_spaces(input);

    if(has_balanced_quotes(input) == 0)
    {
        // printf("Invalid quotes number.\n");
        printf("ERR\n");
        return;
    }
    // Check for alias print
    if (strcmp(input, "alias") == 0 || strcmp(input, "alias ") == 0) {
        print_aliases();
        success_commands++;
        return;
    }

    if (strncmp(input, "alias ", 6) == 0) {
        // remove_spaces_around_equals(input);

        char *equal_sign = strchr(input, '=');
        if (equal_sign != NULL && has_space_around_equal(input) == 0) {
            *equal_sign = '\0';
            char *name = input + 6;
            char *command = equal_sign + 1;

            // Remove surrounding quotes if they exist
            if (command[0] == '\'' || command[0] == '"') {
                total_apostrophes++;
                command++;
                command[strlen(command) - 1] = '\0';
            }

            add_alias(name, command);
            success_commands++;
        } else {
            printf("ERR\n");
            // printf("Invalid alias format. Use alias name='command'.\n");
        }
        return;
    }

    if (strcmp(input, "unalias") == 0) {
        printf("ERR\n");
        // printf("Usage: unalias name\n");
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
    if (num_args > MAX_ARGS + 1) // Not include the command itself
    {
        printf("ERR\n");
        // printf("Error: Maximum number of arguments exceeded.\n");
        return;
    }

    // Check if the input matches any existing aliases
    char *alias_command = get_alias_command(args[0]);
    if (alias_command != NULL) {
        args[0] = alias_command;
        execute_command(concatenate_arguments(num_args, args));
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
        for (int i = 1; i < num_args; ++i) {
            remove_quotes(args[i]);
        }
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
        // printf("Im stupid child --------------------\n");
        // Child process
        if (execvp(args[0], args) < 0) {
            perror("exec");
            _exit(1);
            // _exit instead of simple exit to avoid looping
        }
    } else {
        // Parent process
        wait(&status);
        // If child process succeed
        if(WEXITSTATUS(status) == 0) {
            success_commands++;
            total_apostrophes += has_quotes(args);
        }
    }
}

/**
 * Executes the script file.
 * @param script_file The path to the script file.
 */
void execute_script(char *script_file) {
    FILE *fp;
    char line[MAX_LINE];

    // Open the script file
    fp = fopen(script_file, "r");
    if (fp == NULL) {
        perror("source");
        return;
    }
    int len = strlen(script_file);

    // Check if the last three characters are ".sh"
    if (len < 3 || !(script_file[len - 3] == '.' && script_file[len - 2] == 's' && script_file[len - 1] == 'h')) {
        // printf("Not .sh file\n");
        printf("ERR\n");
        return;
    }


    success_commands++;

    if (fgets(line, sizeof(line), fp) != NULL)
    {
        // Remove newline character
        line[strcspn(line, "\n")] = 0;

        // printf("%s", line);
        // Bash header
        if (strcmp(line, "#!/bin/bash") != 0)
        {
            printf("ERR\n");
            // printf("No bash header\n");
            return;
        }
    }

    // Read and execute each line in the script file
    while (fgets(line, sizeof(line), fp) != NULL) {

        // Remove newline character
        line[strcspn(line, "\n")] = 0;

        // Increment script line counter
        total_script_lines++;

        // Empty line
        if (strlen(line) == 0)
            continue;

        // Execute the command
        execute_command(line);


    }

    // Close the script file
    fclose(fp);
}

/**
 * Exits the shell.
 */
void shell_exit() {
    // printf("Exiting shell...\n");
    printf("%i\n", total_apostrophes);
    free_all_aliases();
    exit(0);
}