#pragma once

#include "../core/header.h"

//
// http://blog.csdn.net/mafuli007/article/details/8488534
// 线程按顺序执行（迅雷笔试题）
// 编写一个程序，开启3个线程，这3个线程的ID分别为A、B、C，每个线程将自己的ID在屏幕上打印10遍，要求输出结果必须按ABC的顺序显示；如：ABCABC….依次递推。

//
// solution 1): use mutex

pthread_mutex_t g_mutex;
static int      n_count   = 0;
static int      num_count = 0;
void*           thread_fun1(void* arg)
{
    while (1) {
        pthread_mutex_lock(&g_mutex);
        if (n_count % 3 == 0 && num_count < 10) {
            // pthread_mutex_lock(&g_mutex);
            n_count++;
            printf("thread1's id is %lu\n", pthread_self());
            // pthread_mutex_unlock(&g_mutex);
        }
        pthread_mutex_unlock(&g_mutex);
        if (num_count == 10)
            break;
    }
    return (void*)0;
}

void* thread_fun2(void* arg)
{
    while (1) {
        pthread_mutex_lock(&g_mutex);
        if (n_count % 3 == 1 && num_count < 10) {
            // pthread_mutex_lock(&g_mutex);
            n_count++;
            printf("thread2's id is %lu\n", pthread_self());
            // pthread_mutex_unlock(&g_mutex);
        }
        pthread_mutex_unlock(&g_mutex);
        if (num_count == 10)
            break;
    }
    return (void*)0;
}

void* thread_fun3(void* arg)
{
    while (1) {
        pthread_mutex_lock(&g_mutex);
        if (n_count % 3 == 2) {
            // pthread_mutex_lock(&g_mutex);
            n_count++;
            printf("thread3's id is %lu\n\n", pthread_self());
            num_count++;
            // pthread_mutex_unlock(&g_mutex);
        }
        pthread_mutex_unlock(&g_mutex);
        if (num_count == 10)
            break;
    }
    return (void*)0;
}

void UseMutex()
{
    pthread_t thread1, thread2, thread3;
    int       res = 0;
    pthread_mutex_init(&g_mutex, NULL);

    res = pthread_create(&thread1, NULL, thread_fun1, NULL);
    if (res) {
        printf("trhead1 created failed\n");
    }
    res = pthread_create(&thread2, NULL, thread_fun2, NULL);
    if (res) {
        printf("thread2 created faield\n");
    }
    res = pthread_create(&thread3, NULL, thread_fun3, NULL);
    if (res) {
        printf("thread3 cerated failed\n");
    }

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
}

//
// solution 2): use semaphore

static int thread_count = 0;
struct th_Arg {
    sem_t*      s_wait;
    sem_t*      s_post;
    std::string message;
    bool        bIncreaseCount;
    th_Arg(sem_t* s_w, sem_t* s_p, const char* msg, bool increaseFlag)
    {
        s_wait         = s_w;
        s_post         = s_p;
        message        = msg;
        bIncreaseCount = increaseFlag;
    }
};

void* thread_fun(void* arg)
{
    th_Arg* pThArg = (th_Arg*)arg;
    while (thread_count < 10) {
        sem_wait(pThArg->s_wait);
        if (thread_count < 10) {
            printf("%s's id is %lu\n", pThArg->message.c_str(), pthread_self());
            if (pThArg->bIncreaseCount) {
                thread_count++;
                printf("\n");
            }
        }
        sem_post(pThArg->s_post);
    }
}

void UseSemaphore()
{
    sem_t sem_th1;
    sem_t sem_th2;
    sem_t sem_th3;
    sem_t sem_th4;
    int   res = sem_init(&sem_th1, 0, 0);
    if (res) {
        return;
    }
    res = sem_init(&sem_th2, 0, 0);
    if (res) {
        return;
    }
    res = sem_init(&sem_th3, 0, 0);
    if (res) {
        return;
    }
    res = sem_init(&sem_th4, 0, 0);
    if (res) {
        printf("Semaphore initilization failed\n");
        return;
    }

    th_Arg arg1(&sem_th1, &sem_th2, "thread 1", false);
    th_Arg arg2(&sem_th2, &sem_th3, "thread 2", false);
    th_Arg arg3(&sem_th3, &sem_th4, "thread 3", false);
    th_Arg arg4(&sem_th4, &sem_th1, "thread 4", true);

    pthread_t thread1;
    pthread_t thread2;
    pthread_t thread3;
    pthread_t thread4;
    res = pthread_create(&thread1, NULL, thread_fun, &arg1);
    if (res) {
        printf("Thread1 created failed\n");
    }
    res = pthread_create(&thread2, NULL, thread_fun, &arg2);
    if (res) {
        printf("thread2 created fialed\n");
    }
    res = pthread_create(&thread3, NULL, thread_fun, &arg3);
    if (res) {
        printf("thread3 created failed\n");
    }
    res = pthread_create(&thread4, NULL, thread_fun, &arg4);
    if (res) {
        printf("thread4 created failed\n");
    }

    sem_post(&sem_th1);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_join(thread4, NULL);

    printf("All threads finished!\n");

    sem_destroy(&sem_th1);
    sem_destroy(&sem_th2);
    sem_destroy(&sem_th3);
    sem_destroy(&sem_th4);
}

