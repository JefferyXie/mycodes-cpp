#ifndef ARRAY_BUS_STATION_H
#define ARRAY_BUS_STATION_H

#include "../main/header.h"

//
// https://www.hackerrank.com/challenges/bus-station
//

std::vector<int> array_bus_station(std::vector<int> arr)
{
    int lower = 0;
    int sum   = 0;
    // 1) get cumulative sum
    std::unordered_set<int> cum_sum;
    for (auto v : arr) {
        lower = std::max(v, lower);
        sum += v;
        cum_sum.insert(sum);
    }
    std::set<int> factors;
    int           fct = 0;
    // 2) get all factors of sum so that sum % factor == 0
    // https://www.rookieslab.com/posts/most-efficient-way-to-find-all-factors-of-a-number-python-cpp
    while (++fct) {
        if (fct * fct > sum)
            break;

        if (sum % fct)
            continue;

        if (fct >= lower)
            factors.insert(fct);
        if (sum / fct >= lower)
            factors.insert(sum / fct);
    }

    // 3) check each factor with cumulative sum
    std::vector<int> rets;
    for (auto factor : factors) {
        int cum_bus_cap = factor;
        while (true) {
            if (cum_sum.count(cum_bus_cap) == 0)
                break;

            if (cum_bus_cap == sum) {
                rets.push_back(factor);
                break;
            }
            cum_bus_cap += factor;
        }
    }
    return rets;
}

std::vector<int> array_bus_station_v2(std::vector<int> arr)
{
    std::vector<int> result;
    int              cum_count = 0;
    for (size_t i = 0; i < arr.size(); ++i) {
        cum_count += arr[i];

        int cum_temp = 0;
        for (size_t j = 0; j < arr.size(); ++j) {
            cum_temp += arr[j];
            if (cum_temp > cum_count) {
                break;
            } else if (cum_temp < cum_count) {
                continue;
            } else {
                cum_temp = 0;
            }
        }
        if (cum_temp == 0) {
            result.push_back(cum_count);
        }
    }
    return result;
}

#endif

