#include "shell.h"

/**
 * find_command - Finds the full path of a command in PATH
 * @command: command name
 * Return: full path (malloc'ed), NULL if not found
 */
char *find_command(char *command)
{
    char *path_env = getenv("PATH");
    char *path_dup, *dir;
    char *full_path;
    size_t len;

    if (!path_env)
        return NULL;

    /* Direct path given */
    if (strchr(command, '/'))
    {
        if (access(command, X_OK) == 0)
            return strdup(command);
        return NULL;
    }

    path_dup = strdup(path_env);
    dir = strtok(path_dup, ":");
    while (dir)
    {
        len = strlen(dir) + strlen(command) + 2;
        full_path = malloc(len);
        if (!full_path)
            exit(EXIT_FAILURE);

        snprintf(full_path, len, "%s/%s", dir, command);
        if (access(full_path, X_OK) == 0)
        {
            free(path_dup);
            return full_path;
        }

        free(full_path);
        dir = strtok(NULL, ":");
    }

    free(path_dup);
    return NULL;
}

