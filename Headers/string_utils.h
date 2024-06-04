// 323073734
#ifndef STRING_UTILS_H
#define STRING_UTILS_H

void parse_arguments(char *input, char *args[], int *num_args);
void remove_first_last_char(char *arg);
void remove_multiple_spaces(char *input);
void remove_spaces_around_equals(char *input);
int has_balanced_quotes(const char *input);
char* concatenate_arguments(int num_args, char *args[]);
int split_alias(const char *input, char *command, char *name);
int has_space_around_equal(const char *str);
void remove_quotes(char *str);
int has_quotes(char *args[]);

extern int total_apostrophes;
#endif
