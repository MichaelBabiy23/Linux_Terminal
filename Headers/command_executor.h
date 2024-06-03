// 323073734
#ifndef COMMAND_EXECUTOR_H
#define COMMAND_EXECUTOR_H

void execute_command(char *input);
void execute_child_process(char *args[]);
void execute_script(char *script_file);
void shell_exit();

#endif
