// 323073734
#include <stdio.h>
#include <string.h>
#include "Headers/command_executor.h"
#include "Headers/alias_manager.h"
#include "Headers/constants.h"

int success_commands = 0;
int total_script_lines = 0;
int total_apostrophes = 0;

void update_prompt() {
    printf("#cmd:%d|#alias:%d|#script lines:%d> ", success_commands, active_aliases, total_script_lines);
}

int main() {
    char input[MAX_LINE];

    while (1) {
        update_prompt();
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        input[strcspn(input, "\n")] = 0;
        execute_command(input);
    }
    return 0;
}
/**
 * source command print 3 times instead of one
*/