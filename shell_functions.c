#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

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
    size_t len;
    char *token;
    int i = 0;

    if (input->line == NULL)
    {
        parse->command = NULL;
        parse->argv = NULL;
        return;
    }

    /* Remove \n */
    len = strlen(input->line);
    if (len > 0 && input->line[len - 1] == '\n')
        input->line[len - 1] = '\0';

    /* Découper en tokens (command + args) */
    parse->argv = malloc(sizeof(char *) * 64);
    if (!parse->argv)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    token = strtok(input->line, " ");
    while (token != NULL && i < 63)
    {
        parse->argv[i] = strdup(token);
        if (!parse->argv[i])
        {
            perror("strdup");
            exit(EXIT_FAILURE);
        }
        i++;
        token = strtok(NULL, " ");
    }
    parse->argv[i] = NULL; /* NULL-terminated array */

    parse->command = (i > 0) ? parse->argv[0] : NULL;
}

char *find_path(char *command)
{
    char *path_env;
    char *path_copy;
    char *token;
    char *full_path;
    size_t len;

    path_env = get_env_value("PATH");
    if (!path_env)
        return NULL;

    path_copy = strdup(path_env); /* copie car strtok modifie la chaîne */
    if (!path_copy)
        return NULL;

    token = strtok(path_copy, ":");
    while (token)
    {
        len = strlen(token) + strlen(command) + 2; /* "/" + '\0' */
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

/* execution with args and PATH handling */
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
    int i;

    if (parse->argv)
    {
        for (i = 0; parse->argv[i] != NULL; i++)
            free(parse->argv[i]);  /* libère chaque strdup */
        free(parse->argv);
        parse->argv = NULL;
    }

    parse->command = NULL;
}
char *get_env_value(const char *name)
{
    int i;
    size_t len = strlen(name);

    for (i = 0; environ[i]; i++)
    {
        if (strncmp(environ[i], name, len) == 0 && environ[i][len] == '=')
        {
            return environ[i] + len + 1; 
        }
    }
    return NULL;
}
