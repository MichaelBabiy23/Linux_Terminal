// 323073734
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Headers/constants.h"
#include "Headers/string_utils.h"

/**
 * Removes the first and last characters from a given string.
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
 * @param input The input string.
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
    *num_args = 0;
    char *token = strtok(input, " ");

    while (token != NULL && *num_args < MAX_ARGS + 2) { // MAX_ARGS + command + spare one to not show an error
        // Check if the token starts with a quote
        if (token[0] == '\"' || token[0] == '\'') {
            char quote_type = token[0]; // Store the quote type (single or double)
            // If the token ends with the same quote
            if (token[strlen(token) - 1] == quote_type) {
                args[(*num_args)++] = token; // Add the token as an argument
            } else {
                // If the quote is not closed, concatenate the token with the next one
                char *quote = strdup(token);
                while ((token = strtok(NULL, " ")) != NULL) {
                    strcat(quote, " ");
                    strcat(quote, token);
                    // If the concatenated string ends with the same quote, break the loop
                    if (token[strlen(token) - 1] == quote_type) {
                        args[(*num_args)++] = quote;
                        break;
                    }
                }
                if (token == NULL) {
                    args[(*num_args)++] = quote; // Add the incomplete quoted string as an argument
                }
            }
        } else {
            args[(*num_args)++] = token;
        }
        // printf("ARGS: %s\n", args[(*num_args) - 1]);
        token = strtok(NULL, " "); // Move to the next token
    }
    args[*num_args] = NULL;
}

/**
 * Checks if the input has balanced quotes (single or double)
 * @param input
 * @return 0/1
 */
int has_balanced_quotes(const char *input) {
    int single_quote_count = 0;
    int double_quote_count = 0;

    while (*input) {
        if (*input == '\'') {
            single_quote_count++;
        } else if (*input == '"') {
            double_quote_count++;
        }
        input++;
    }

    // Check if both single and double quotes are balanced
    return (single_quote_count % 2 == 0) && (double_quote_count % 2 == 0);
}

/**
 * Concatenate all the arguments to one string.
 * @param num_args
 * @param args
 * @return
 */
char* concatenate_arguments(int num_args, char *args[]) {
    // Calculate the total length needed
    int total_length = 0;
    for (int i = 0; i < num_args; i++) {
        total_length += strlen(args[i]);
        if (i < num_args - 1) {
            total_length++;  // for the space between arguments
        }
    }

    // Allocate memory for the concatenated string
    char *result = (char *)malloc((total_length + 1) * sizeof(char));
    if (result == NULL) {
        perror("malloc");
        exit(1);
    }

    // Initialize the result string
    result[0] = '\0';

    // Concatenate all arguments into the result string
    for (int i = 0; i < num_args; i++) {
        strcat(result, args[i]);
        if (i < num_args - 1) {
            strcat(result, " ");
        }
    }

    return result;
}

/**
 * Split the input to name and command - Alias format.
 * @param input
 * @param command
 * @param name
 */
int split_alias(const char *input, char *command, char *name) {
    const char *delimiters = " ='\"";
    char *token;

    // Find the first delimiter
    token = strtok((char *)input, delimiters);
    if (token == NULL) {
        // printf("Error: Invalid alias format\n");
        fprintf(stderr, "ERR\n");
        return 1;
    }

    // Copy the name
    strcpy(name, token);

    // Find the next non-empty token
    while ((token = strtok(NULL, delimiters)) != NULL) {
        if (strcmp(token, "=") == 0 || strcmp(token, " ") == 0) {
            continue; // Skip equal signs and spaces
        } else {
            strcpy(command, token);
            return 0;
        }
    }

    return 1;
}

/**
 * Getting a string and check if it has a space before or after the equal sign.
 * @param str
 * @return 1 if exists
 */
int has_space_around_equal(const char *str) {
    int len = strlen(str);
    int i;

    for (i = 0; i < len; i++) {
        if (str[i] == '=') {
            if ((i > 0 && str[i - 1] == ' ') || (i < len - 1 && str[i + 1] == ' '))
                return 1; // If space found before or after '='
        }
    }

    return 0; // No space found before or after '='
}

/**
 * Removes the quotes
 * @param str
 */
int remove_quotes(char *str) {
    int len = strlen(str);
    int removed = 0;
    // Handle single quotes at the start and end
    if (len > 1 && str[0] == '\'' && str[len - 1] == '\'') {
        memmove(str, str + 1, len - 2); // Shift characters to overwrite the start quote
        str[len - 2] = '\0'; // Null terminate the string
        removed = 1;
    }
    else
    {
        len = strlen(str); // Update length in case it was changed
        int i, j = 0;

        // Remove all double quotes
        for (i = 0; i < len; i++) {
            if (str[i] != '\"') {
                str[j++] = str[i];
            }
            else
                removed = 1;
        }

        str[j] = '\0'; // Null terminate the string
    }
    return removed;
}

/**
 * Check if args array has any quote
 * @param args
 * @return true/false
 */
int has_quotes(char *args[]) {
    for (char* arg; *args; args++) {
        arg = *args;
        while (*arg) {
            if (*arg == '"' || *arg == '\'') {
                return 1; // Found a quote, return true
            }
            arg++;
        }
    }
    return 0; // No quotes found in any of the strings
}