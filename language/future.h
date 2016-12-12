#ifndef FUTURE_H
#define FUTURE_H

#include "../main/header.h"

/*
 * std::future, std::async, std::packaged_task, std::promise
 * http://stackoverflow.com/questions/17729924/when-to-use-promise-over-async-or-packaged-task
 * http://stackoverflow.com/questions/11004273/what-is-stdpromise
 * http://jakascorner.com/blog/2016/03/promise-difference.html
 * http://en.cppreference.com/w/cpp/thread/shared_future

 * std::packaged_task is just a lower level feature for implementing std::async (which is why it can do more than std::async if 
 * used together with other lower level stuff, like std::thread). Simply spoken a std::packaged_task is a std::function linked 
 * to a std::future and std::async wraps and calls a std::packaged_task (possibly in a different thread).
 * std::promise is a powerful mechanism, for example you can pass a value to new thread without need of any additional synchronizing
 * mechanism. The std::promise can explicitly set a value to a std::future anytime and not only at the end of a function call as 
 * std::async and std::packaged_task do. 
*/

void run_future()
{
    auto sleep = [](int s) { std::this_thread::sleep_for(std::chrono::seconds(s)); cout << std::this_thread::get_id() << endl; };

    auto time_consumed = [](const chrono::steady_clock::time_point& begin, const chrono::steady_clock::time_point& end) {
        std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() <<std::endl;
        // std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() <<std::endl;
    };
    
    std::chrono::steady_clock::time_point begin, end;

    cout << "----------std::async----------" << endl;
    
    cout << "f" << endl;
    begin = std::chrono::steady_clock::now();
    // std::launch::async flag will force a new thread spawned
    // std::~future will block until spawned thread finishes
    {
        auto f = std::async( std::launch::async, sleep, 2);
        
        end = std::chrono::steady_clock::now();
        // time difference == 0, nothing needs to wait
        time_consumed(begin, end);
        begin = std::chrono::steady_clock::now();
    }
    end= std::chrono::steady_clock::now();
    // time difference == 2, std::~future will block
    time_consumed(begin, end);

    cout << "f1, f2" << endl;
    begin = std::chrono::steady_clock::now();
    // std::async will block if std::future is in temporary object, saying, no explict std::future declared
    {
        //
        //auto f1 = std::async( std::launch::async, sleep, 2 );
        std::async( std::launch::async, sleep, 2 );
        
        end= std::chrono::steady_clock::now();
        // time difference == 2; if using auto f1 = std::async(...), time difference == 0
        time_consumed(begin, end);
        begin = std::chrono::steady_clock::now();
        
        //
        //auto f2 = std::async( std::launch::async, sleep, 2 );
        std::async( std::launch::async, sleep, 2 );
        
        // time difference == 2; if using auto f2 = std::async(...), time difference == 0
        end= std::chrono::steady_clock::now();
        time_consumed(begin, end);
        begin = std::chrono::steady_clock::now();
    }
    end= std::chrono::steady_clock::now();
    // time difference == 0; if using f1 and f2 declared variable, time difference == 0
    time_consumed(begin, end);
    cout << endl;

    cout << "----------std::packaged_task----------" << endl;
    
    // std::packaged_task by itself has nothing to do with threads: it is just a functor and a related future
    { 
        std::packaged_task<void(int)> package{ sleep };
        begin = std::chrono::steady_clock::now();
        std::future<void> f = package.get_future();

        cout << "directly call with packaged_task" << endl;
        // invoke the underlying function which runs in the major thread synchronically
        // has to be called before f.get(), otherwise, f(future)'s state will never be ready
        package(3);
    
        end= std::chrono::steady_clock::now();
        // time difference == 3
        time_consumed(begin, end);
        
        // f.get() won't be blocked
        f.get();
    }

    {    
        std::packaged_task<void(int)> package{ sleep };
        begin = std::chrono::steady_clock::now();
        std::future<void> f = package.get_future();
    
        cout << "running in std::thread" << endl;
        // create a thread with std::packaged_task
        std::thread t { std::move(package), 3 };
        // block here until the task/future finishes in the new thread asynchronously
        f.get();

        end= std::chrono::steady_clock::now();
        // time difference == 3
        time_consumed(begin, end);
    
        t.join();
    }

    cout << "----------std::promise----------" << endl;

    auto task = [](std::future<int> fu) {
        // hold until std::promise fulfill promise
        std::cout << fu.get() << std::endl;
    };

    std::promise<int> pr;
    // create a thread and pass future as parameter
    std::thread th {task, pr.get_future()};

    // fulfill promise
    pr.set_value(5);

    th.join();
}

#endif

