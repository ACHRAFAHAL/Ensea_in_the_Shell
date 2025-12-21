#ifndef SHELL_H
#define SHELL_H

#include <sys/types.h>

ssize_t read_command(char *buffer, size_t buffer_size);
int is_exit_command(const char *command);
int parse_command(char *command, char **args, char **input_file, char **output_file);
void handle_redirections(char *input_file, char *output_file);
int execute_command(char *command, long *exec_time_ms);

#endif
