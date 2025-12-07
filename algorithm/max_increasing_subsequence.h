#pragma once

#include "../core/header.h"

//
// https://www.youtube.com/watch?v=EgXQp1YOtyY&ab_channel=Simplilearn
//
// max increasing sub-sequence (not necessarily continuous) in the array, rather
// than sub-array (continuous)
//
// arr = [10, 7, 12, 30, 22, 35, 60, 9]
// max = [1,  1,  2,  3,  3,  4,  5, 5]
//
// arr = [10, 7, 12, 30, 11, 5, 60, 9]
// max = [1,  1,  2,  3,  2, 1,  4, 2]

unsigned max_increasing_subsequence(std::vector<int> arr)
{
    unsigned              result = 1;
    std::vector<unsigned> max_subsequence(arr.size(), 1);
    for (size_t i = 1; i < arr.size(); ++i) {
        for (size_t j = 0; j < i; ++j) {
            // 1) get max increasing sequence if current arr[i] is part of that -
            //    this doesn't have to be the overall optimal sequence
            // 2) since only one new element is added, the max size increased is 1
            if (arr[i] > arr[j] && max_subsequence[i] < max_subsequence[j] + 1) {
                max_subsequence[i] += 1;
            }
        }
        if (max_subsequence[i] > result) {
            result = max_subsequence[i];
        }
    }
    auto print = [](std::string prefix, auto& input) {
        std::cout << prefix << ": [";
        for (size_t i = 0; i < input.size(); ++i) {
            if (i > 0)
                std::cout << ", ";
            std::cout << std::setfill(' ') << std::setw(4) << input.at(i);
        }
        std::cout << "]" << std::endl;
    };
    print("arr", arr);
    print("max", max_subsequence);
    return result;
}

