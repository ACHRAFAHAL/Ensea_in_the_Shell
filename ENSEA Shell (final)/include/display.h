#ifndef DISPLAY_H
#define DISPLAY_H

#include <unistd.h>
#include <string.h>

void display(const char *message);
void display_prompt(int status, long exec_time_ms);

#endif
