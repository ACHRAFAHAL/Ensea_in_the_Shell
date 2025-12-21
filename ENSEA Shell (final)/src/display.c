#include "display.h"
#include "config.h"
#include <stdio.h>
#include <sys/wait.h>

/*
    Display a message to stdout using write() 
*/ 
void display(const char *message) {
    write(STDOUT_FILENO, message, strlen(message));
}

/*
    Build and display prompt with exit status or signal and execution time
*/
void display_prompt(int status, long exec_time_ms) {
    char prompt[bufsize];
    
    if (WIFEXITED(status)) {
        sprintf(prompt, "%s [exit:%d|%ldms] %% ", Promt, WEXITSTATUS(status), exec_time_ms);
    } else if (WIFSIGNALED(status)) {
        sprintf(prompt, "%s [sign:%d|%ldms] %% ", Promt, WTERMSIG(status), exec_time_ms);
    } else {
        sprintf(prompt, "%s %% ", Promt);
    }
    
    display(prompt);
}
