#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#define SHM_SIZE 1024
int main()
{
    key_t key = ftok("shmfile", 65);

    int shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1)
    {
        perror("shmget");
        exit(1);
    }
    char *shmaddr = (char *)shmat(shmid, NULL, 0);
    if (shmaddr == (char *)-1)
    {
        perror("shmat");
        exit(1);
    }
    pid_t pid = fork();

    if (pid < 0)
    {
        printf("Child process failed");
    }
    else if (pid == 0)
    {
        printf("Reading from the shared memory\n");
        sleep(2);
        printf("Message %s\n", shmaddr);
    }
    else
    {
        printf("Parent process writing  message to shared memory\n");
        strcpy(shmaddr, "Hello data in shared memory");
        wait(NULL);
    }

    shmdt(shmaddr);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}