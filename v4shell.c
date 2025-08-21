#include "shell.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>


void handle_sigint(int sig)
{
    (void)sig;          
    write(1, "\n$ ", 3); 
    fflush(stdout);
}

int main(void)
{
    t_shell shell;

    shell.prompt.text = "$ ";
    shell.running = 1;

    signal(SIGINT, handle_sigint);

    while (shell.running)
    {
        
        if (isatty(STDIN_FILENO))
            display_prompt(&shell.prompt);

        if (read_input(&shell.input) == -1)
        {
            if (isatty(STDIN_FILENO)) 
                printf("\n");
            break;
        }

        parse_command(&shell.input, &shell.parse);

        if (shell.parse.command != NULL)
        {
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
        }

        free_input(&shell.input);
        free_parse(&shell.parse);
    }

    return 0;
}
