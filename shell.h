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

/* Global environ */
extern char **environ;

/* Function prototypes */
void prompt(void);
void read_command(char *command, size_t size);
int execute_command(char *argv[], char *progname, int cmd_count);
char *find_command(char *command);
void print_env(void);

#endif
