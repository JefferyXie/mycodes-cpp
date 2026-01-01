#pragma once

#include "header.h"

//
// generic event used in thread pool
struct event_t {
    unsigned index = 0;
    event_t(unsigned idx) : index(idx) {}
    virtual ~event_t() = default;
    virtual void run() = 0;
};
template <class CalcF, class CallbackF, class... Args>
struct event_impl_t : public event_t {
    std::tuple<Args...> paras;
    CalcF               f_calc;
    CallbackF           f_cb;

    event_impl_t(unsigned idx, CalcF& calc, CallbackF& cb, const std::tuple<Args...>& args)
        : event_t(idx),
          paras(args),
          f_calc(std::forward<CalcF>(calc)),
          f_cb(std::forward<CallbackF>(cb))
    {
    }

    template <std::size_t... Is>
    auto run_calc(std::index_sequence<Is...>)
    {
        return f_calc(std::get<Is>(paras)...);
    }

    void run() override
    {
        auto result = run_calc(std::index_sequence_for<Args...>{});
        f_cb(index, std::move(result));
    }
};
// special event used as final event when calculation is done
struct event_all_done_t : public event_t {
    std::function<void()> f_notify_all_done;
    event_all_done_t(std::function<void()>&& cb) : event_t(0), f_notify_all_done(std::move(cb)) {}
    void run() override { f_notify_all_done(); }
};

//
// generic thread pool
struct thread_pool {
    // TODO: below 3 can be merged into 1 status variable
    bool stopped               = false;
    bool waiting_all_done      = false;
    bool events_all_done_added = false;

    std::mutex              mu;
    std::condition_variable cv;

    std::queue<event_t*>     events;
    std::vector<std::thread> threads;

    thread_pool() {}
    ~thread_pool() { stop(); }
    // to start running thread pool
    size_t start()
    {
        stopped             = false;
        auto concurrent_num = std::thread::hardware_concurrency() + 2;
        for (unsigned i = 0; i < concurrent_num; ++i) {
            threads.emplace_back(do_work, this, i);
        }
        return concurrent_num;
    }

    // to stop running thread pool
    void stop()
    {
        std::unique_lock<std::mutex> locker(mu);
        stopped = true;
        while (!events.empty()) {
            events.pop();
        }
        locker.unlock();
        for (auto& th : threads) {
            th.join();
        }
        threads.clear();
    }

    // add event to the queue, event is allocated by caller
    size_t push(event_t* event)
    {
        std::unique_lock<std::mutex> locker(mu);
        waiting_all_done = false;
        events.push(event);
        return events.size();
    }

    // wait until all events are handled
    void wait_until_all_done()
    {
        std::unique_lock<std::mutex> locker(mu);
        waiting_all_done = true;
        while (!events.empty()) {
            cv.wait(locker, [this]() {
                return events.empty();
            });
            break;
        }
    }

    // thread proc function
    static void do_work(thread_pool* pool, unsigned th_idx)
    {
        event_t* event = nullptr;
        while (1) {
            {
                std::unique_lock<std::mutex> locker(pool->mu);
                if (pool->events.empty()) {
                    if (pool->waiting_all_done && !pool->events_all_done_added) {
                        pool->events.push(new event_all_done_t{[pool]() {
                            std::unique_lock<std::mutex> lk(pool->mu);
                            pool->waiting_all_done      = false;
                            pool->events_all_done_added = false;
                            pool->cv.notify_one();
                        }});
                        pool->events_all_done_added = true;
                    }
                    if (pool->stopped)
                        break;
                    else
                        continue;
                }
                event = pool->events.front();
                pool->events.pop();
            }
            event->run();
        }
    }
};

// generic helper callback when a calc is finished
template <class T>
void callback(std::vector<T>& results, unsigned idx, T result)
{
    if (results.size() <= idx)
        results.resize(idx + 1);
    results[idx] = result;
}

// generic helper function that makes event_impl_t instance
template <class CalcF, class CallbackF, class... Args>
event_impl_t<CalcF, CallbackF, Args...>
make_event(unsigned idx, CalcF&& calc, CallbackF&& cb, const std::tuple<Args...>& args)
{
    return event_impl_t<CalcF, CallbackF, Args...>(idx, std::forward<CalcF>(calc), std::forward<CallbackF>(cb), args);
}

