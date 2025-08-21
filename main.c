#include "shell.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* Gestion du Ctrl+C */
void handle_sigint(int sig)
{
    (void)sig;
    write(1, "\n$ ", 3);
    fflush(stdout);
}

int main(void)
{
    t_shell shell;
    int exit_status = 0;
    ssize_t nread;

    shell.prompt.text = "$ ";
    shell.running = 1;
    shell.input.line = NULL;
    shell.input.len = 0;
    shell.input.status = 0;
    shell.parse.command = NULL;
    shell.parse.argv = NULL;
    shell.exec.result = 0;

    signal(SIGINT, handle_sigint);

    while (shell.running)
    {
        if (isatty(STDIN_FILENO))
            display_prompt(&shell.prompt);

        nread = read_input(&shell.input);
        if (nread == -1) /* EOF ou erreur */
        {
            free_input(&shell.input);
            if (isatty(STDIN_FILENO))
                printf("\n");
            break;
        }

        if (shell.input.line[0] == '\0') /* Ligne vide */
        {
            free_input(&shell.input);
            continue;
        }

        parse_command(&shell.input, &shell.parse);

        if (shell.parse.command != NULL)
        {
            if (strcmp(shell.parse.command, "exit") == 0)
            {
                shell.running = 0;
                exit_status = shell.exec.result;
            }
            else if (strcmp(shell.parse.command, "cd") == 0)
            {
                if (shell.parse.argv[1])
                {
                    if (chdir(shell.parse.argv[1]) != 0)
                        perror("cd");
                    shell.exec.result = (chdir(shell.parse.argv[1]) == 0) ? 0 : 1;
                }
                else
                {
                    chdir(getenv("HOME"));
                    shell.exec.result = 0;
                }
            }
            else
            {
                execute_command(&shell.parse, &shell.exec);
            }
        }

        free_input(&shell.input);
        free_parse(&shell.parse);

        if (!isatty(STDIN_FILENO)) /* Mode non-interactif : sortir après une commande */
            break;
    }

    free_input(&shell.input);
    free_parse(&shell.parse);

    return exit_status;
}
