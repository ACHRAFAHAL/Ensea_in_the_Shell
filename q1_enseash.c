#include <unistd.h>
#include <string.h>

#define BIENVENUE_MSG "Bienvenue dans le Shell ENSEA.\n\rPour quitter taper 'exit'\n"
#define PROMPT "enseash %\n"

int main()
{

    write(STDOUT_FILENO, BIENVENUE_MSG, strlen(BIENVENUE_MSG));
    write(STDOUT_FILENO, PROMPT, strlen(PROMPT));

    return 0;
}

