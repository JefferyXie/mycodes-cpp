#ifndef CONDITION_VARIABLE_H
#define CONDITION_VARIABLE_H

#include "../header.h"

// test std::condition_variable and std::unique_lock
struct CV_notify_before_unlock 
{
    std::mutex _mtx;
    std::condition_variable _cv;
    int _cargo = 0;

    void consume(int n)
    {
        for (int i = 0; i < n; ++i)
        {
            std::unique_lock<std::mutex> lk(_mtx);
            _cv.wait(lk, [&]() {
                    // this callback only executes after the mutex is released by other thread
                    if (_cargo != 0) {
                    cout << "there is cargo, should consume!" << endl;
                    return true;
                    }
                    return false;
                    });
            // start consuming...
            cout << _cargo << endl;
            _cargo = 0;
        }
    }
    void run()
    {
        int num_cargos = 5;
        std::thread consumer_thread(&CV_notify_before_unlock::consume, this, num_cargos);

        // produce cargos...
        for (int i = 0; i < num_cargos; ++i)
        {
            while (_cargo != 0)
                std::this_thread::yield();
            std::unique_lock<mutex> lk(_mtx);
            _cargo = i + 1;
            // notify is to send signal but won't change status of mutex
            _cv.notify_one();
            // the condition signal won't be received by waiting thread until mutex
            // is released
            int t = 5;
            while (t--) {
                this_thread::sleep_for(chrono::seconds(1));
                cout << "sleep after notify but before unlock: " << t << endl;
            }
        }

        consumer_thread.join();
    }
};

struct CV_notify_after_unlock
{
    std::mutex _mtx;
    std::condition_variable _cv;
    int _cargo = 0;

    void consume(int n)
    {
        for (int i = 0; i < n; ++i)
        {
            std::unique_lock<std::mutex> lk(_mtx);
            _cv.wait(lk, [&]() {
                    if (_cargo != 0) {
                    cout << "there is cargo, should consume!" << endl;
                    return true;
                    }
                    return false;
                    });
            // start consuming...
            cout << _cargo << endl;
            _cargo = 0;
        }
    }
    void run()
    {
        int num_cargos = 5;
        std::thread consumer_thread(&CV_notify_after_unlock::consume, this, num_cargos);

        // produce cargos...
        for (int i = 0; i < num_cargos; ++i)
        {
            while (_cargo != 0)
                std::this_thread::yield();
            std::unique_lock<mutex> lk(_mtx);
            _cargo = i + 1;
            // typically we should manually release mutex before notify signal
            // with this, the waiting thread will receive signal immediately
            lk.unlock();
            _cv.notify_one();
            int t = 5;
            while (t--) {
                this_thread::sleep_for(chrono::seconds(1));
                cout << "sleep after notify and unlock: " << t << endl;
            }
        }

        consumer_thread.join();
    }
};

struct CV_notify_all
{
    std::mutex _mtx;
    std::condition_variable _cv;
    int _cargo = 0;
    int _consumed = 0;

    void consume(int cargos) {
        while (1) {
            std::unique_lock<std::mutex> lk(_mtx);
            cout << "[" << this_thread::get_id() << "] before wait" << endl;
            _cv.wait(lk, [&]() {
                // this callback will be executed once immediately when this wait runs
                // after this, the callback is only called when receiving a signal
                cout << "[" << this_thread::get_id() << "] received signal" << endl;
                if (_consumed >= cargos) {
                    cout << "[" << this_thread::get_id() << "] all consumed, stopped!" << endl;
                    return true;
                }
                if (_cargo != 0) {
                    cout << "[" << this_thread::get_id() << "] there is cargo, should consume!" << endl;
                    return true;
                }
                return false;
            });
            if (_consumed++ >= cargos) {
                cout << "[" << this_thread::get_id() << "] break " << (_consumed-1) << endl;
                break;
            }
            // start consuming...
            cout << "[" << this_thread::get_id() << "] " <<  _cargo << endl;
            _cargo = 0;
        }
        cout << "[" << this_thread::get_id() << "] ends." << endl;
    }
    void run() {
        int num_cargos = 3;
        std::thread consumer_thread1(&CV_notify_all::consume, this, num_cargos);
        std::thread consumer_thread2(&CV_notify_all::consume, this, num_cargos);
        std::thread consumer_thread3(&CV_notify_all::consume, this, num_cargos);

        // produce cargos...
        for (int i = 0; i < num_cargos; ++i) {
            while (_cargo != 0)
                std::this_thread::yield();
            std::unique_lock<mutex> lk(_mtx);
            _cargo = i + 1;
            cout << "produced cargo, before unlock " << _cargo << endl;
            lk.unlock();
            // if using notify_one, only one waiting thread can receive the signal
            //_cv.notify_one();
            // if using notify_all, all waiting threads are guaranteed to receive a signal 
            // respectively when multiple threads are waiting, we should use notify_all, 
            // otherwise, some threads will never receive signal and will stuck there.
            _cv.notify_all();
        } 

        consumer_thread1.join();
        consumer_thread2.join();
        consumer_thread3.join();
    }
};

#endif


