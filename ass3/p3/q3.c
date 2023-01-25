#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        printf("Atleast provide 1 file\n");
        exit(EXIT_FAILURE);
    }
    int index = 0;
    pid_t childProcess_pid[argc + 1];
    for (int i = 1; i < argc; i++)
    {
        pid_t pid = fork();
        childProcess_pid[index++] = pid;
        if (pid == -1)
        {
            printf("Error in fork\n");
            exit(EXIT_FAILURE);
        }
        if (pid == 0)
        {
            printf("Executing %s file\n", argv[i]);
            int value = execv(argv[i], (char *[]){argv[i], NULL});
            if (value == -1)
            {
                printf("Error in executable file\n");
                exit(EXIT_FAILURE);
            }
            exit(EXIT_SUCCESS);
        }
    }
    for (int i = 0; i < index; i++)
    {
        int wstatus;
        do
        {
            pid_t w = waitpid(childProcess_pid[i], &wstatus, 0);

            if (w == -1)
            {
                exit(EXIT_FAILURE);
            }
            printf("pid of child==>%d\n", childProcess_pid[i]);

            if (WIFEXITED(wstatus))
            {
                printf("Terminated Normally\nStatus=%d\n", WEXITSTATUS(wstatus));
            }
            else if (WIFSIGNALED(wstatus))
            {
                printf("Terminated by a signal\nNumber of thee signal=%d", WTERMSIG(wstatus));
            }
            else if (WIFSTOPPED(wstatus))
            {
                printf("Stopped by a signal\nNumber of the signal=%d", WSTOPSIG(wstatus));
            }
            else if (WIFCONTINUED(wstatus))
            {
                printf("Executing continues\n");
            }
        } while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));
    }
    return 0;
}