#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/**
 *read_command - Reads a command from the user
 *@command: buffer to store the input
 *@size: size of the buffer
 */

void read_command(char *command, size_t size)
{
 size_t i = 0;

    if (fgets(command, size, stdin) == NULL)
    {
        if (feof(stdin)) // Ctrl+D
        {
            printf("\n");
            exit(EXIT_SUCCESS);
        }
        else
        {
            perror("fgets failed");
            exit(EXIT_FAILURE);
        }
    }

    while (command[i] != '\0')
        i++;
    if (i > 0 && command[i - 1] == '\n')
        command[i - 1] = '\0';
}
