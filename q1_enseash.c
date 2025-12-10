#include <unistd.h>
#include <string.h>

#define welcome "Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\n"
#define Promt "enseash % "
#define bufsize 256

void display(const char *message) {
    write(STDOUT_FILENO, message, strlen(message));
}

int main() {
    display("\n");
    display(welcome);
    display(Promt);
    return 0;
}