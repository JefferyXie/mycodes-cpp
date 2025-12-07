#pragma once

#include "../core/header.h"

class thread_pool
{
public:
    thread_pool(int num_threads)
    {
        _stopped = false;
        while (num_threads--) {
            _threads.emplace_back(&thread_pool::thread_func, this);
        }
    };

    ~thread_pool() { cleanup(); }

    void thread_func()
    {
        // this is the only place that requires _stopped be atomic
        while (!_stopped.load(std::memory_order_acquire)) {
            std::unique_lock<std::mutex> job_lock(_job_mutex);
            _jobs_available.wait(job_lock, [this]() {
                // memory_order_relaxed is enough since mutex is used inside cleanup()
                return !_jobs.empty() || _stopped.load(std::memory_order_relaxed);
            });

            // memory_order_relaxed is enough since mutex is used inside cleanup()
            if (_stopped.load(std::memory_order_relaxed))
                break;

            auto job = std::move(_jobs.front());
            _jobs.pop_front();
            job_lock.unlock();

            job();
        }
    }

    template <typename FU>
    std::future<FU> addjob(std::packaged_task<FU()> job)
    {
        auto                         job_ptr = std::make_shared<std::packaged_task<FU()>>(std::move(job));
        std::unique_lock<std::mutex> job_lock(_job_mutex);
        _jobs.push_back([job_ptr]() {
            job_ptr->operator()();
        });
        job_lock.unlock();
        _jobs_available.notify_one();
        return job_ptr->get_future();
    }

    void cleanup()
    {
        std::unique_lock<std::mutex> job_lock(_job_mutex);
        _stopped = true;
        _jobs.clear();
        job_lock.unlock();
        _jobs_available.notify_all();

        for (auto& th : _threads) {
            if (th.joinable()) {
                th.join();
            }
        }
    }

private:
    std::mutex              _job_mutex;
    std::condition_variable _jobs_available;

    std::atomic<bool>                 _stopped;
    std::vector<std::thread>          _threads;
    std::deque<std::function<void()>> _jobs;
};

std::pair<int, float> CalcInvSqr(int idx, float x)
{
    // cout << std::this_thread::get_id() << ": " << x << endl;
    return {idx, x};
}

void run_thread_pool()
{
    constexpr int JOBS    = 5000;
    constexpr int THREADS = 100;

    thread_pool                                     pool(THREADS);
    std::vector<std::future<std::pair<int, float>>> results;
    for (int i = 0; i < JOBS; i++) {
        const auto                                  num = (float)(rand() % RAND_MAX) / (float)RAND_MAX;
        std::packaged_task<std::pair<int, float>()> task([i, num]() {
            return CalcInvSqr(i, num);
        });
        results.push_back(pool.addjob<std::pair<int, float>>(std::move(task)));
    }

    // use std::future below can ensure all jobs are done, and dump in original order
    for (int i = 0; i < JOBS; i++) {
        auto&& result = results[i].get();
        std::cout << "Job " << i << ", {idx=" << result.first << ", value=" << result.second << "}" << std::endl;
    }
    std::cout << "Jobs Done ! " << std::endl;
}

