#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <stdio.h>

#define welcome "\nBienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\n"
#define Promt "enseash"
#define exit_message "Bye bye...\n"
#define exit_command "exit"
#define bufsize 256
#define max_arg 32

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

/*
    Read command from standard input STDIN and remove trailing newline
    Returns the number of bytes read (excluding newline)
 */
ssize_t read_command(char *buffer, size_t buffer_size) {
    ssize_t input_bytes = read(STDIN_FILENO, buffer, buffer_size - 1);

    if (input_bytes > 0) {
        buffer[input_bytes] = '\0';
        
        if (input_bytes > 0 && buffer[input_bytes - 1] == '\n') {
            buffer[input_bytes - 1] = '\0';
        }
    }
    
    return input_bytes;  
}

/*
    Check if the command is the exit command
    Returns 1 if it's "exit", 0 otherwise
*/
int is_exit_command(const char *command) {
    return strncmp(command, exit_command, strlen(exit_command)) == 0 &&
           strlen(command) == strlen(exit_command);
}

/*
    Parse command line into arguments array with the spliting condition is space and tabs using strtok()
    Returns the number of arguments
*/
int parse_command(char *command, char **args) {
    int argc = 0;
    char *token = strtok(command, " \t");
    
    while (token != NULL && argc < max_arg - 1) {
        args[argc++] = token;
        token = strtok(NULL, " \t");
    }
    
    args[argc] = NULL;
    return argc;
}

/*
    Execute a command with arguments and measure execution time
    Returns the status of the executed command
*/
int execute_command(char *command, long *exec_time_ms) {
    pid_t pid = fork();
    int status = 0;
    struct timespec start, end;
    char *args[max_arg];
    
    if (pid == -1) {
        display("fork failed\n");
        return -1;
    }
    
    if (pid == 0) {
        parse_command(command, args);
        
        if (args[0] == NULL) {
            _exit(0);
        }
        
        execvp(args[0], args);
        display("command not found\n");
        _exit(127);
    } else {
        clock_gettime(CLOCK_MONOTONIC, &start);
        waitpid(pid, &status, 0);
        clock_gettime(CLOCK_MONOTONIC, &end);
        
        *exec_time_ms = (end.tv_sec - start.tv_sec) * 1000 + 
                        (end.tv_nsec - start.tv_nsec) / 1000000;
        
        return status;
    }
}

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