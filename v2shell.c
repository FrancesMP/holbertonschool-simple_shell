#include "shell.h"

int main(void)
{
    t_shell shell;

    shell.prompt.text = "$ ";
    shell.running = 1;

    while (shell.running)
    {
        display_prompt(&shell.prompt);

        if (read_input(&shell.input) == -1)
        {
            
            printf("\nBye!\n");
            break;
        }

        parse_command(&shell.input, &shell.parse);

        execute_command(&shell.parse, &shell.exec);

        if (shell.exec.result != 0)
            printf("Erreur exécution de '%s'\n", shell.parse.command);

        free_input(&shell.input);
        free(shell.parse.command);
    }

    return 0;
}
