#pragma once

#include "../core/header.h"

static int                  g_num = 0;
static std::mutex           g_num_mutex;
static std::recursive_mutex g_num_recursive_mutex;

class Recurisve_Mutex
{
public:
    static void print(int id, int num) { std::cout << id << " => " << num << std::endl; }

    static void print_recursive(int id, int num)
    {
        g_num_recursive_mutex.lock();
        std::cout << id << " => " << num << std::endl;
        g_num_recursive_mutex.unlock();
    }

    static void increment(int id)
    {
        for (int i = 0; i < 3; ++i) {
            g_num_mutex.lock();

            print(id, g_num);

            g_num_mutex.unlock();

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    static void increment_recursive(int id)
    {
        for (int i = 0; i < 3; ++i) {
            g_num_recursive_mutex.lock();

            print_recursive(id, g_num);

            g_num_recursive_mutex.unlock();

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    void run()
    {
        std::thread t0(increment, 0);
        std::thread t1(increment, 1);

        std::thread t2(increment_recursive, 2);
        std::thread t3(increment_recursive, 3);

        t0.join();
        t1.join();
        t2.join();
        t3.join();
    }
};

