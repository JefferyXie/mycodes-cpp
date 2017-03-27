#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "../main/header.h"

/*
 * 
 * std not yet provides semaphore, but we can implement one with mutex and condition variable
 * http://stackoverflow.com/questions/4792449/c0x-has-no-semaphores-how-to-synchronize-threads
 * 
*/
class semaphore
{
public:
    void notify() {
        std::unique_lock<decltype(mutex_)> lock(mutex_);
        ++count_;
        condition_.notify_one();
    }

    void wait() {
        std::unique_lock<decltype(mutex_)> lock(mutex_);
        condition_.wait(lock, [this]() {
            return count_ > 0; // Handle spurious wake-ups
        });
        --count_;
    }

    bool try_wait() {
        std::unique_lock<decltype(mutex_)> lock(mutex_);
        if(count_) {
            --count_;
            return true;
        }
        return false;
    }

private:
    std::mutex mutex_;
    std::condition_variable condition_;
    unsigned long count_ = 0; // Initialized as locked.
};

class semaphore_atomic
{
public:
    void notify() {
        count_.fetch_add(1, std::memory_order_release);
    }

    void wait() {
        while (true) {
            int count = count_.load(std::memory_order_relaxed);
            if (count > 0) {
                if (count_.compare_exchange_weak(count, count-1, std::memory_order_acq_rel, std::memory_order_relaxed)) {
                    break;
                }
            }
        }
    }

    bool try_wait() {
        int count = count_.load(std::memory_order_relaxed);
        if (count > 0) {
            if (count_.compare_exchange_strong(count, count-1, std::memory_order_acq_rel, std::memory_order_relaxed)) {
                return true;
            }
        }
        return false;
    }

private:
    std::atomic_int count_{0};
};

#endif

