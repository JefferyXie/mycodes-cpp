#ifndef LOCK_READER_WRITER_H
#define LOCK_READER_WRITER_H

#include "../main/header.h"
#include "thread_id.h"

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
    struct scoped_reader_lock_t {
        lock_reader_writer& _rw;
        scoped_reader_lock_t() = delete;
        scoped_reader_lock_t(const scoped_reader_lock_t&) = delete;
        // mu must have been locked
        scoped_reader_lock_t(lock_reader_writer& rw) : _rw(rw) {
            _rw.read_lock();
        }
        ~scoped_reader_lock_t() {
            _rw.read_unlock();
        }
    };

    // 
    // it's not encouraged to explictly call below lock/unlock interface
    // instead, use SCOPED_RW_READ & SCOPED_RW_WRITE

    void read_lock() {
        std::unique_lock<std::mutex> lock_read(_mu_read);
        if (_readers++ == 0)
        {
            cout << "\n\t[" << gettid() << "] will lock write" << endl;
            _mu_write.lock();
            _mu_write_owner_thread = gettid();
        }
    }
    void read_unlock() {
        std::unique_lock<std::mutex> lock_read(_mu_read);
        if (gettid() == _mu_write_owner_thread) {
            if (_readers == 1) {
                --_readers;
                _mu_write_owner_thread = 0;
                _mu_write.unlock();
            } else if (_readers > 1) {
                // only the owner thread can unlock
                // should wait until notified all other readers done
                _cv.wait(lock_read, [this]() {
                    return _readers == 1;
                });

                cout << "\n\t[" << gettid() << "] will unlock write" << endl;

                --_readers;
                _mu_write_owner_thread = 0;
                _mu_write.unlock();
            } else {
                assert("Wrong _readers!");
            }
        } else {
            if (--_readers == 1) {
                _cv.notify_one();
            }
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

    // the old implementation of read_lock() & read_unlock() is wrong
    // same mutext's unlock should be done by the thread who locked it
    // otherwise the behavior is undefined per c++ standard
    void WRONG_read_lock() {
        std::unique_lock<std::mutex> lock_read(_mu_read);
        if (_readers++ == 0)
        {
            cout << "\n\t[" << gettid() << "] will lock write" << endl;
            _mu_write.lock();
        }
    }
    void WRONG_read_unlock() {
        std::unique_lock<std::mutex> lock_read(_mu_read);
        if (--_readers == 0) {
            cout << "\n\t[" << gettid() << "] will unlock write" << endl;
            _mu_write.unlock();
        }
    }

    std::mutex& get_mu_write() { return _mu_write; }

private:
    int         _readers = 0;
    unsigned    _mu_write_owner_thread = 0;
    std::condition_variable _cv;
    std::mutex	_mu_read;
    std::mutex	_mu_write;
};

// TODO: is there a better way to define the macro??
#define SCOPED_RW_READ(rw) lock_reader_writer::scoped_reader_lock_t __scoped_r_##rw(rw)
#define SCOPED_RW_WRITE(rw) std::lock_guard<std::mutex> __scoped_w_##rw(rw.get_mu_write())

lock_reader_writer s_locker;

void* reader(void*);
void* writer(void*);
void run_lock_reader_writer()
{  
    int a = 0, b = 0;
    int a1 = 1, b1 = 1;
    int a2 = 2, b2 = 2;
    int a3 = 3, b3 = 3;
    int a4 = 4, b4 = 4;
    system("clear");
    pthread_t r,w,r1,w1,r2,w2,r3,w3,r4,w4;
    pthread_create(&r,NULL,reader,(void*)&a);
    pthread_create(&w1,NULL,writer,(void*)&b);
    pthread_create(&r1,NULL,reader,(void*)&a1);
    pthread_create(&w,NULL,writer,(void*)&b1);
    pthread_create(&r2,NULL,reader,(void*)&a2);
    pthread_create(&w2,NULL,writer,(void*)&b2);
    pthread_create(&r3,NULL,reader,(void*)&a3);
    pthread_create(&w3,NULL,writer,(void*)&b3);
    pthread_create(&r4,NULL,reader,(void*)&a4);
    pthread_create(&w4,NULL,writer,(void*)&b4);

    pthread_join(r,NULL);
    pthread_join(w1,NULL);
    pthread_join(r1,NULL);
    pthread_join(w,NULL);
    pthread_join(w2,NULL);
    pthread_join(r2,NULL);
    pthread_join(w3,NULL);
    pthread_join(r3,NULL);
    pthread_join(w4,NULL);
    pthread_join(r4,NULL);

    printf("\n\nrun_lock_reader_writer ends.\n");
}
void* reader(void* arg)  
{
    int c = *static_cast<int*>(arg);
    printf("\n[R]reader %d is created",c); sleep(1);

//    s_locker.read_lock();
    SCOPED_RW_READ(s_locker);

    /*Critcal Section */
    printf("\n\n[R]reader %d is reading ", c);
    sleep(1);
    printf("\n[R]reader%d finished reading",c);
    /* critical section completd */

//    s_locker.read_unlock();

    return 0;
}
void* writer(void* arg)
{
    int c = *static_cast<int*>(arg);
    printf("\n[W]writer %d is created", c);
    sleep(1);

//    s_locker.write_lock();
    SCOPED_RW_WRITE(s_locker);

    printf("\n[W]writer %d is writing", c);
    sleep(1);
    printf("\n[W]writer%d finished writing", c);

//    s_locker.write_unlock();

    return 0;
}

#endif
