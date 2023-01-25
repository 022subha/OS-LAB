#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <time.h>

const char *filename = "./factorial.txt";
int shmid;

long long factorial(int num)
{
    long long res = 1;
    for (int i = 2; i <= num; i++)
    {
        res *= i;
    }
    return res;
}

int main()
{
    shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0777);
    int *shm_pointer = shmat(shmid, NULL, 0);
    if (shmid == -1)
    {
        printf("Error in making Shared Memory\n");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == -1)
    {
        printf("Error in fork\n");
        exit(EXIT_FAILURE);
    }
    if (pid == 0)
    {
        if (shm_pointer == (int *)-1)
        {
            printf("Error in attaching shared memory for child process\n");
            exit(EXIT_FAILURE);
        }

        for (int i = 1; i < 15; i++)
        {
            printf("Factorial of %d = %lld\n", *shm_pointer, factorial(*shm_pointer));
            getchar();
        }
        exit(EXIT_SUCCESS);
    }
    else if (pid > 0)
    {
        int *shm_pointer = shmat(shmid, NULL, 0);
        for (int i = 1; i < 15; i++)
        {
            srand(time(0));
            *shm_pointer = rand() % 10;
            printf("Shared Memory updated to %d\n", *shm_pointer);
            getchar();
        }
    }

    shmdt(shm_pointer);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
