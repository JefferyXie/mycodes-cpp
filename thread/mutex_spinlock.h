/*******************************************************************************

// mutex vs. spinlock:
// Spinlock keeps checking the lock (busy waiting), while mutex puts
// threads waiting for the lock into sleep (blocked). A busy-waiting
// thread wastes CPU cycles, while a blocked thread does not. Regardless
// of this difference, mutex and spinlock have very similar behavior.

// implementation of spinlock
volatile int lock = 0;
void *worker(void*)
{
  while (__sync_lock_test_and_set(&lock, 1)) while (lock);
  // critical section
  __sync_lock_release(&lock);
}

// try pthread_spin_lock

// mutex source code analysis: http://blog.chinaunix.net/uid-20791902-id-2169057.html

// critical section vs. mutex:
// 1), cs is done in user mode without involving the kernel
// 2), cs is not a kernel object so it works only within single process
// 3), cs is faster as it doesn't require the system call into kernel mode
// 4), mutex is more CPU intensive and slower due to a larger amount of bookkeeping
//     and the deep execution path into the kernel taken to acquire a mutex
// 5), cs remains in thread context, and consists of merely checking and
//     incrementing/decrementing lock-count related data.

mutex: can multi-process in linux share same mutex?

// memory ordering
// https://msdn.microsoft.com/en-us/library/ms686355.aspx
// Processors can support instructions for memory barriers with acquire, release,
// and fence semantics. These semantics describe the order in which results of an
// operation become available. With acquire semantics, the results of the operation
// are available before the results of any operation that appears after it in code.
// With release semantics, the results of the operation are available after the results
// of any operation that appears before it in code. Fence semantics combine acquire and
// release semantics. The results of an operation with fence semantics are available before
// those of any operation that appears after it in code and after those of any operation
// that appears before it.

// semaphore vs. mutex
// A mutex is essentially the same thing as a binary semaphore and sometimes uses the
// same basic implementation. The differences between them are in how they are used.
// While a binary semaphore may be used as a mutex, a mutex is a more specific use-case,
// which allows extra guarantees:
// 1), Mutexes have a concept of an owner. Only the process that locked the mutex is supposed
//     to unlock it. If the owner is stored by the mutex this can be verified at runtime.
// 2), Mutexes may provide priority inversion safety. If the mutex knows its current owner,
//     it is possible to promote the priority of the owner whenever a higher-priority task
//     starts waiting on the mutex.
// 3), Mutexes may also provide deletion safety, where the process holding the mutex cannot
//     be accidentally deleted.

// c++ mutexes:
// 1), mutex
// 2), recrusive_mutex
// 3), timed_mutex
// 4), recursive_timed_mutex

*******************************************************************************/


//
// http://www.alexonlinux.com/pthread-mutex-vs-pthread-spinlock
//
#include "../main/header.h"
#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#include <sys/time.h>

#define LOOPS 10000000

std::list<int> the_list;

#ifdef USE_SPINLOCK
pthread_spinlock_t spinlock;
#else
pthread_mutex_t mutex_p;
#endif

//pid_t gettid() { return syscall( __NR_gettid ); }

pid_t gettid() {
    uint64_t tid;
    pthread_threadid_np(NULL, &tid);
    return tid;
}

void *consumer(void *ptr)
{
    printf("Consumer TID %lu\n", (unsigned long)gettid());

    while (1)
    {
#ifdef USE_SPINLOCK
        pthread_spin_lock(&spinlock);
#else
        pthread_mutex_lock(&mutex_p);
#endif

        if (the_list.empty())
        {
#ifdef USE_SPINLOCK
            pthread_spin_unlock(&spinlock);
#else
            pthread_mutex_unlock(&mutex_p);
#endif
            break;
        }

        [[maybe_unused]] auto i = the_list.front();
        the_list.pop_front();

#ifdef USE_SPINLOCK
        pthread_spin_unlock(&spinlock);
#else
        pthread_mutex_unlock(&mutex_p);
#endif
    }

    return NULL;
}

void Run_mutex_spinlock()
{
    pthread_t thr1, thr2;
    struct timeval tv1, tv2;

#ifdef USE_SPINLOCK
    pthread_spin_init(&spinlock, 0);
    printf("[Use spinlock]\n");
#else
    pthread_mutex_init(&mutex_p, NULL);
    printf("[Use mutex]\n");
#endif

    // Creating the list content...
    for (int i = 0; i < LOOPS; i++)
        the_list.push_back(i);

    // Measuring time before starting the threads...
    gettimeofday(&tv1, NULL);

    pthread_create(&thr1, NULL, consumer, NULL);
    pthread_create(&thr2, NULL, consumer, NULL);

    pthread_join(thr1, NULL);
    pthread_join(thr2, NULL);

    // Measuring time after threads finished...
    gettimeofday(&tv2, NULL);

    if (tv1.tv_usec > tv2.tv_usec)
    {
        tv2.tv_sec--;
        tv2.tv_usec += 1000000;
    }

    printf("Total time elapsed (seconds): %ld.%ld\n",
           tv2.tv_sec - tv1.tv_sec, tv2.tv_usec - tv1.tv_usec);

#ifdef USE_SPINLOCK
    pthread_spin_destroy(&spinlock);
#else
    pthread_mutex_destroy(&mutex_p);
#endif
}

