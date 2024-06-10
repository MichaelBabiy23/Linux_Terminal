// 323073734
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include "Headers/string_utils.h"
#include "Headers/command_executor.h"
#include "Headers/process_handler.h"
#include "Headers/process_manager.h"

extern int total_script_lines;
extern int success_commands;

/**
 * Executes the given command in a child process.
 * @param args The command arguments.
 */
void execute_child_process(char *args[], int num_args) {
    int status = 0;
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        // printf("Im stupid child --------------------\n");
        // Child process
        if (execvp(args[0], args) < 0) {
            perror("exec");
            _exit(1);
            // _exit instead of simple exit to avoid looping
        }
    } else {
        // Parent process
        sigset_t mask;
        if (!background_process_flag)
        {
            sigemptyset(&mask);
            sigaddset(&mask, SIGCHLD);
            sigprocmask(SIG_BLOCK, &mask, NULL); // Block SIGCHLD signal
            waitpid(pid, &status, 0); // Wait for foreground process
            if(WEXITSTATUS(status) == 0) {
                success_commands++;
                total_apostrophes += has_quotes(args);
            }
            sigprocmask(SIG_UNBLOCK, &mask, NULL); // Unblock SIGCHLD signal

        }
        else
        {
            add_process(pid, concatenate_arguments(num_args, args), status, has_quotes(args));
            printf("[%d]%d\n", active_processes, pid);
            sleep(1);
        }
    }
}

/**
 * Handler for SIGCHLD signal to clean up zombie processes
 * @param sig
 */
void sigchld_handler(int sig) {
    // printf("In handler :)\n");
    int status;
    pid_t pid;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        if(WEXITSTATUS(status) == 0) {
            success_commands++;
        }
        remove_process(pid);
        // printf("Cleaned up zombie process with PID %d\n", pid);
    }
}

/**
 * Set up signal handling for SIGCHLD to clean up zombie processes
 */
void setup_sigaction()
{
    struct sigaction sa_chld;
    sa_chld.sa_handler = sigchld_handler;
    sigemptyset(&sa_chld.sa_mask);
    sa_chld.sa_flags = 0; // Restart interrupted system calls
    if (sigaction(SIGCHLD, &sa_chld, NULL) == -1) {
        perror("sigaction");
    }
}