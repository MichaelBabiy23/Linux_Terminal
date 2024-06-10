// 323073734
#ifndef COMMAND_EXECUTOR_H
#define COMMAND_EXECUTOR_H

void execute_command(char *input);
void execute_child_process(char *args[], int i);
void execute_script(char *script_file);
void shell_exit();
void alias_handler(char* input);
void or_and_sign_handler(char* input, char* and_sign, char* or_sign);

extern int background_process_flag;


#endif
