#include "shell.h"
#include <stdio.h>
#include <stdlib.h>

void display_prompt(t_prompt *prompt)
{
    printf("%s", prompt->text);
}

int read_input(t_input *input)
{
    input->line = NULL;
    input->len = 0;

    if (getline(&input->line, &input->len, stdin) == -1)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

void free_input(t_input *input)
{
    if (input->line != NULL)
    {
        free(input->line);
        input->line = NULL;
    }
}


void parse_command(t_input *input, t_parse *parse)
{
    if (input->line == NULL)
    {
        parse->command = NULL;
        return;
    }

    /* Removing  \n  */
    size_t len = strlen(input->line);
    if (len > 0 && input->line[len - 1] == '\n')
        input->line[len - 1] = '\0';

    parse->command = strdup(input->line);
    if (parse->command == NULL)
    {
        perror("Erreur allocation parse->command");
    }
}

/* execution simple : fork + execve : (no arg no path) */
void execute_command(t_parse *parse, t_execute *exec)
{
    pid_t pid;

    if (parse->command == NULL || parse->command[0] == '\0')
    {
        exec->result = -1;
        return;
    }

    pid = fork();
    if (pid < 0)
    {
        perror("fork");
        exec->result = -1;
        return;
    }

    if (pid == 0)
    {
        /* Processus enfant */
        char *args[] = {parse->command, NULL};
        extern char **environ;
        if (execve(parse->command, args, environ) == -1)
        {
            perror("execve");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        /* Processus parent */
        int status;
        waitpid(pid, &status, 0);
        exec->result = (WIFEXITED(status)) ? WEXITSTATUS(status) : -1;
    }
}