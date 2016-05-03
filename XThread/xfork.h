#include "../main/header.h"

void Run_fork()
{
    int time = rand();
    int pid = fork();
    if (pid == 0)
    {
        printf("process %d is sleeping for %d\n", pid, time);
        sleep(time);
        printf("process %d done!\n", pid);
    }
    else if (pid > 0)
    {
        printf("process %d is sleeping for %d\n", pid, time);
    }
    else
    {
        perror("fork");
    }
}
