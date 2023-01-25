#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        printf("Atleast provide 1 files\n");
        return 1;
    }

    for (int i = 1; i < argc; i++)
    {
        pid_t pid = fork();

        if (pid > 0)
        {
            wait(NULL);
        }
        if (pid == 0)
        {
            printf("%d----%s\n", i, argv[i]);
            if (execv(argv[i], (char *[]){argv[i], NULL}) == -1)
            {
                printf("Error in Executable File\n");
            }
            return 0;
        }
    }
}
