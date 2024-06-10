// 323073734

#ifndef OSHOMEWORK1_PROCESS_HANDLER_H
#define OSHOMEWORK1_PROCESS_HANDLER_H

void execute_child_process(char *args[], int num_args);
void sigchld_handler(int sig);
void setup_sigaction();

#endif //OSHOMEWORK1_PROCESS_HANDLER_H
