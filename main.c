#include "shell.h"
#define BUFFER_SIZE 120
/**
 *main - Entry pooint of the simple shell
 *
 *Function runs infinite loop to display prompt
 *It allocates space for the command input
 *
 *Return: Always 0 (success)
 */

int main(void)

{
	char command[120];
	while (1)
	{
		prompt();
		read_command(command, 120);
		if(command[0] != '\0')
		execute_command(command);
	}
	return (0); 
}
