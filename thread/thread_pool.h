#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include "../main/header.h"

class thread_pool
{
public:
    thread_pool(int num_threads)
    {
        _stopped = false;
        while (num_threads--)
        {
            _threads.emplace_back(&thread_pool::thread_func, this);
        }
    };

    ~thread_pool()
    {
        cleanup();
    }

    void thread_func()
    {
        while (!_stopped.load(std::memory_order_acquire)) {
            unique_lock<mutex> job_lock(_job_mutex);
            _jobs_available.wait(job_lock,
                                 [this]() {
                                    return !_jobs.empty();
                                 });

            if (_stopped.load(std::memory_order_acquire)) break;

            auto job = move(_jobs.front());
            _jobs.pop_front();
            job_lock.unlock();

            job();
        }
    }

    template<typename FU>
    std::future<FU>
    addjob(std::packaged_task<FU()> job)
    {
        auto job_ptr = make_shared<std::packaged_task<FU()>>(move(job));
        unique_lock<mutex> job_lock(_job_mutex);
        _jobs.push_back([job_ptr]() {
                            job_ptr->operator()();
                        });
        job_lock.unlock();
        _jobs_available.notify_one();
        return job_ptr->get_future();
    }

    void cleanup()
    {
        unique_lock<mutex> job_lock(_job_mutex);
        _stopped = true;
        _jobs.clear();
        job_lock.unlock();
        _jobs_available.notify_all();

        for (auto& thread : _threads) {
            if (thread.joinable())
                thread.join();
        }
    }

private:
    std::mutex              _job_mutex;
    std::condition_variable _jobs_available;

    std::atomic<bool>                 _stopped;
    std::vector<std::thread>          _threads;
    std::deque<std::function<void()>> _jobs;
};

#define JOBS 5000

float CalcInvSqr(float x)
{
    return (x);
}

void run_thread_pool()
{
    thread_pool pool(40);
    std::vector<std::future<float>> results;
    for (int i = 0; i < JOBS; i++)
    {
        float num = (float)(rand()%RAND_MAX)/(float)RAND_MAX;
        std::packaged_task<float()> task([num]() {
                                            return CalcInvSqr(num);
                                         });
        results.push_back(pool.addjob<float>(move(task)));
    }

    for (int i = 0; i < JOBS; i++)
    {
        cout << "VALUE: " << results[i].get() << endl;
    }

    cout << "Jobs Done ! " << endl;
}

#endif

