#pragma once

#include "header.h"

namespace math {

inline bool is_prime(size_t n)
{
    if (n <= 1 || n % 2 == 0)
        return false;

    auto lim = static_cast<size_t>(std::sqrt(static_cast<double>(n))) + 1;
    for (size_t d = 3; d <= lim; d += 2) {
        if (n % d == 0)
            return false;
    }
    return true;
}

inline size_t next_prime(size_t n)
{
    while (!is_prime(n)) {
        ++n;
    }
    return n;
}

constexpr bool compile_time_is_prime(uint64_t N)
{
    if (N <= 2)
        return false;
    else {
        // std::sqrt is not constexpr unfortunately
        // size_t lim = std::sqrt(N) + 1;
        for (uint64_t d = 2; d * d <= N; ++d) {
            if (N % d == 0)
                return false;
        }
    }
    return true;
}
constexpr uint32_t compile_time_count_primes(uint64_t N)
{
    uint32_t count = 0;
    for (uint64_t n = 0; n < N; ++n) {
        if (compile_time_is_prime(n)) {
            ++count;
        }
    }
    return count;
}

};    // namespace math

