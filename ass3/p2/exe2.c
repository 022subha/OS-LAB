#include <stdio.h>
int main(int argc, char *argv[])
{
    printf("This is executable file 2\n");
    for (int i = 1; i < argc; i++)
    {
        printf("%s\n", argv[i]);
    }
    return 0;
}