#ifndef BUILTIN_EXPECT_H
#define BUILTIN_EXPECT_H

#include <iostream>
#include <iomanip>
#include <string>

#include <ctime>
#include <limits>
#include <random>
#include <chrono>
#include <algorithm>
#include <functional>


#define likely(expr)    __builtin_expect(!!(expr), 1)
#define unlikely(expr)  __builtin_expect(!!(expr), 0)

//
volatile std::time_t near_futur = -1;
//void error_handling() { std::cerr << "error\n"; }
//bool method_impl() { return std::time(NULL) != near_futur; }
constexpr int64_t BENCHSIZE = 2000000000;
inline
void error_handling() { auto a = BENCHSIZE*2; auto b = a%125; b = 2*a + 3*b - 100; }
inline
bool method_impl(int64_t i) {
    return std::time(NULL) != near_futur;
//    auto b = std::time(NULL) != near_futur;
//    return b && ((i%1000) < 888);
}

inline
bool method_no_builtin(int64_t i)
{
    const bool res = method_impl(i);
    if (res == false) {
        error_handling();
        return false;
    }
    return true;
}

inline
bool method_builtin(int64_t i)
{
    const bool res = method_impl(i);
    //if (__builtin_expect(res, 1) == false) {
    if (unlikely(!res)) {
        error_handling();
        return false;
    }
    return true;
}    

inline
bool method_rewritten(int64_t i)
{
    const bool res = method_impl(i);
    if (res == true) {
        return true;
    } else {
        error_handling();
        return false;
    }
}

//
class Clock
{
public:
    static inline std::chrono::time_point<std::chrono::steady_clock> now()
    {
        return std::chrono::steady_clock::now();
    }

    Clock() : _start(now())
    { }

    template<class DurationUnit>
    int64_t end()
    {
        return std::chrono::duration_cast<DurationUnit>(now() - _start).count();
    }
private:
    std::chrono::time_point<std::chrono::steady_clock> _start;
};

//
void
fixed_condition()
{
    {
        Clock clock;
        bool result = true;
        for (int64_t i = 0 ; i < BENCHSIZE ; ++i)
        {
            result &= method_no_builtin(i);
            result &= method_no_builtin(i);
            result &= method_no_builtin(i);
        }
        const double unit_time = clock.end<std::chrono::nanoseconds>() / static_cast<double>(BENCHSIZE);
        std::cout << std::setw(40) << "method_no_builtin(): " << std::setprecision(3) << unit_time << " ns\n";
    }
    {
        Clock clock;
        bool result = true;
        for (int64_t i = 0 ; i < BENCHSIZE ; ++i)
        {
            result &= method_builtin(i);
            result &= method_builtin(i);
            result &= method_builtin(i);
        }
        const double unit_time = clock.end<std::chrono::nanoseconds>() / static_cast<double>(BENCHSIZE);
        std::cout << std::setw(40) << "method_builtin(): " << std::setprecision(3) << unit_time << " ns\n";
    }
    {
        Clock clock;
        bool result = true;
        for (int64_t i = 0 ; i < BENCHSIZE; ++i)
        {
            result &= method_rewritten(i);
            result &= method_rewritten(i);
            result &= method_rewritten(i);
        }
        const double unit_time = clock.end<std::chrono::nanoseconds>() / static_cast<double>(BENCHSIZE);
        std::cout << std::setw(40) << "method_rewritten(): " << std::setprecision(3) << unit_time << " ns\n";
    }
}

#endif
