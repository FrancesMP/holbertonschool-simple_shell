#include "shell.h"
#include <sys/types.h>
#include <sys/wait.h>

/**
 * execute_command - Executes a single command
 * @command: full path or single-word command
 */

void execute_command(const char *command)
{
	pid_t pid = fork();  /* Create child process */

	if (pid < 0)
	{
		perror("Fork failed");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		char *argv[2];

		argv[0] = (char *)command;
		argv[1] = NULL;

		execve(command, argv, NULL);


		perror("Command execution failed");
		exit(EXIT_FAILURE);
	}
	else
	{
		wait(NULL);  /* Parent waits for child */
	}
}

