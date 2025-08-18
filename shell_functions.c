#include "shell.h"
#include <stdio.h>
#include <stdlib.h>

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
