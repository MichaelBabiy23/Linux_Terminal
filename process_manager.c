// 323073734
#include "Headers/process_manager.h"
#include "Headers/string_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Process *process_list = NULL;
int active_processes = 0;
int total_processes = 0;
/**
 * Add a new process to the end process list.
 * @param pid The process ID.
 * @param command The command associated with the process.
 * @param status The status of the process.
 * @param has_quotes Indicates if the command has quotes.
 */
void add_process(pid_t pid, char* command, int has_quotes) {
    // Allocate memory for the new process
    Process *new_process = (Process *)malloc(sizeof(Process));
    if (new_process == NULL) {
        perror("malloc");
        exit(1);
    }

    // Set the attributes of the new process
    new_process->pid = pid;
    new_process->index = ++total_processes;
    new_process->command = strdup(command);
    new_process->has_quotes = has_quotes;
    new_process->next = NULL;

    // If the list is empty, make the new process the head of the list
    if (process_list == NULL) {
        process_list = new_process;
    } else {
        // Traverse the list to find the last node
        Process *current_process = process_list;
        while (current_process->next != NULL) {
            current_process = current_process->next;
        }
        // Append the new process to the end of the list
        current_process->next = new_process;
    }

    // Increment the count of active processes
    active_processes++;
    free(command);
}

/**
 * Remove a process from the process list.
 * @param pid The process ID of the process to be removed.
 */
void remove_process(pid_t pid) {
    Process *current_process = process_list;
    Process *prev_process = NULL;

    // Traverse the list to find the process with the given pid
    while (current_process != NULL && current_process->pid != pid) {
        prev_process = current_process;
        current_process = current_process->next;
    }

    // If the process is found, remove it from the list
    if (current_process != NULL) {
        if (prev_process != NULL) {
            prev_process->next = current_process->next;
        } else {
            process_list = current_process->next;
        }

        total_apostrophes += current_process->has_quotes;

        // Free the memory allocated for the command
        free(current_process->command);
        // Free the memory allocated for the process
        free(current_process);

        // Decrement the count of active processes
        active_processes--;
    }
}

/**
 * Print all processes in the process list.
 */
void print_process() {
    Process *current_process = process_list;
    while (current_process != NULL) {
        printf("[%d] Running \t\t%s &\n", current_process->index, current_process->command);
        current_process = current_process->next;
    }
}

/**
 * Free all memory allocated for the process list.
 */
void free_all_process() {
    Process *current_process = process_list;
    while (current_process != NULL) {
        Process *temp = current_process;
        current_process = current_process->next;
        free(temp->command);
        free(temp);
    }
    process_list = NULL;
    active_processes = 0;
}
