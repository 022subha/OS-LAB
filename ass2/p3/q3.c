#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{
    FILE *fp = fopen("sample.txt", "w");
    if (fp == NULL)
    {
        printf("Error\n");
        exit(1);
    }
    pid_t pid = fork();
    if (pid < 0)
    {
        printf("Fork failed\n");
        exit(1);
    }
    else if (pid == 0)
    {
        fprintf(fp, "%s\n", "This is child process");
    }
    else
    {
        wait(NULL);
        fprintf(fp, "%s\n", "This is parent process");
    }
    return 0;
}
