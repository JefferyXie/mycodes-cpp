#pragma once

#include "../core/header.h"

int   g_sum = 0;
void* runner(void* param)
{
    int upper = *(int*)param;    // (int)*param;
    for (int i = 1; i < upper; ++i)
        g_sum += i;
    pthread_exit(0);
}

void run_pthread(int num)
{
    pthread_t      tid;     // thread id
    pthread_attr_t attr;    // thread attributes

    // get default attributes
    pthread_attr_init(&attr);

    // create thread
    pthread_create(&tid, NULL, runner, &num);

    // wait for the thread to exit
    pthread_join(tid, NULL);

    printf("g_sum = %d\n", g_sum);
}

void run_fork()
{
    int time = rand();
    int pid  = fork();
    if (pid == 0) {
        printf("process %d is sleeping for %d\n", pid, time);
        sleep(time);
        printf("process %d done!\n", pid);
    } else if (pid > 0) {
        printf("process %d is sleeping for %d\n", pid, time);
    } else {
        perror("fork");
    }
}
