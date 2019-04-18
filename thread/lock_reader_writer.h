#ifndef LOCK_READER_WRITER_H
#define LOCK_READER_WRITER_H

#include "../main/header.h"

//
/*
 * This implementation has quite a few drawbacks, though.

 1) No fairness. Readers starve writers - writers won't execute unless all
    readers finish (readers == 0)

 You can fix that by tracking the number of pending read and write locks, and
 either stop acquiring read locks once there a pending write locks (though you
 will then starve readers!), or randomly waking up either all readers or one
 writer (assuming you use separate condition variable, see section above).

 2) Locks aren't dealt out in the order they are requested

 To guarantee this, you'll need a real wait queue. You could e.g. create one
 condition variable for each waiter, and signal all readers or a single writer,
 both at the head of the queue, after releasing the lock.

 **/

class lock_reader_writer
{
public:
    void read_lock()
    {
        std::unique_lock<std::mutex> lock_read(_mu_read);
        if (readers++ == 0)
        {
            _mu_write.lock();
        }
    }
    void read_unlock()
    {
        std::unique_lock<std::mutex> lock_read(_mu_read);
        if (--readers == 0)
        {
            _mu_write.unlock();
        }
    }
    void write_lock()
    {
        _mu_write.lock();
    }
    void write_unlock()
    {
        _mu_write.unlock();
    }

private:
    int         readers = 0;
    std::mutex	_mu_read;
    std::mutex	_mu_write;
};

static lock_reader_writer s_locker;
void* reader(void*);
void* writer(void*);
void run_lock_reader_writer()
{  
    int a = 1, b = 1;
    int a1 = 2, b1 = 2;
    system("clear");
    pthread_t r,w,r1,w1;
    pthread_create(&r,NULL,reader,(void*)&a);
    pthread_create(&w1,NULL,writer,(void*)&b);
    pthread_create(&r1,NULL,reader,(void*)&a1);
    pthread_create(&w,NULL,writer,(void*)&b1);
    pthread_join(r,NULL);
    pthread_join(w1,NULL);
    pthread_join(r1,NULL);
    pthread_join(w,NULL);
    printf("run_lock_reader_writer ends.\n");
    return 0;
}
void* reader(void* arg)  
{
    int c = *static_cast<int*>(arg);
    printf("\nreader %d is created",c); sleep(1);

    s_locker.read_lock();

    /*Critcal Section */
    printf("\n\nreader %d is reading ", c);
    sleep(1);
    printf("\nreader%d finished reading",c);
    /* critical section completd */

    s_locker.read_unlock();

    return 0;
}
void* writer(void* arg)
{
    int c = *static_cast<int*>(arg);
    printf("\nwriter %d is created", c);
    sleep(1);

    s_locker.write_lock();

    printf("\nwriter %d is writing", c);
    sleep(1);
    printf("\nwriter%d finished writing", c);

    s_locker.write_unlock();

    return 0;
}

#endif

