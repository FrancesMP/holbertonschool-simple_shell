#include "shell.h"
#include <stdio.h>
#include <stdlib.h>

extern char **environ;

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
        parse->argv = NULL;
        return;
    }

    /* Remove \n */
    size_t len = strlen(input->line);
    if (len > 0 && input->line[len - 1] == '\n')
        input->line[len - 1] = '\0';

    /* Découper en tokens (command + args) */
    char *token = strtok(input->line, " ");
    int i = 0;

    /* On suppose max 64 arguments */
    parse->argv = malloc(sizeof(char *) * 64);
    if (!parse->argv)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    while (token != NULL && i < 63)
    {
        parse->argv[i++] = strdup(token);
        token = strtok(NULL, " ");
    }
    parse->argv[i] = NULL; /* NULL-terminated array */

    parse->command = (i > 0) ? parse->argv[0] : NULL;
}


/* execution simple : fork + execve : (no arg no path) */
/* void execute_command(t_parse *parse, t_execute *exec)
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
        / Processus enfant /
  /       char *args[] = {parse->command, NULL};
        extern char **environ;
        if (execve(parse->command, args, environ) == -1)
        {
            perror("execve");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        / Processus parent /
       / int status;
        waitpid(pid, &status, 0);
        exec->result = (WIFEXITED(status)) ? WEXITSTATUS(status) : -1; /
     }
}  */

char *find_path(char *command)
{
    char *path_env = getenv("PATH");
    char *token, *full_path;
    size_t len;

    if (!path_env)
        return NULL;

    char *path_copy = strdup(path_env);  /* strdup car strtok modifie la chaîne */    if (!path_copy)
        return NULL;

    token = strtok(path_copy, ":");
    while (token)
    {
        len = strlen(token) + strlen(command) + 2; /*  "/" + '\0' */
        full_path = malloc(len);
        if (!full_path)
        {
            free(path_copy);
            return NULL;
        }
        snprintf(full_path, len, "%s/%s", token, command);

        if (access(full_path, X_OK) == 0)
        {
            free(path_copy);
            return full_path;
        }

        free(full_path);
        token = strtok(NULL, ":");
    }

    free(path_copy);
    return NULL; /* pas trouvé */
}

/*execution with args and PATH handling */
void execute_command(t_parse *parse, t_execute *exec)
{
    pid_t pid;
    char *full_path;

    if (parse->command == NULL)
    {
        exec->result = -1;
        return;
    }

    /* Cherche le binaire dans le PATH si besoin */
    if (strchr(parse->command, '/'))
        full_path = strdup(parse->command); /* chemin absolu/relatif */
    else
        full_path = find_path(parse->command);

    if (!full_path)
    {
        fprintf(stderr, "Command not found: %s\n", parse->command);
        exec->result = -1;
        return;
    }

    pid = fork();
    if (pid == 0)
    {
        execve(full_path, parse->argv, environ);
        perror("execve");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        wait(NULL);
        exec->result = 0;
    }
    else
    {
        perror("fork");
        exec->result = -1;
    }

    free(full_path);
}

void free_parse(t_parse *parse)
{
    if (parse->argv)
    {
        for (int i = 0; parse->argv[i]; i++)
            free(parse->argv[i]);
        free(parse->argv);
        parse->argv = NULL;
    }
    parse->command = NULL;
}
