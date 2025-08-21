#include "shell.h"
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>

#define MAX_ARGS 64
#define BUFFER_SIZE 120

/* Ctrl+C handler */
void sigint_handler(int sig)
{
    (void)sig;
    write(1, "\n$$$ ", 5); /* Print prompt again */
    fflush(stdout);
}

int main(int argc, char **argv)
{
    char line[BUFFER_SIZE];
    char *args[MAX_ARGS];
    char *token;
    int cmd_count = 0;
    int i;

    (void)argc;

    /* Set Ctrl+C handler */
    signal(SIGINT, sigint_handler);

    while (1)
    {
        prompt();
        read_command(line, BUFFER_SIZE);

        if (line[0] == '\0')
            continue;

        /* Split input line into arguments */
        i = 0;
        token = strtok(line, " ");
        while (token && i < MAX_ARGS - 1)
        {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        cmd_count++;

        /* Built-ins */
        if (strcmp(args[0], "exit") == 0)
            exit(0);

        if (strcmp(args[0], "env") == 0)
        {
            print_env();
            continue;
        }

        /* Execute external command */
        execute_command(args, argv[0], cmd_count);
    }

    return 0;
}
