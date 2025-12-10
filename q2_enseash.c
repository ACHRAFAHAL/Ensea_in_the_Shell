#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define welcome "\nBienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\n"
#define Promt "enseash % "
#define bufsize 256

/*
    Display a message to stdout using write() 
*/ 
void display(const char *message) {
    write(STDOUT_FILENO, message, strlen(message));
}

/*
    Read command from standard input STDIN and remove trailing newline (\n when hitting Enter)
    funtion used to add 0 at the end of the string to mark its end
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
    Execute a simple command without arguments using fork() to create the child
    process and execvp()
*/
void execute_command(char *command) {
    pid_t pid = fork();
    
    if (pid == -1) {
        display("fork failed\n");
        return;
    }
    
    if (pid == 0) {
        char *args[] = {command, NULL};
        execvp(command, args);
        
        display("command not found\n");
        _exit(1);
    } else {
        int status;
        waitpid(pid, &status, 0);
    }
}

int main() {
    char command[bufsize];
    
    display(welcome);
    
    // REPL loop 
    while (1) {
        display(Promt);
        read_command(command, bufsize);
        
        if (strlen(command) == 0) {
            continue;
        }
        
        execute_command(command);
    }
    
    return 0;
}