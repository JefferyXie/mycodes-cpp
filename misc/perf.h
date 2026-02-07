#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

void run_perf_test()
{
    constexpr int CACHE_LINE         = 64;
    constexpr int LENGTH             = 512 * 1024 * 1024;
    constexpr int NUM_PER_CACHE_LINE = CACHE_LINE / sizeof(int);
    constexpr int NUM_ITEMS          = LENGTH / NUM_PER_CACHE_LINE;

    std::vector<int> arr(LENGTH);
    const auto       start = std::chrono::high_resolution_clock::now();

    // access NUM_ITEMS/16 cache lines
    /*
        for (int i = 0; i < NUM_ITEMS; i++)
            arr[i]++;
    */
    /*
        // access NUM_ITEMS cache lines: much higher cost than above
        for (int i = 0; i < NUM_ITEMS*NUM_PER_CACHE_LINE; i+=NUM_PER_CACHE_LINE)
            arr[i]++;
    */
    // access NUM_ITEMS cache lines, twice of index access: similar cost as above
    for (int i = 0; i < NUM_ITEMS * NUM_PER_CACHE_LINE; i += NUM_PER_CACHE_LINE) {
        arr[i]++;
        arr[i + NUM_PER_CACHE_LINE - 1]++;
    }

    const auto stop = std::chrono::high_resolution_clock::now();
    const auto dura = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::cout << (float)dura.count() << std::endl;
}

void run_thread_atom_perf_test()
{
    using int_t               = int32_t;
    using atom_int_t          = std::atomic<int_t>;
    constexpr auto CAPPED_NUM = std::numeric_limits<int_t>::max();
    constexpr auto NUM_THREAD = 10;

    auto do_test = []() {
        int_t                    cnt = 0;
        std::vector<std::thread> threads;
        for (int i = 0; i < NUM_THREAD; i++) {
            threads.emplace_back([&cnt]() {
                int_t prev = 0, tmp = 0;
                while (prev < CAPPED_NUM) {
                    tmp = cnt;

                    // If it is not safe, tmp will not increase all the time.
                    if (tmp < prev) {
                        std::cout << "Error cnt declined" << std::endl;
                    }

                    // if (tmp % 1000000 == 0) cout << tmp << endl;
                    prev = tmp;
                }
            });
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));

        auto start = std::chrono::high_resolution_clock::now();

        while (cnt < CAPPED_NUM) {
            ++cnt;
        }
        for (auto& t : threads) {
            t.join();
        }

        auto stop = std::chrono::high_resolution_clock::now();
        auto dura = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        std::cout << (float)dura.count() / 1000 << std::endl;
    };

    do_test();

    auto do_atom = []() {
        atom_int_t               cnt = 0;
        std::vector<std::thread> threads;
        for (int i = 0; i < NUM_THREAD; i++) {
            threads.emplace_back([&cnt]() {
                int_t prev = 0, tmp = 0;
                while (prev < CAPPED_NUM) {
                    tmp = cnt.load(std::memory_order_relaxed);

                    // If it is not safe, tmp will not increase all the time.
                    if (tmp < prev) {
                        std::cout << "Error cnt declined" << std::endl;
                    }

                    // if (tmp % 1000000 == 0) cout << tmp << endl;
                    prev = tmp;
                }
            });
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));

        auto start = std::chrono::high_resolution_clock::now();

        int_t v = 0;
        while (v < CAPPED_NUM) {
            cnt.store(++v, std::memory_order_relaxed);
        }
        for (auto& t : threads) {
            t.join();
        }

        auto stop = std::chrono::high_resolution_clock::now();
        auto dura = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        std::cout << (float)dura.count() / 1000 << std::endl;
    };
    do_atom();
}

