#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ;

/**
 * display_prompt - Prints the shell prompt
 * @prompt: Pointer to t_prompt structure
 *
 * Return: void
 */
void display_prompt(t_prompt *prompt)
{
    printf("%s", prompt->text);
}

/**
 * read_input - Reads a line from standard input
 * @input: Pointer to t_input structure
 *
 * Return: 0 on success, -1 on EOF or error
 */
int read_input(t_input *input)
{
    input->line = NULL;
    input->len = 0;

    if (getline(&input->line, &input->len, stdin) == -1)
        return (-1);
    return (0);
}

/**
 * free_input - Frees the memory allocated for input line
 * @input: Pointer to t_input structure
 *
 * Return: void
 */
void free_input(t_input *input)
{
    if (input->line != NULL)
    {
        free(input->line);
        input->line = NULL;
    }
}

/**
 * parse_command - Splits input line into command and arguments
 * @input: Pointer to t_input structure
 * @parse: Pointer to t_parse structure
 *
 * Return: void
 */
void parse_command(t_input *input, t_parse *parse)
{
    int i;
    size_t len;
    char *token_local;

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

    i = 0;
    token_local = strtok(input->line, " ");
    while (token_local != NULL && i < 63)
    {
        parse->argv[i] = strdup(token_local);
        if (!parse->argv[i])
        {
            perror("strdup");
            exit(EXIT_FAILURE);
        }
        i++;
        token_local = strtok(NULL, " ");
    }
    parse->argv[i] = NULL;
    parse->command = parse->argv[0];
}

/**
 * find_path - Finds the full path of a command using PATH environment
 * @command: Command name
 *
 * Return: Full path string if found, NULL otherwise
 */
char *find_path(char *command)
{
    char *path_env;
    char *path_copy;
    char *token_local;
    char *full_path;
    size_t len;

    path_env = get_env_value("PATH");
    if (!path_env)
        return (NULL);

    path_copy = strdup(path_env);
    if (!path_copy)
        return (NULL);

    token_local = strtok(path_copy, ":");
    while (token_local)
    {
        len = strlen(token_local) + strlen(command) + 2;
        full_path = malloc(len);
        if (!full_path)
        {
            free(path_copy);
            return (NULL);
        }
        snprintf(full_path, len, "%s/%s", token_local, command);

        if (access(full_path, X_OK) == 0)
        {
            free(path_copy);
            return (full_path);
        }

        free(full_path);
        token_local = strtok(NULL, ":");
    }

    free(path_copy);
    return (NULL);
}
/**
 * execute_command - Executes a command using fork and execvp
 * @parse: Pointer to t_parse structure
 * @exec: Pointer to t_execute structure
 *
 * Return: Command exit status
 */
int execute_command(t_parse *parse, t_execute *exec)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return (-1);
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
    return (exec->result);
}

/**
 * free_parse - Frees the memory allocated for parse structure
 * @parse: Pointer to t_parse structure
 *
 * Return: void
 */
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

/**
 * get_env_value - Returns the value of an environment variable
 * @name: Environment variable name
 *
 * Return: Pointer to value string, or NULL if not found
 */
char *get_env_value(const char *name)
{
    int i;
    size_t len;

    len = strlen(name);
    for (i = 0; environ[i]; i++)
    {
        if (strncmp(environ[i], name, len) == 0 && environ[i][len] == '=')
            return (environ[i] + len + 1);
    }
    return (NULL);
}
