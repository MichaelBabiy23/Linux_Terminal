// 323073734
#ifndef ALIAS_MANAGER_H
#define ALIAS_MANAGER_H

typedef struct Alias {
    char *name;
    char *command;
    struct Alias *next;
} Alias;

extern Alias *alias_list;
extern int active_aliases;

void add_alias(char *name, char *command);
void remove_alias(char *name);
char* get_alias_command(char *name);
void print_aliases();
void free_all_aliases();

#endif
