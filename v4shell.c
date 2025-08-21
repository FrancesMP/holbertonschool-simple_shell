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
    int cmd_count = 0;
    int status = 0;

    shell.prompt.text = "$ ";
    shell.running = 1;
    shell.input.line = NULL;
    shell.input.len = 0;
    shell.input.status = 0;
    shell.parse.command = NULL;
    shell.parse.argv = NULL;

    signal(SIGINT, handle_sigint);

    while (shell.running)
    {
        if (isatty(STDIN_FILENO))
            display_prompt(&shell.prompt);

        if (read_input(&shell.input) == -1)
        {
            free_input(&shell.input);  
            if (isatty(STDIN_FILENO))
                printf("\n");
            break;
        }

        cmd_count++; /* compteur de commandes */
        parse_command(&shell.input, &shell.parse);

        if (shell.parse.command != NULL)
        {
            if (strcmp(shell.parse.command, "exit") == 0)
            {
                shell.running = 0;
            }
            else if (strcmp(shell.parse.command, "cd") == 0)
            {
                if (shell.parse.argv[1])  
                {
                    if (chdir(shell.parse.argv[1]) != 0)
                        perror("cd");
                }
                else
                {
                    /* getenv est interdit → fallback vers "/" */
                    if (chdir("/") != 0)
                        perror("cd");
                }
            }
            else
            {
                if (execute_command(&shell.parse, &shell.exec) == -1)
                {
                    fprintf(stderr, "./hsh: %d: %s: not found\n",
                            cmd_count, shell.parse.command);
                    status = 127;
                }
            }
        }

        free_input(&shell.input);  
        free_parse(&shell.parse);  
    }

    
    free_input(&shell.input);
    free_parse(&shell.parse);

    return status;
}
