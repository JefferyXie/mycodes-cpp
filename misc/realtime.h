#ifndef MAYBE_H
#define MAYBE_H

#include "../main/header.h"
#include "../main/utility.h"
#include "../misc/builtin_expect.h"

#include <time.h>
#include <sched.h>

/*
 *
 * https://aufather.wordpress.com/2010/09/08/high-performance-time-measuremen-in-linux/
 * https://stackoverflow.com/questions/8602336/getting-cpu-cycles-using-rdtsc-why-does-the-value-of-rdtsc-always-increase/8605960
 * https://stackoverflow.com/questions/10921210/cpu-tsc-fetch-operation-especially-in-multicore-multi-processor-environment/
 *
 * */

const uint64_t NANO_SECONDS_IN_SEC = 1000000000LL;

struct realtime {

    // assembly code to read the TSC
    static uint64_t rdtsc()
    {
        unsigned int hi, lo;
        __asm__ volatile("rdtsc" : "=a" (lo), "=d" (hi));
        return ((uint64_t)hi << 32) | lo;
    }

    static double ticks_per_ns() {
        return instance().ticks_per_ns_;
    }

    // returns a struct timespec with the time difference (end - begin)
    static timespec timespec_diff(const timespec& begin, const timespec& end)
    {
        struct timespec ts;
        ts.tv_sec = end.tv_sec - begin.tv_sec;
        ts.tv_nsec = end.tv_nsec - begin.tv_nsec;
        if (ts.tv_nsec < 0) {
            ts.tv_sec--;
            ts.tv_nsec += NANO_SECONDS_IN_SEC;
        }
        return ts;
    }

    static timespec timespec_diff(uint64_t ns_begin, uint64_t ns_end)
    {
        struct timespec ts;
        ts.tv_sec = (ns_end - ns_begin) / NANO_SECONDS_IN_SEC;
        ts.tv_nsec = (ns_end - ns_begin) % NANO_SECONDS_IN_SEC;
        return ts;
    }

    static timespec ns_to_timespec(uint64_t nsecs)
    {
        struct timespec ts;
        ts.tv_sec = nsecs / NANO_SECONDS_IN_SEC;
        ts.tv_nsec = nsecs % NANO_SECONDS_IN_SEC;
        return ts;
    }

    /* ts will be filled with time converted from TSC reading */
    static uint64_t get_ns_since_epoch_from_rdtsc()
    {
        return rdtsc() / ticks_per_ns();
    }

    static uint64_t get_ns_since_epoch_from_clocktime()
    {
        struct timespec now = get_clock_ts();
        return now.tv_sec * NANO_SECONDS_IN_SEC + now.tv_nsec;
    }

    static timespec get_clock_ts()
    {
        struct timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);
        return now;
    }

private:
    static realtime& instance() {
        static realtime __instance;
        return __instance;
    }

    realtime() {
        init_rdtsc();
//        std::cout << "realtime::ticks_per_ns_=" << ticks_per_ns_ << std::endl;
    }

    // call once before using rdtsc, has side effect of binding process to CPU1
    void init_rdtsc() {
#ifdef __linux__ 
        cpu_set_t cpu_mask;
        // bind to cpu 1
        CPU_SET(2, &cpu_mask);
        int err = sched_setaffinity(0, sizeof(cpu_mask), &cpu_mask);
        if (err) {
            std::cout << "sched_setaffinity failed: "
                << strerror(err) << std::endl;
        } else {
            std::cout << "sched_setaffinity success." << std::endl;
        }
#else
        std::cout << "sched_setaffinity not available." << std::endl;
#endif
        ticks_per_ns_ = get_calibrate_ticks();
    }

    double ticks_per_ns_ = 0;
    double get_calibrate_ticks() {
        struct timespec begints, endts;
        clock_gettime(CLOCK_MONOTONIC, &begints);
        uint64_t begin = rdtsc();
        for (uint64_t i = 0; i < 1000000; i++) {} // must be CPU intensive
        uint64_t end = rdtsc();
        clock_gettime(CLOCK_MONOTONIC, &endts);
        struct timespec tsdiff = timespec_diff(begints, endts);
        uint64_t nsecElapsed = tsdiff.tv_sec * NANO_SECONDS_IN_SEC + tsdiff.tv_nsec;
        return (double)(end - begin)/(double)nsecElapsed;
    }

};

void run_realtime()
{
    std::cout << "ticks_per_ns=" << realtime::ticks_per_ns() << std::endl;
    std::cout << "rdtsc=" << realtime::rdtsc() << std::endl;
    auto ns_rdtsc = realtime::get_ns_since_epoch_from_rdtsc();
    std::cout << "get_ns_since_epoch_from_rdtsc=" << ns_rdtsc << std::endl;
    std::cout << "ns_to_timespec:"
        << realtime::ns_to_timespec(ns_rdtsc).tv_sec << ", "
        << realtime::ns_to_timespec(ns_rdtsc).tv_nsec
        << std::endl;
    auto ns_clock = realtime::get_ns_since_epoch_from_clocktime();
    std::cout << "get_ns_since_epoch_from_clocktime=" << ns_clock << std::endl;
    std::cout << "ns_to_timespec:"
        << realtime::ns_to_timespec(ns_clock).tv_sec << ", "
        << realtime::ns_to_timespec(ns_clock).tv_nsec
        << std::endl;
    
    std::cout << "\n-------------------------\n" << std::endl;
    run_builtin_expect(); // fixed_condition()
    std::cout << "\n-------------------------\n" << std::endl;

    auto nss_rdtsc = realtime::get_ns_since_epoch_from_rdtsc();
    std::cout << "get_ns_since_epoch_from_rdtsc=" << nss_rdtsc << std::endl;
    std::cout << "ns_to_timespec:"
        << realtime::ns_to_timespec(nss_rdtsc).tv_sec << ", "
        << realtime::ns_to_timespec(nss_rdtsc).tv_nsec
        << std::endl;
    auto nss_clock = realtime::get_ns_since_epoch_from_clocktime();
    std::cout << "get_ns_since_epoch_from_clocktime=" << nss_clock << std::endl;
    std::cout << "ns_to_timespec:"
        << realtime::ns_to_timespec(nss_clock).tv_sec << ", "
        << realtime::ns_to_timespec(nss_clock).tv_nsec
        << std::endl;
    
    auto ts_diff_rdtsc = realtime::timespec_diff(ns_rdtsc, nss_rdtsc);
    auto ts_diff_clock = realtime::timespec_diff(ns_clock, nss_clock);
    std::cout << "ts_diff_rdtsc=" << ts_diff_rdtsc.tv_sec << ", "
              << ts_diff_rdtsc.tv_nsec << std::endl;
    std::cout << "ts_diff_clock=" << ts_diff_clock.tv_sec << ", "
              << ts_diff_clock.tv_nsec << std::endl;
}

#endif

