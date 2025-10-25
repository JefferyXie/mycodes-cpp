#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

// head files for socket
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#include <strings.h>
#include <unistd.h>
#include <arpa/inet.h>

// head files for posix thread
#include <pthread.h>
#include <semaphore.h>

// head files for std
#include <ios>
#include <iostream>
#include <limits>
#include <fstream>
#include <sstream>
#include <set>
#include <stack>
#include <string>
#include <array>
#include <vector>
#include <list>
#include <queue>
#include <iterator>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <algorithm>
#include <typeinfo>
#include <assert.h>
#include <utility>
#include <cstring>
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <type_traits>
#include <random>
#include <functional>
#include <iomanip>
#include <cmath>
#include <tuple>
#include <regex>
#include <ctime>
#include <chrono>
#include <ratio>
#include <thread>
#include <atomic>
#include <mutex>
#include <future>
#include <condition_variable>
#include <climits>
#include <filesystem>

#include <experimental/type_traits>

// boost start
#include <boost/functional/hash.hpp>
#include <boost/format.hpp>
#include <boost/math/special_functions/prime.hpp>
// boost end

// __GNUC__ start
#ifdef __GNUC__

#ifndef likely
#define likely(expr) __builtin_expect(!!(expr), 1)
#endif

#ifndef unlikely
#define unlikely(expr) __builtin_expect(!!(expr), 0)
#endif

#else

#ifndef likely
#define likely(x) x
#endif

#ifndef unlikely
#define unlikely(x) x
#endif

#endif
// __GNUC__ end

#define FORCE_INLINE __attribute__((always_inline)) inline
#define NO_INLINE __attribute__((noinline))
#define HOT_PATH __attribute__((hot))
#define COLD_PATH __attribute__((cold))

#ifdef __APPLE__
#include <mach/error.h>
#include <mach/mach_interface.h>
#include <mach/mach_types.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/sysctl.h>

// https://www.hybridkernel.com/2015/01/18/binding_threads_to_cores_osx.html
#define SYSCTL_CORE_COUNT "machdep.cpu.core_count"

typedef struct cpu_set {
    uint32_t count;
} cpu_set_t;

static inline void CPU_ZERO(cpu_set_t* cs)
{
    cs->count = 0;
}

static inline void CPU_SET(int num, cpu_set_t* cs)
{
    cs->count |= (1 << num);
}

static inline int CPU_ISSET(int num, cpu_set_t* cs)
{
    return (cs->count & (1 << num));
}

int sched_getaffinity(pid_t pid, size_t cpu_size, cpu_set_t* cpu_set)
{
    int32_t core_count = 0;
    size_t  len        = sizeof(core_count);
    int     ret        = sysctlbyname(SYSCTL_CORE_COUNT, &core_count, &len, 0, 0);
    if (ret) {
        printf("error while get core count %d\n", ret);
        return -1;
    }
    cpu_set->count = 0;
    for (int i = 0; i < core_count; i++) {
        cpu_set->count |= (1 << i);
    }

    return 0;
}

int pthread_setaffinity_np(pthread_t thread, size_t cpu_size, cpu_set_t* cpu_set)
{
    thread_port_t mach_thread;
    size_t        core = 0;

    for (core = 0; core < 8 * cpu_size; core++) {
        if (CPU_ISSET(core, cpu_set))
            break;
    }
    printf("binding to core %ld\n", core);
    thread_affinity_policy_data_t policy = {static_cast<int>(core)};
    mach_thread                          = pthread_mach_thread_np(thread);
    thread_policy_set(mach_thread, THREAD_AFFINITY_POLICY, (thread_policy_t)&policy, 1);
    return 0;
}
#else
#include <error.h>
#endif

