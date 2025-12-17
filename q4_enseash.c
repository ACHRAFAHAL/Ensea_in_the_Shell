#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define welcome "\nBienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\n"
#define Promt "enseash"
#define exit_message "Bye bye...\n"
#define exit_command "exit"
#define bufsize 256

/*
    Display a message to stdout using write() 
*/ 
void display(const char *message) {
    write(STDOUT_FILENO, message, strlen(message));
}

/*
    Convert integer to string by dividing each time by 10 and storing remainders
*/
void int_to_string(int value, char *buffer) {
    int i = 0;
    
    if (value == 0) {
        buffer[i++] = '0';
    } else {
        char temp[20];
        int j = 0;
        
        while (value > 0) {
            temp[j++] = '0' + (value % 10);
            value /= 10;
        }
        
        while (j > 0) {
            buffer[i++] = temp[--j];
        }
    }
    
    buffer[i] = '\0';
}

/*
    Build and display prompt with exit status if process is terminated normally 
    or signal if terminated by a signal
*/
void display_prompt(int status) {
    char prompt[bufsize];
    char status_str[20];
    
    strncpy(prompt, Promt, bufsize - 1);
    prompt[bufsize - 1] = '\0';
    strncat(prompt, " ", bufsize - strlen(prompt) - 1);
    
    if (WIFEXITED(status)) {
        strncat(prompt, "[exit:", bufsize - strlen(prompt) - 1);
        int_to_string(WEXITSTATUS(status), status_str);
        strncat(prompt, status_str, bufsize - strlen(prompt) - 1);
        strncat(prompt, "]", bufsize - strlen(prompt) - 1);
    } else if (WIFSIGNALED(status)) {
        strncat(prompt, "[sign:", bufsize - strlen(prompt) - 1);
        int_to_string(WTERMSIG(status), status_str);
        strncat(prompt, status_str, bufsize - strlen(prompt) - 1);
        strncat(prompt, "]", bufsize - strlen(prompt) - 1);
    }
    
    strncat(prompt, " % ", bufsize - strlen(prompt) - 1);
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
            input_bytes--;
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
    Execute a simple command without arguments using fork() to create the child process and execvp()
    Returns the status of the executed command
*/
int execute_command(char *command) {
    pid_t pid = fork();
    int status = 0;
    
    if (pid == -1) {
        display("fork failed\n");
        return -1;
    }
    
    if (pid == 0) {
        char *args[] = {command, NULL};
        execvp(command, args);
        display("command not found\n");
        _exit(127);
    } else {
        waitpid(pid, &status, 0);
        return status;
    }
}

int main() {
    char command[bufsize];
    ssize_t input_bytes;
    int status = 0;
    int first_prompt = 1;
    
    display(welcome);
    
    // REPL loop 
    while (1) {
        // Display prompt with status
        if (first_prompt) {
            display(Promt);
            display(" % ");
            first_prompt = 0;
        } else {
            display_prompt(status);
        }
        
        input_bytes = read_command(command, bufsize);
        
        // Manage Ctrl+D input
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
        
        status = execute_command(command);
    }
    
    return 0;
}