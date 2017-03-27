#ifndef MAP_USAGE_H
#define MAP_USAGE_H

#include "../main/header.h"

//
// We can use std::map, std::unordered_map and related algorithms to simplify your work
// 
// This function offers sample/solution to these questions:
// 1) std::map is ordered map and custom comparator (by key) is easily supported
// 2) find the top 3 elements whose value (not key) is biggest with std::partial_sort_copy
// 3) find the element that has biggest value (not key) with std::max_element
// 4) find the max & min elements by value (not key) with std::minmax_element
// 5) find non repeating number(s) in an array with std::unordered_map
// 
void map_usage()
{
    std::map<std::string, int> mm = {
        {"hello", 3},
        {"world", 2},
        {"yes", 4},
        {"no", 1},
        {"okay", 5},
        {"best", 0},
        {"regards", 6},
    };
    
    // std::map is ordered by the key, so the output by iterator should be in ascend order
    for (const auto& ele : mm) {
        std::cout << ele.first << ", " << ele.second << std::endl;
    }
    
    std::cout << "--------customize comparator by key's length--------" << std::endl;
    
    // use customize key comparator for std::map order
    auto length_comp = [](const std::string& k1, const std::string& k2) {
        return k1.length() > k2.length();
    };
    std::map<std::string, int, decltype(length_comp)> mm_length_comp = {
        {
            {"hello", 3},
            {"world", 2},
            {"yes", 4},
            {"no", 1},
            {"okay", 5},
            {"best", 0},
            {"regards", 6},
        },
        length_comp
    };
    for (const auto& ele : mm_length_comp) {
        std::cout << ele.first << ", " << ele.second << std::endl;
    }

    std::cout << "--------top 3 elements by value--------" << std::endl;
    
    // 
    // find the top 3 elements whose value is biggest
    std::vector<std::pair<std::string, int>> result(3);
    std::partial_sort_copy(mm.begin(), mm.end(),
                           result.begin(), result.end(),
                           [](const std::pair<std::string, int>& v1, const std::pair<std::string, int>& v2) {
                               return v1.second > v2.second;
                           });
    for (const auto& ele : result) {
        std::cout << ele.first << ", " << ele.second << std::endl;
    }
    
    std::cout << "--------max element by value--------" << std::endl;
    
    using pair_type = std::pair<std::string, int>;
    // 
    // find the element that has biggest value
    auto ele_max = std::max_element(mm.begin(), mm.end(),
                                          [](const pair_type& v1, const pair_type& v2) {
                                              return v1.second < v2.second;
                                          });
    std::cout << ele_max->first << ", " << ele_max->second << std::endl;
    
    std::cout << "--------max & min elements by value--------" << std::endl;
    
    // 
    // find the max & min elements by value
    auto ele_min_max = std::minmax_element(mm.begin(), mm.end(),
                                           [](const pair_type& v1, const pair_type& v2) {
                                               return v1.second < v2.second;
                                           });
    std::cout << ele_min_max.first->first << ", " << ele_min_max.first->second << "; "
              << ele_min_max.second->first << ", " << ele_min_max.second->second << std::endl;

    std::cout << "--------non repeating number in an array--------" << std::endl;
    int arr[] = {2,3,3,1,5,4,2,1,5,3};
    std::unordered_map<int, int> um_frequency;
    for (auto i : arr) {
        um_frequency[i]++;
    }
    std::for_each(begin(um_frequency), end(um_frequency),
                  [](const std::pair<int, int>& ele) {
                      if (ele.second == 1)
                      {
                          std::cout << ele.first << "; ";
                      }
                  });
}

// 
// find the top # elements in the container by using min-priority queue (std::priority_queue).
// 1) Unconditionnally insert into the queue the n first elements
// 2) For each remaining element x, insert x if it is greater than the least element of the
//    queue (O(log n) operation), and remove the least element (O(log n)).
// 3) When done, the priority queue contains n elements, which are the n largest elements of
//    the original array.
// Total complexity: O(N log n) where N is the total number of elements in the array.

// Priority Queue
// https://en.wikipedia.org/wiki/Priority_queue
// https://www.cise.ufl.edu/~sahni/cop3530/slides/lec232.pdf
// https://www.hackerearth.com/practice/notes/heaps-and-priority-queues/

#endif
