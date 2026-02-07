#include <array>
#include <condition_variable>
#include <deque>
#include <iostream>
#include <mutex>
#include <thread>

namespace producer_consumer {

struct storage_t {
    explicit storage_t(size_t capacity) : capacity_{capacity} {}
    bool block_push(int m)
    {
        size_t pre_storage_size = 0;
        {
            std::unique_lock<std::mutex> locker{mutex_};
            if (storage_.size() >= capacity_) {
                if (has_consumer_) {
                    std::cout << "" << __FUNCTION__ << ": wait until storage has room" << std::endl;
                    cond_.wait(locker, [this]() {
                        return storage_.size() < capacity_ || !has_consumer_;
                    });
                }

                if (storage_.size() >= capacity_ && !has_consumer_) {
                    std::cout << "\t" << __FUNCTION__ << ": storage is full" << std::endl;
                    return false;
                }
            }

            pre_storage_size = storage_.size();

            std::cout << __FUNCTION__ << ": value " << m << ", size=" << pre_storage_size << std::endl;

            storage_.emplace_back(m);
        }

        if (pre_storage_size == 0) {
            cond_.notify_all();
        }
        return true;
    }
    bool block_pop(int& m)
    {
        size_t pre_storage_size = 0;
        {
            std::unique_lock<std::mutex> locker{mutex_};
            if (storage_.empty()) {
                if (has_producer_) {
                    std::cout << "\t" << __FUNCTION__ << ": wait until storage is ready" << std::endl;
                    cond_.wait(locker, [this]() {
                        return !storage_.empty() || !has_producer_;
                    });
                }

                if (storage_.empty() && !has_producer_) {
                    std::cout << "\t" << __FUNCTION__ << ": storage is empty" << std::endl;
                    return false;
                }
            }

            pre_storage_size = storage_.size();

            m = std::move(storage_.front());
            storage_.pop_front();

            std::cout << "\t" << __FUNCTION__ << ": value " << m << ", size=" << pre_storage_size << std::endl;
        }

        if (pre_storage_size == capacity_) {
            cond_.notify_all();
        }
        return true;
    }

    void turn_on_producer()
    {
        {
            std::lock_guard<std::mutex> locker{mutex_};
            has_producer_ = true;
        }
        cond_.notify_all();
    }
    void turn_off_producer()
    {
        {
            std::lock_guard<std::mutex> locker{mutex_};
            has_producer_ = false;
        }
        cond_.notify_all();
    }
    void turn_on_consumer()
    {
        {
            std::lock_guard<std::mutex> locker{mutex_};
            has_consumer_ = true;
        }
        cond_.notify_all();
    }
    void turn_off_consumer()
    {
        {
            std::lock_guard<std::mutex> locker{mutex_};
            has_consumer_ = false;
        }
        cond_.notify_all();
    }

    bool                    has_producer_ = false;
    bool                    has_consumer_ = false;
    size_t                  capacity_     = 0;
    std::mutex              mutex_;
    std::condition_variable cond_;
    std::deque<int>         storage_;
};

// support multiple writers one reader
// TODO: need more test
template <size_t Capacity>
struct atomic_storage_t {
    static_assert((Capacity & (Capacity - 1)) == 0, "Capacity must be power of 2");

    bool block_push(int m)
    {
        // update idx_write_ here in order to keep safty for N writers
        size_t idx_pending_write = 0;
        while (true) {
            auto idx_read     = idx_read_.load();
            idx_pending_write = idx_write_.load();
            if (idx_pending_write - idx_read >= capacity_) {
                std::cout << "\t" << __FUNCTION__ << ": storage is full" << std::endl;
                return false;
            }
            if (idx_write_.compare_exchange_weak(idx_pending_write, idx_pending_write + 1)) {
                break;
            }
            __asm__("pause");
        }

        std::cout << __FUNCTION__ << ": value " << m << ", idx_pending_write=" << idx_pending_write << std::endl;

        const auto pos          = idx_pending_write & (capacity_ - 1);
        auto&      reserved_obj = storage_[pos];
        reserved_obj.object     = std::move(m);
        // index within object is to guarantee reader safty, this is very important
        reserved_obj.index.store(idx_pending_write);

        return true;
    }
    bool block_pop(int& m)
    {
        auto       idx_pending_read = idx_read_.load();
        const auto pos              = idx_pending_read & (capacity_ - 1);
        auto&      loaded_obj       = storage_[pos];
        if (loaded_obj.index.load() != idx_pending_read) {
            std::cout << "\t" << __FUNCTION__ << ": storage is empty" << std::endl;
            return false;
        }
        m = std::move(loaded_obj.object);

        std::cout << __FUNCTION__ << ": value " << m << ", idx_pending_read=" << idx_pending_read << std::endl;

        // don't update idx_read_ until finish reading, also this means we can only support 1 reader!
        idx_read_.store(idx_pending_read + 1);

        return true;
    }

    template <typename T>
    struct internal_obj_t {
        std::atomic<size_t> index = std::numeric_limits<size_t>::max();
        T                   object;
    };

    static constexpr size_t                   capacity_  = Capacity;
    std::atomic<size_t>                       idx_read_  = 0;    // the next read index
    std::atomic<size_t>                       idx_write_ = 0;    // the next write index
    std::array<internal_obj_t<int>, Capacity> storage_;
};

template <typename Storage = storage_t>
struct producer_t {
    explicit producer_t(Storage& storage) : storage_{&storage} {}

    bool produce(int value) { return storage_->block_push(value); }

    Storage* storage_ = nullptr;
};

template <typename Storage = storage_t>
struct consumer_t {
    explicit consumer_t(Storage& storage) : storage_{&storage} {}

    bool consume()
    {
        int m = 0;
        return storage_->block_pop(m);
    }

    Storage* storage_ = nullptr;
};

void run()
{
    storage_t storage{5};

    std::thread th_producer{[&]() {
        producer_t producer{storage};
        storage.turn_on_producer();
        int value = 0;
        while (value++ < 30) {
            producer.produce(value);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        storage.turn_off_producer();
    }};

    std::thread th_consumer{[&]() {
        consumer_t consumer{storage};
        storage.turn_on_consumer();
        int times = 0;
        while (times++ < 20) {
            consumer.consume();
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
        storage.turn_off_consumer();
    }};

    th_producer.join();
    th_consumer.join();
}

void run_atomic()
{
    const size_t        times_produce   = 30;
    const size_t        times_consume   = 20;
    size_t              success_produce = 0;
    size_t              success_consume = 0;
    atomic_storage_t<4> storage;

    std::thread th_producer{[&]() {
        producer_t producer{storage};
        int        times = times_produce;
        while (times--) {
            if (producer.produce(success_produce)) {
                ++success_produce;
            }
            // std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }};

    std::thread th_consumer{[&]() {
        consumer_t consumer{storage};
        int        times = times_consume;
        while (times--) {
            if (consumer.consume()) {
                ++success_consume;
            }
            // std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }};

    th_producer.join();
    th_consumer.join();

    std::cout << "Done: times_produce=" << times_produce << ", times_consume=" << times_consume
              << ", success_produce=" << success_produce << ", success_consume=" << success_consume << std::endl;
}

};    // namespace producer_consumer

