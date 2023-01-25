#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
int fac(int n)
{
    int ans = 1;
    for (int i = 2; i <= n; i++)
    {
        ans = ans * i;
    }
    return ans;
}

int main(int argc, char *argv[])
{
    for (int i = 1; i < argc; i++)
    {
        int data = atoi(argv[i]);
        pid_t childProcess = fork();
        if (childProcess > 0)
        {
            wait(NULL);
        }
        else if (childProcess == 0)
        {
            printf("Factorial of %d -------> %d\n", data, fac(data));
            return 0;
        }
    }

    return 0;
}
