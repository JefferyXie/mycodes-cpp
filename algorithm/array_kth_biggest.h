#ifndef ARRAY_KTH_BIGGEST_H
#define ARRAY_KTH_BIGGEST_H

#include "../main/header.h"

//
// Find the k-th biggest number in an unordered integer array
//

int array_kth_biggest(std::vector<int> arr, size_t k)
{
    int min_value = std::numeric_limits<int>::max();
    int max_value = std::numeric_limits<int>::min();
    for (size_t i = 0; i < arr.size(); ++i) {
        min_value = std::min(min_value, arr[i]);
        max_value = std::max(max_value, arr[i]);
    }

    while (min_value < max_value) {
        size_t    count  = 0;
        const int median = (min_value + max_value) / 2;
        for (size_t i = 0; i < arr.size(); ++i) {
            if (arr[i] > median) {
                ++count;
            }
        }

        if (count >= k) {
            min_value = median + 1;
        } else {
            max_value = median;
        }
    }
    return max_value;
}

// Use minimum priority queue (top node is min value)
int array_kth_biggest_v2(std::vector<int> arr, size_t k)
{
    std::priority_queue<int, std::vector<int>, std::greater<int>> pri_queue;
    for (size_t i = 0; i < arr.size(); i++) {
        pri_queue.push(arr[i]);
        if (pri_queue.size() > k) {
            pri_queue.pop();
        }
    }

    return pri_queue.top();
}

// Use quick selection method, time complexity is O(n) since it only iterates half array (n * 1/2 + n * 1/4 + ... -> n)
// 1) pick up a pivot value, i.e. arr[0];
// 2) go through the array, move the element > pivot to left side, others on the right, arr[i] == pivot;
// 3) if i == k - 1, return arr[i];
//    if i > k - 1, do 1-2) again with arr[0] to arr[i - 1];
//    if i < k - 1, do 1-2) again with arr[i + 1] to arr[n], k = k - i - 1;
int array_kth_biggest_v3(std::vector<int> arr, size_t k)
{
    // TODO: ...
    return 0;
}

#endif
