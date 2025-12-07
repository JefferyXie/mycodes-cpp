#pragma once

#include "../core/header.h"

//
// Given an array [0,1,1,0,1,1,1]; output should be [0,1,1,0,1,2,3] ie., for every non-zero element
// in array, find its distance to the nearest zero in O(n) time.
//
// Solution 1): store the positions of 0, then calculate the distance
void min_distance_from_zero_1(const std::vector<int>& input, std::vector<int>& output)
{
    //    std::vector<int> v = {0, 1, 1, 0, 1, 1, 1, 0};
    output.resize(input.size(), 0);
    std::vector<int> zeros_pos;

    for (int i = 0; i < (int)input.size(); i++) {
        if (!input[i]) {
            zeros_pos.push_back(i);
        }
    }

    int fz = zeros_pos[0];
    int sz = zeros_pos[1];
    int j  = 2;

    for (int i = 0; i < 8; i++) {
        output[i] = std::min(abs(i - fz), abs(i - sz));
        if (i == sz) {
            fz = sz;
            if (j < (int)zeros_pos.size()) {
                sz = zeros_pos[j];
                j++;
            }
        }
    }
}

// Solution 2): loop starts from left, calculate distance from current element to last 0 on the left;
// then loop starts from right, calculate distance from current element to last 0 on the right, and
// compare it with the number from above step, get the result.
void min_distance_from_zero_2(const std::vector<int>& input, std::vector<int>& output)
{
    int length = input.size();
    output.resize(length, 0);
    int pos_last_zero = -1;
    for (int i = 0; i < length; ++i) {
        if (input[i]) {
            if (pos_last_zero >= 0)
                output[i] = i - pos_last_zero;
            else
                output[i] = length;
        } else {
            pos_last_zero = i;
        }
    }
    pos_last_zero = -1;
    for (int i = length - 1; i >= 0; --i) {
        if (input[i]) {
            if (pos_last_zero >= 0)
                output[i] = (output[i] > (pos_last_zero - i) ? (pos_last_zero - i) : output[i]);
        } else {
            pos_last_zero = i;
        }
    }
}

