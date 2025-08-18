#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    char *token;

    printf("$ ");
    nread = getline(&line, &len, stdin);
    if (nread == -1)
    {
        free(line);
        return 0;
    }

    token = strtok(line, " \t\r\n");
    while (token)
    {
        printf("%s\n", token);
        token = strtok(NULL, " \t\r\n");
    }

    free(line);
    return 0;
}
