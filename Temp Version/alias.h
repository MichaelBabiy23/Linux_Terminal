// 323073734
#ifndef OSHOMEWORK1_ALIAS_H
#define OSHOMEWORK1_ALIAS_H

// Structure which representing an alias
typedef struct Alias {
    char *name;
    char *command;
    struct Alias *next;
} Alias;

void add_alias(char *name, char *command);
void remove_alias(char *name);
char* get_alias_command(char *name);
void print_aliases();

extern int active_aliases;

#endif
