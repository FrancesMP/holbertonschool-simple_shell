#ifndef SHELL_H
#define SHELL_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
*
*
*
*
*/ 

void prompt(void);
void read_command(char *command, size_t size);
void execute_command(const char *command);



#endif
