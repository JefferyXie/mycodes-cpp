#include "../header.h"

// Marathon程序: http://ifeve.com/mutex-and-memory-visibility/
bool arrived = false; // SHOULD use volatile
float miles = 0.0;  // SHOULD use volatile

void* run(void* i)
{
    while (!arrived) { }
    printf("[%d] run ends with miles: %f\n", *(int*)i, miles);

    //pthread_exit(NULL);

    return NULL;
}
void* complete(void* i)
{
    miles = 26.385; // 42.195 km
    arrived = true;

    //pthread_exit(NULL);
    return NULL;
}

void Run_memoryorder()
{
    for (int i = 0; i < 10000000; ++i)
    {
        pthread_t th1, th2;
        pthread_create(&th1, NULL, run, (void*)&i);
        pthread_create(&th2, NULL, complete, NULL);

        pthread_join(th1, NULL);
        pthread_join(th2, NULL);
    }

    //pthread_exit(NULL);
}
