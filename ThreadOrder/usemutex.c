// 使用互斥变量mutex

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t  g_mutex;
int count = 0;
int num_count = 0;
void *thread_fun1(void* arg)
{	
	while(1)
	{
		pthread_mutex_lock(&g_mutex);
		if(count % 3 == 0 && num_count < 10)
		{
			//pthread_mutex_lock(&g_mutex);
			count++;
			printf("thread1's id is %d\n",pthread_self()); 
			//pthread_mutex_unlock(&g_mutex);
		}
		pthread_mutex_unlock(&g_mutex);
		if(num_count == 10) break;
	}
	return (void*) 0;
}

void* thread_fun2(void* arg)
{
	while(1)
	{
		pthread_mutex_lock(&g_mutex);
		if(count %3 == 1 && num_count < 10)
		{
			//pthread_mutex_lock(&g_mutex);
			count++;
			printf("thread2's id is %d\n",pthread_self()); 
			//pthread_mutex_unlock(&g_mutex);
		}
		pthread_mutex_unlock(&g_mutex);
		if(num_count == 10)  break;
	}
	return (void*) 0;
}

void* thread_fun3(void* arg)
{	
	while(1)
	{
		pthread_mutex_lock(&g_mutex);		
		if(count %3 == 2)
		{
			//pthread_mutex_lock(&g_mutex);		
			count++;
			printf("thread3's id is %d\n\n", pthread_self());
			num_count++;
			//pthread_mutex_unlock(&g_mutex);
		}
		pthread_mutex_unlock(&g_mutex);
		if(num_count == 10) break;
		
	}
	return (void*) 0;
}

int main()
{
	pthread_t thread1, thread2, thread3;
	int res = 0;
	pthread_mutex_init(&g_mutex, NULL);
	
	res = pthread_create(&thread1, NULL, thread_fun1, NULL);
	if(res)
	{
		printf("trhead1 created failed\n");
	}
	res = pthread_create(&thread2, NULL, thread_fun2, NULL);
	if(res)
	{
		printf("thread2 created faield\n");
	}
	res = pthread_create(&thread3, NULL, thread_fun3, NULL);
	if(res)
	{
		printf("thread3 cerated failed\n");
	}

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	pthread_join(thread3, NULL);		
}


