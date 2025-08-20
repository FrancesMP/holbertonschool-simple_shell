#include "shell.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    t_shell shell;

    shell.prompt.text = "$ ";
    shell.running = 1;

    while (shell.running)
    {
        display_prompt(&shell.prompt);

        if (read_input(&shell.input) == -1)
        {
            printf("\n");
            break;
        }

        parse_command(&shell.input, &shell.parse);

        /* commandes simples */
        if (strcmp(shell.parse.command, "exit") == 0)
            shell.running = 0;
        else if (strcmp(shell.parse.command, "cd") == 0)
        {
            if (chdir("/") != 0)
                perror("cd");
        }
        else
        {
            execute_command(&shell.parse, &shell.exec);
        }

        free_input(&shell.input);
    }

    return 0;
}

