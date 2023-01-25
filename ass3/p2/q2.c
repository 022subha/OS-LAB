#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        printf("At least provide one file\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i < argc; i++)
    {
        char *exeArgument[10];
        exeArgument[0] = argv[i];
        int j = 1;
        for (; j < 10 && i + 1 < argc && argv[i + 1][0] == '-'; j++)
        {
            i++;
            exeArgument[j] = argv[i];
        }
        exeArgument[j] = NULL;

        pid_t pid = fork();
        if (pid == 0)
        {
            if (execv(exeArgument[0], exeArgument) == -1)
            {
                printf("Not Executable File\n");
                exit(EXIT_FAILURE);
            }
            exit(EXIT_SUCCESS);
        }
        else if (pid > 0)
        {
            wait(NULL);
        }
    }
    exit(EXIT_SUCCESS);
}