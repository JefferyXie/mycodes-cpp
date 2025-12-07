#pragma once

#include <cmath>
namespace util_number {

constexpr inline bool greater_than(double bigger, double smaller, double precision)
{
    return bigger > smaller + precision;
}

constexpr inline bool greater_than(double bigger, double smaller)
{
    return greater_than(bigger, smaller, 0.0);
}

constexpr inline bool less_than(double smaller, double bigger, double precision)
{
    return smaller < bigger - precision;
}

constexpr inline bool less_than(double smaller, double bigger)
{
    return less_than(smaller, bigger, 0.0);
}

constexpr inline bool almost_equal(double a, double b, double precision)
{
    return !greater_than(a, b, precision) && !less_than(a, b, precision);
}

constexpr inline bool almost_equal(double a, double b)
{
    return !greater_than(a, b) && !less_than(a, b);
}

// round to nearest multiple of factor
constexpr inline double round_to_nearest_factor(double x, double factor)
{
    if (!std::isfinite(x)) {
        return x;
    }
    return x < 0.0 ? ((static_cast<double>(static_cast<int64_t>((x / factor) - 0.5))) * factor)
                   : ((static_cast<double>(static_cast<int64_t>((x / factor) + 0.5))) * factor);
}

constexpr inline double round_up_to_nearest_factor(double x, double factor)
{
    if (!std::isfinite(x)) {
        return x;
    }
    return ((x > 0) + static_cast<int64_t>((x - 1.0e-10) / factor)) * factor;
}

constexpr inline double round_down_to_nearest_factor(double x, double factor)
{
    if (!std::isfinite(x)) {
        return x;
    }
    return (-(x < 0) + static_cast<int64_t>((x + 1.0e-10) / factor)) * factor;
}

}    // namespace util_number
