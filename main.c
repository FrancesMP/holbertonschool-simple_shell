#include "shell.h"
#include <string.h>

#define MAX_ARGS 64
#define BUFFER_SIZE 120

/**
 *main - Entry pooint of the simple shell
 *
 *Function runs infinite loop to display prompt
 *It allocates space for the command input
 *
 *Return: Always 0 (success)
 */

int main(int argc, char **argv)

{
    char line[BUFFER_SIZE];
    char *args[MAX_ARGS];
    char *token;
    int cmd_count = 0;
    int i;

    (void)argc;

    while (1)
    {
        prompt();
        read_command(line, BUFFER_SIZE);

        if (line[0] == '\0')
            continue;

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

        execute_command(args, argv[0], cmd_count);
    }

    return 0;
}
