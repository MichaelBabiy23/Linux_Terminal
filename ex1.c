// 323073734
#include <stdio.h>
#include <string.h>
#include <sys/errno.h>
#include "Headers/command_executor.h"
#include "Headers/alias_manager.h"
#include "Headers/constants.h"
#include "Headers/process_handler.h"

int success_commands = 0;
int total_script_lines = 0;
int total_apostrophes = 0;

void update_prompt() {
    printf("#cmd:%d|#alias:%d|#script lines:%d> ", success_commands, active_aliases, total_script_lines);
}

int main() {
    char input[MAX_LINE] = {0};

    setup_sigaction();

    while (1) {
        update_prompt();
        if (fgets(input, sizeof(input), stdin) == NULL) {
            if (errno == EINTR)
            {
                printf("\n");
                clearerr(stdin);
                continue;
            }
            else
                break;
        }
        input[strcspn(input, "\n")] = 0;
        execute_command(input);
    }
    return 0;
}