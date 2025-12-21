#include "config.h"
#include "display.h"
#include "shell.h"
#include <string.h>

int main() {
    char command[bufsize];
    ssize_t input_bytes;
    int status = -1;
    long exec_time_ms = 0;
    
    display(welcome);
    
    // REPL loop 
    while (1) {
        if (status == -1) {
            display(Promt);
            display(" % ");
        } else {
            display_prompt(status, exec_time_ms);
        }
        
        input_bytes = read_command(command, bufsize);
        
        if (input_bytes <= 0) {
            display("\n");
            display(exit_message);
            break;
        }
        
        if (strlen(command) == 0) {
            continue;
        }
        
        if (is_exit_command(command)) {
            display(exit_message);
            break;
        }
        
        status = execute_command(command, &exec_time_ms);
    }
    
    return 0;
}
