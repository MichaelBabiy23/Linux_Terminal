// 323073734
#ifndef COMMAND_EXECUTOR_H
#define COMMAND_EXECUTOR_H

void execute_command(char *input);
void execute_child_process(char *args[]);
void execute_script(char *script_file);
void shell_exit();
void alias_handler(char* input);
void sign_handler(char* input, char* and_sign, char* or_sign);

#endif
