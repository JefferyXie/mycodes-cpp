#pragma once

#include "../core/header.h"

template <typename T>
T interpolate(const std::vector<int64_t>& sample_times, const std::vector<T>& sample_values, int64_t time_point)
{
    const auto sample_count = sample_times.size();
    if (sample_count == 0 || sample_count != sample_values.size()) {
        return T();
    }

    const auto& front_time = sample_times.front();
    const auto& back_time  = sample_times.back();
    if (front_time == time_point) {
        return sample_values.front();
    } else if (back_time == time_point) {
        return sample_values.back();
    } else if (sample_count < 2) {
        return T();
    }

    int idx_left  = 0;
    int idx_right = 0;
    if (front_time > time_point) {
        idx_left  = 0;
        idx_right = 1;
    } else if (back_time < time_point) {
        idx_left  = sample_count - 2;
        idx_right = sample_count - 1;
    } else {
        idx_left  = 0;
        idx_right = sample_count - 1;
        while (idx_right > idx_left + 1) {
            // for log(n) time complexity, do binary search the index position
            const auto  idx_mid = (idx_left + idx_right) / 2;
            const auto& mid     = sample_times[idx_mid];
            if (mid > time_point) {
                idx_right = idx_mid;
            } else if (mid < time_point) {
                idx_left = idx_mid;
            } else {
                return sample_values[idx_mid];
            }
        }
    }

    // y = y0 + (x - x0) * (y1 - y0) / (x1 - x0)
    const auto& y0 = sample_values[idx_left];
    const auto& y1 = sample_values[idx_right];
    const auto& x0 = sample_times[idx_left];
    const auto& x1 = sample_times[idx_right];

    std::cout << "time_point=" << time_point << ", idx=[" << idx_left << ", " << idx_right << "]"
              << ", y0=" << y0 << ", y1=" << y1 << ", x0=" << x0 << ", x1=" << x1 << std::endl;

    return y0 + (time_point - x0) * (y1 - y0) / (x1 - x0);
}

