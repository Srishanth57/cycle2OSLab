#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    pid_t pid = fork();

    if (pid < 0)
    {
        fprintf(stderr, "Fork Failed");
        return 1;
    }
    else if (pid == 0)
    { // Child process
        execlp("/bin/ls", "ls", NULL);
    }
    else
    { // Parent process
        wait(NULL);
        printf("Child executed");
    }
    return 0;
}
