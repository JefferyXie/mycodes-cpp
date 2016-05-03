#ifndef USESEMAPHORE_C
#define USESEMAPHORE_C

// 使用信号量semaphore

#include "../main/header.h"

int m_count = 0;

struct th_Arg {
	sem_t* s_wait;
	sem_t* s_post;
	char* message;
	bool bIncreaseCount;
	th_Arg(sem_t* s_w, sem_t* s_p, char* msg, bool increaseFlag) {
		s_wait = s_w;
		s_post = s_p;
		message = msg;
		bIncreaseCount = increaseFlag;
	}
};

void *thread_fun(void* arg)
{
	th_Arg* pThArg = (th_Arg*)arg;
	while (m_count < 10) {
		sem_wait(pThArg->s_wait);
		if (m_count < 10) {
			printf("%s's id is %d\n", pThArg->message, pthread_self());
			if (pThArg->bIncreaseCount) {
				m_count++;
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
	int res = sem_init(&sem_th1, 0, 0);
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
	if(res)
	{
		printf("thread2 created fialed\n");
	}
	res = pthread_create(&thread3, NULL, thread_fun, &arg3);
	if(res)
	{
		printf("thread3 created failed\n");
	}	
	res = pthread_create(&thread4, NULL, thread_fun, &arg4);
	if(res)
	{
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

#endif

