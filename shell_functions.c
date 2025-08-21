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
    char *token;
    int i = 0;
    size_t len; 

    if (!input->line)
    {
        parse->command = NULL;
        parse->argv = NULL;
        return;
    }

    len = strlen(input->line); 
    if (len > 0 && input->line[len - 1] == '\n')
        input->line[len - 1] = '\0';

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
    parse->argv[i] = NULL;
    parse->command = parse->argv[0];
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

    path_copy = strdup(path_env);
    if (!path_copy)
        return NULL;

    token = strtok(path_copy, ":");
    while (token)
    {
        len = strlen(token) + strlen(command) + 2;
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
    return NULL;
}

int execute_command(t_parse *parse, t_execute *exec)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return -1;
    }
    else if (pid == 0) 
    {
        execvp(parse->command, parse->argv);
        fprintf(stderr, "./hsh: 1: %s: not found\n", parse->command);
        exit(127);
    }
    else 
    {
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            exec->result = WEXITSTATUS(status);
    }
    return exec->result;
}



void free_parse(t_parse *parse)
{
    int i;

    if (parse->argv)
    {
        for (i = 0; parse->argv[i] != NULL; i++)
            free(parse->argv[i]);
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
