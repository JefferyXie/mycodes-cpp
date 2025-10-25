#include <gtest/gtest.h>

#include "../main/header.h"
#include "../main/math.h"

TEST(utility, math_prime)
{
    static_assert(!math::compile_time_is_prime(0));
    static_assert(!math::compile_time_is_prime(1));
    static_assert(!math::compile_time_is_prime(2));
    static_assert(math::compile_time_is_prime(3));
    static_assert(!math::compile_time_is_prime(4));
    static_assert(math::compile_time_is_prime(5));
    static_assert(!math::compile_time_is_prime(6));

    static_assert(math::compile_time_count_primes(0) == 0);
    static_assert(math::compile_time_count_primes(5) == 1);
    static_assert(math::compile_time_count_primes(10) == 3);
    static_assert(math::compile_time_count_primes(20) == 7);
    static_assert(math::compile_time_count_primes(30) == 9);
}

