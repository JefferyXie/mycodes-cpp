#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// head files for socket
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

// head files for posix thread
#include <pthread.h>
#include <semaphore.h>

// head files for std
#include <algorithm>
#include <array>
#include <assert.h>
#include <atomic>
#include <chrono>
#include <climits>
#include <cmath>
#include <concepts>
#include <condition_variable>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <functional>
#include <future>
#include <iomanip>
#include <ios>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <random>
#include <ratio>
#include <regex>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <thread>
#include <tuple>
#include <type_traits>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include <experimental/type_traits>

// boost start
#include <boost/format.hpp>
#include <boost/functional/hash.hpp>
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
#include <sys/sysctl.h>
#include <sys/types.h>

// https://www.hybridkernel.com/2015/01/18/binding_threads_to_cores_osx.html
#define SYSCTL_CORE_COUNT "machdep.cpu.core_count"

typedef struct cpu_set {
    uint32_t count;
} cpu_set_t;

inline void CPU_ZERO(cpu_set_t* cs)
{
    cs->count = 0;
}

inline void CPU_SET(int num, cpu_set_t* cs)
{
    cs->count |= (1 << num);
}

inline int CPU_ISSET(int num, cpu_set_t* cs)
{
    return (cs->count & (1 << num));
}

inline int sched_getaffinity([[maybe_unused]] pid_t pid, [[maybe_unused]] size_t cpu_size, cpu_set_t* cpu_set)
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

inline int pthread_setaffinity_np(pthread_t thread, size_t cpu_size, cpu_set_t* cpu_set)
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

