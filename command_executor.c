// 323073734
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Headers/alias_manager.h"
#include "Headers/string_utils.h"
#include "Headers/command_executor.h"
#include "Headers/constants.h"
#include "Headers/process_handler.h"
#include "Headers/process_manager.h"

extern int success_commands;
extern int total_script_lines;

int background_process_flag = 0;

/**
 * Alias handler function to process alias command.
 * @param input The input string containing the alias command
 */
void alias_handler(char* input)
{
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
}

/**
 * Handles commands separated by AND or OR operators.
 * @param input The input string containing the commands.
 * @param and_sign Pointer to the AND sign.
 * @param or_sign Pointer to the OR sign.
 */
void or_and_sign_handler(char* input, char* and_sign, char* or_sign)
{
    char* first_command = input;
    char* second_command = and_sign == NULL ? or_sign + 3 : and_sign + 3;
    int temp_success_commands = success_commands;
    if (and_sign)
    {
        *and_sign = '\0';
        execute_command(first_command);
        if (temp_success_commands + 1 == success_commands)
            execute_command(second_command);
    }
    else
    {
        *or_sign = '\0';
        execute_command(first_command);
        if (temp_success_commands + 1 != success_commands)
            execute_command(second_command);
    }
}

/**
 * Executes the given command by parsing and forking a new process.
 * @param input.
 */
void execute_command(char *input) {
    if (input == NULL || strlen(input) == 0) { // Check for NULL input or empty string
        printf("ERR\n");
        return;
    }


    remove_multiple_spaces(input);

    if(has_balanced_quotes(input) == 0)
    {
        // printf("Invalid quotes number.\n");
        printf("ERR\n");
        return;
    }

    // Check for & sign
    background_process_flag = 0;
    input[strlen(input) - 1] = input[strlen(input)- 1]  == ' ' ? '\0' : input[strlen(input)- 1];
    if (input[strlen(input)- 1]  == '&')
    {
        input[strlen(input)- 1] = '\0';
        background_process_flag = 1;
    }

    // Check for alias print
    if (strcmp(input, "jobs") == 0 || strcmp(input, "jobs ") == 0) {
        print_process();
        success_commands++;
        return;
    }

    // Check for alias print
    if (strcmp(input, "alias") == 0 || strcmp(input, "alias ") == 0) {
        print_aliases();
        success_commands++;
        return;
    }

    // Adding new alias
    if (strncmp(input, "alias ", 6) == 0) {
        alias_handler(input);
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


    char* and_sign = strstr(input, "&&");
    char* or_sign = strstr(input, "||");
    if (and_sign != NULL || or_sign != NULL)
    {
        or_and_sign_handler(input, and_sign, or_sign);
        return;
    }

    // If not an alias, execute the command
    char *args[MAX_ARGS] = {0};
    int num_args = 0;
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
        int checker = 0;
        for (int i = 1; i < num_args; ++i) {
            if (remove_quotes(args[i]) == 1)
                checker = 1;
        }
        total_apostrophes += checker;
    }
    execute_child_process(args, num_args);
}


/**
 * Executes the script file.
 * @param script_file The path to the script file.
 */
void execute_script(char *script_file) {
    FILE *fp;
    char line[MAX_LINE] = {0};

    // Open the script file
    fp = fopen(script_file, "r");
    if (fp == NULL) {
        perror("source");
        return;
    }
    int len = strlen(script_file);

    // Check if the last three characters are ".sh"
    if (len < 3 || strcmp(script_file + len - 3, ".sh") != 0) {
        printf("ERR\n");
        fclose(fp);
        return;
    }


    success_commands++;

    // Bash header check
    if (fgets(line, sizeof(line), fp) != NULL && strcmp(line, "#!/bin/bash\n") != 0) {
        printf("ERR\n");
        fclose(fp);
        return;
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
    free_all_process();
    exit(0);
}