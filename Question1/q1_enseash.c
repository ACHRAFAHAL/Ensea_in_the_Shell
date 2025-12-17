#include <unistd.h>
#include <string.h>

#define welcome "Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\n"
#define Prompt "enseash % "
#define bufsize 256

// Display a message to stdout using write() 
void display(const char *message) {
    write(STDOUT_FILENO, message, strlen(message));
}

int main() {
    display(welcome);
    display(Prompt);
    return 0;
}