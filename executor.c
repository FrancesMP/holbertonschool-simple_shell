#include "shell.h"

/**
 * execute_command - Executes a command with its arguments
 * @argv: Array of command and arguments
 * @progname: Name of the program
 * @cmd_count: Number of commands executed
 * Return: 1 if command was executed, 0 if not
 */
int execute_command(char *argv[], char *progname, int cmd_count)
{
	pid_t pid;
	char *command_path;
	int status;

	/* Find command in PATH */
	command_path = find_command(argv[0]);
	if (!command_path)
	{
		fprintf(stderr, "%s: %d: %s: not found\n", progname, cmd_count, argv[0]);
		return (127);
	}

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		free(command_path);
		return (-1);
	}
	else if (pid == 0)
	{
		/* Child process */
		if (execve(command_path, argv, environ) == -1)
		{
			perror(argv[0]);
			free(command_path);
			exit(126);
		}
	}
	else
	{
		/* Parent process */
		wait(&status);
		free(command_path);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status)); /* Return the real code of the child process */
		else
			return (-1); /* Execution error */
	}
	return (1);
}
