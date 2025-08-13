#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    FILE *f = fopen("/proc/sys/kernel/pid_max", "r");
    if (!f)
    {
        perror("fopen");
        return 1;
    }
    char buffer[32];
    if (fgets(buffer, sizeof(buffer), f))
        printf("%s", buffer);
    fclose(f);
    return 0;
}
