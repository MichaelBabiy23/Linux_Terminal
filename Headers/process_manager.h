// 323073734

#ifndef OSHOMEWORK1_PROCESS_MANAGER_H
#define OSHOMEWORK1_PROCESS_MANAGER_H

#include <sys/types.h>

typedef struct Process {
    pid_t pid;
    int index;
    char* command;
    int has_quotes;
    struct Process *next;
} Process;

extern Process *process_list;
extern int active_processes;

void add_process(pid_t pid, char* command, int has_quotes);
void remove_process(pid_t pid);
void print_process();
void free_all_process();

#endif //OSHOMEWORK1_PROCESS_MANAGER_H
