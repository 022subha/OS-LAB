#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>

int shmid;

void row(void *shm_pointer, int **A, int **B, int row, int m, int p)
{
    for (int i = 0; i < p; i++)
    {
        int sum = 0;
        for (int j = 0; j < m; j++)
        {
            sum += A[row][j] * B[j][i];
        }
        *((int *)shm_pointer + row * m + i) = sum; // row major order
    }
}

void display(void *shm_pointer, int n, int m, int p)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < p; j++)
        {
            printf("%d ", *((int *)shm_pointer + i * m + j)); // row major order
        }
        printf("\n");
    }
}
int main()
{
    int n, m, p;
    pid_t childPid[n];

    printf("Enter the value of number of rows and columns for matrix A respectively\n");
    scanf("%d%d", &n, &m);
    printf("Enter the value of number columns for matrix B\n");
    scanf("%d", &p);

    int **A = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++)
    {
        A[i] = (int *)malloc(m * sizeof(int *));
        for (int j = 0; j < m; j++)
        {
            printf("Enter the element (%d,%d) of matrix A\n", i + 1, j + 1);
            scanf("%d", &A[i][j]);
        }
    }

    int **B = (int **)malloc(m * sizeof(int *));
    for (int i = 0; i < m; i++)
    {
        B[i] = (int *)malloc(p * sizeof(int *));
        for (int j = 0; j < p; j++)
        {
            printf("Enter the element (%d,%d) of matrix B\n", i + 1, j + 1);
            scanf("%d", &B[i][j]);
        }
    }

    shmid = shmget(IPC_PRIVATE, n * p * sizeof(int), IPC_CREAT | 0777);

    if (shmid == -1)
    {
        printf("Error in creating shared memory\n");
        exit(EXIT_FAILURE);
    }

    int index = 0;
    for (int i = 0; i < n; i++)
    {

        pid_t pid = fork();
        childPid[index++] = pid;
        if (pid == -1)
        {
            printf("Error in fork\n");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            void *shm_pointer = shmat(shmid, NULL, 0);
            printf("Child process %d created\n", i + 1);
            row(shm_pointer, A, B, i, m, p);
            exit(EXIT_SUCCESS);
        }
    }

    // wait for all child processes to finish
    for (int i = 0; i < index; i++)
    {
        int wstatus;
        do
        {
            // wait for child process to finish
            pid_t w = waitpid(childPid[i], &wstatus, 0);

            if (w == -1)
            {
                exit(EXIT_FAILURE);
            }
            printf("pid of child==>%d\n", childPid[i]);

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

    printf("---------------------Result Matrix-----------------\n");
    display(shmat(shmid, NULL, 0), n, m, p);
    return 0;
}