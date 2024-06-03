// 323073734
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Headers/alias_manager.h"

Alias *alias_list = NULL;
int active_aliases = 0;

/**
 * Adds a new alias to the alias list.
 * @param name.
 * @param command.
 */
void add_alias(char *name, char *command) {
    if (strcmp(name, command) == 0)
    {
        printf("Can't add alias with the same command and name.\n");
        return;
    }

    // Updating if alias exists
    Alias *current = alias_list;
    while (current != NULL) {
        // Check if the alias name matches
        if (strcmp(current->name, name) == 0) {
            // Free the old command and update with the new command
            free(current->command);
            current->command = strdup(command);
            return;
        }
        current = current->next;
    }

    // Adding alias
    Alias *new_alias = (Alias *)malloc(sizeof(Alias));
    new_alias->name = strdup(name);
    new_alias->command = strdup(command);
    new_alias->next = alias_list;
    alias_list = new_alias;
    active_aliases++;
}


/**
 * Removes an alias from the alias list.
 * @param name The name of the alias to remove.
 */
void remove_alias(char *name) {
    Alias *current = alias_list;
    Alias *previous = NULL;

    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            if (previous == NULL) {
                alias_list = current->next;
            } else {
                previous->next = current->next;
            }
            free(current->name);
            free(current->command);
            free(current);
            active_aliases--;
            return;
        }
        previous = current;
        current = current->next;
    }
}

/**
 * Retuning the command associated with an alias.
 * @param name.
 * @return The command associated with the alias, or NULL if not found.
 */
char* get_alias_command(char *name) {
    Alias *current = alias_list;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            char *temp = malloc(strlen(current->command) + 1);
            strcpy(temp, current->command);
            return temp;
        }
        current = current->next;
    }
    return NULL;
}

/**
 * Prints all aliases in the alias list.
 */
void print_aliases() {
    Alias *current = alias_list;
    while (current != NULL) {
        printf("%s='%s'\n", current->name, current->command);
        current = current->next;
    }

}

/**
 * Free all the aliases
 */
void free_all_aliases() {
    while (alias_list != NULL) {
        remove_alias(alias_list->name);
    }
}
