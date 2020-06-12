#ifndef CIRCULAR_BUF_ASYNC_H
#define CIRCULAR_BUF_ASYNC_H

#include "../main/header.h"

// 
// TODO: NOT YET FINISHED...
// 
// Adanced circular buffer
// 
// 1) threading-safe, support 1 consumer (get), 1+ producers (free)
// 2) get is lock free as long as the free buffer meets the request
// 3) free typically happens in producer thread which won't block consumer
// 4) typical use case: consumer is highly time sensitive compare to producer
// 
class circular_buf_async
{
public:
    circular_buf_async(uint32_t capacity)
    {
        head_ = new char[capacity];
        capacity_ = capacity;

        lockfreeHead_ = head_;
        lockfreeCapacity_ = capacity_;
        lockfreePos_ = 0;

        lockHead_ = head_;
        lockCapacity_ = 0;
        lockPos_ = 0;
    }

    char* get(uint32_t len)
    {
        // TODO: border case - it's close to tail but buffer in the tail is not
        // enough, has to bypass the tail and start from head_...
        if (lockfreePos_ == lockfreeCapacity_ ||
            lockfreeCapacity_ - lockfreePos_ < len)
        {
            std::lock_guard<std::mutex> lg(mutex_);
            lockfreeHead_ += lockfreePos_;
            lockfreeCapacity_ += lockCapacity_ - lockfreePos_;
            lockfreePos_ = 0;

            // TODO: lockHead_ cannot be always increase, it should go back to
            // head_ again and again...
            lockHead_ += lockCapacity_;
            lockCapacity_ = 0;
            lockPos_ = 0;
        }

        if (lockfreeCapacity_ - lockfreePos_ < len)
        {
            // WRONG: not enough buffer available
            return nullptr;
        }

        auto buf = lockfreeHead_ + lockfreePos_;
        lockfreePos_ += len;
        return buf;
    }

    void free(char* t, uint32_t len)
    {
        std::lock_guard<std::mutex> lg(mutex_);
        // TODO: border case ...
        if (lockHead_ + lockCapacity_ == t)
        {
            lockCapacity_ += len;
        }
        else
        {
            // WRONG: t is not at the continuous position ....
        }

        // TODO: border case ...
        if (lockHead_ + lockCapacity_ > head_ + capacity_)
        {
            // WRONG: released buffer overflow...
        }
    }

protected:
    char* head_;
    uint32_t capacity_;

    char* lockfreeHead_;
    uint32_t lockfreePos_;
    uint32_t lockfreeCapacity_;

    char* lockHead_;

    uint32_t lockPos_;
    uint32_t lockCapacity_;

    std::mutex mutex_;
};

#endif
