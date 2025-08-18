#include "shell.h"

int main(void)
{
    t_shell shell;

    
    shell.prompt.text = "$ ";  
    shell.input.line = NULL;
    shell.input.len = 0;
    shell.running = 1;

    while (shell.running)
    {
        
        display_prompt(&shell.prompt);

        
        if (read_input(&shell.input) == -1)
        {
            /* Ctrl+D  :  EOF  ou erreur  */
            printf("\n");
            break;
        }

       
        printf("You just typed :  %s", shell.input.line);

        /* free before the next */
        free_input(&shell.input);
        shell.input.line = NULL;
        shell.input.len = 0;
    }

    return 0;
}
