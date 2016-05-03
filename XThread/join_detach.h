#include "../main/header.h"

/***************************************************************************
 * http://stackoverflow.com/questions/6202762/pthread-create-as-detached
 * The output should be
 * $ ./join_01
 * thread1 was joinable
 * thread2 is detached
 * thread3 is detached
 * *************************************************************************/

/*------------------------------- join_01.c --------------------------------*
On Linux, compile with:
cc -std=c99 -pthread join_01.c -o join_01
gcc join_01.c -o join_01 -std=c99 -lpthread (Ubuntu)

Check your system documentation how to enable C99 and POSIX threads on
other Un*x systems.
*--------------------------------------------------------------------------*/

/***************************************************************************/
/* our macro for errors checking                                           */
/***************************************************************************/
#define COND_CHECK(func, cond, retv, errv) \
if ( (cond) ) \
{ \
   fprintf(stderr, "\n[CHECK FAILED at %s:%d]\n| %s(...)=%d (%s)\n\n",\
              __FILE__,__LINE__,func,retv,strerror(errv)); \
   exit(EXIT_FAILURE); \
}

#define ErrnoCheck(func,cond,retv)  COND_CHECK(func, cond, retv, errno)
#define PthreadCheck(func,rc) COND_CHECK(func,(rc!=0), rc, rc)

/*****************************************************************************/
/* thread- dummy thread                                                      */
/*****************************************************************************/
void* thread_f(void* ignore)
{
   sleep(5);
   return NULL;
}

/*****************************************************************************/
/* detach_state. Print detachstate of a thread.                              */
/*****************************************************************************/
/*
 * We find out indirectly if a thread is detached using pthread_join().
 * If a thread is detached, then pthread_join() fails with EINVAL.
 * Otherwise the thread is joined, and hence was joinable.
 *
 */
void detach_state(
        pthread_t   tid,  // thread to check detach status
        const char *tname // thread name
        )
{
   int rc; // return code

   rc = pthread_join(tid, NULL);
   if (rc == EINVAL)
   {
      printf("%s is detached\n", tname);
   }
   else if (rc == 0)
   {
      printf("%s was joinable\n", tname);
   }
   else
   {
      printf("%s: pthread_join() = %d (%s)\n",
             tname, rc, strerror(rc)
            );
   }
}

/*****************************************************************************/
/* main- main thread                                                         */
/*****************************************************************************/
void Run_join_detach()
{
   pthread_t tid1, tid2, tid3; // thread 1,2 and 3.
   int rc;  // return code

   /*--------------------------------------------------------*/
   /* 1st test: normal thread creation                       */
   /*--------------------------------------------------------*/
   rc = pthread_create(&tid1, NULL, thread_f, NULL);
   PthreadCheck("pthread_create", rc);
   detach_state(tid1, "thread1"); // expect: joinable

   /*--------------------------------------------------------*/
   /* 2nd test: detach thread from main thread               */
   /*--------------------------------------------------------*/
   rc = pthread_create(&tid2, NULL, thread_f, NULL);
   PthreadCheck("pthread_create", rc);
   rc = pthread_detach(tid2);
   PthreadCheck("pthread_detach", rc);
   detach_state(tid2, "thread2"); // expect: detached

   /*--------------------------------------------------------*/
   /* 3rd test: create detached thread                       */
   /*--------------------------------------------------------*/

   // set detachstate attribute to DETACHED
   //
   pthread_attr_t attr;
   rc = pthread_attr_init(&attr);
   PthreadCheck("pthread_attr_init", rc);
   rc = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
   PthreadCheck("pthread_attr_setdetachstate", rc);

   // create thread now
   //
   rc = pthread_create(&tid3, &attr, thread_f, NULL);
   PthreadCheck("pthread_create", rc);
   detach_state(tid3, "thread3");

   //pthread_exit(NULL);
}



/****************************************************************
 * The other example
 * *************************************************************/
// http://stackoverflow.com/questions/6042970/pthread-detach-question
// To quote the Linux Programmer's Manual:
//  The detached attribute merely determines the behavior of the system when
//  the thread terminates; it does not prevent the thread from being terminated
//  if the process terminates using exit(3) (or equivalently, if the main thread returns).
// Also from the Linux Programmer's Manual:
//  To allow other threads to continue execution, the main thread should terminate
//  by calling pthread_exit() rather than exit(3).

void *func(void *data)
{
    while (1)
    {
        printf("Speaking from the detached thread...\n");
        sleep(5);
    }
    pthread_exit(NULL);
}

void Run_detach()
{
    pthread_t handle;
    if (!pthread_create(&handle, NULL, func, NULL))
    {
        printf("Thread create successfully !!!\n");
        if (!pthread_detach(handle))
            printf("Thread detached successfully !!!\n");
    }

    //sleep(10);
    pthread_exit(NULL);
    printf("Main thread dying...\n");
}

