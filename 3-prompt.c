#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    char *line = NULL;          /* pointeur qui va contenir la ligne saisie */
    size_t buffer_size = 0;     /* taille allouée par getline */
    ssize_t chars_read;         /* nombre de caractères lus */

    while (1)
    {
        printf("$ ");
        chars_read = getline(&line, &buffer_size, stdin);
        if (chars_read == -1)   /* EOF (Ctrl+D) ou erreur */
            break;
        printf("%s", line);
    }
    free(line);
    return 0;
}
