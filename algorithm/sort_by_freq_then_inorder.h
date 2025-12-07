#pragma once

#include "../core/header.h"

//
/*
 * Given an array, output by element's occurence; keep the original appearance order if same freqency
 *
 * Examples :
 *
    Input:  Array [6,1,2,2,1,3,3,3,9]
    Output: 3,1,2,6,9
 *
 */

// map + priority_queue

/*
 For std::priority_queue<std::pair<int, int>>, by default the comparator will compare based
 on the first element; if the values are equal, it will then compare based on the second element.
    template <class T1, class T2>
    bool operator<(const pair<T1, T2>& x, const pair<T1, T2>& y)
    {
        return x.first < y.first || (!(y.first < x.first) && x.second < y.second);
    }
*/

std::vector<int> sort_by_freq_then_inorder(std::vector<int>& nums)
{
    std::unordered_map<int, int>             freq;
    std::priority_queue<std::pair<int, int>> maxHeap;
    std::vector<int>                         res;
    int                                      n = nums.size();
    if (n == 0)
        return res;

    for (int i = 0; i <= n - 1; i++) {
        ++freq[nums[i]];
    }
    for (auto it = freq.begin(); it != freq.end(); it++) {
        // TODO: it->first is the actual element, this way will make the same frequency
        // elements output by ascend
        // To keep element's original appearance order, we should replace it->first with
        // element's index in input array, then output by nums[it->first]
        maxHeap.push(std::pair<int, int>(it->second, it->first));
    }
    while (!maxHeap.empty()) {
        res.push_back(maxHeap.top().second);
        maxHeap.pop();
    }
    return res;
}

