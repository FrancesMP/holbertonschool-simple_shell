#include <stdio.h>
#include <unistd.h>

int main(void)
{
    printf("%u\n", (unsigned int)getppid());
    return 0;
}
