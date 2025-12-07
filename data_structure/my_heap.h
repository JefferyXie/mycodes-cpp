#pragma once

#include "../core/utility.h"

//
/*
 * https://www.codeproject.com/Tips/816934/Min-Binary-Heap-Implementation-in-Cplusplus
 *
 * A good introduction: https://www.programiz.com/dsa/heap-sort#heap
 */

//
// build min heap
//
void heapify_min(int* arr, int idx_ending_value)
{
    int idx_parent = (idx_ending_value - 1) / 2;
    if (idx_ending_value == 0 || idx_parent < 0)
        return;

    if (*(arr + idx_parent) > *(arr + idx_ending_value)) {
        std::swap(*(arr + idx_parent), *(arr + idx_ending_value));

        heapify_min(arr, idx_parent);
    }
}

void build_min_heap(int* arr, int len)
{
    int idx = 0;
    while (idx++ < len) {
        heapify_min(arr, idx - 1);
    }
}

//
// convert min heap to max heap
//
void min_to_max_node(int* arr, int len, int idx)
{
    int idx_left_child  = 2 * idx + 1;
    int idx_right_child = 2 * idx + 2;

    int idx_max = idx_left_child;
    if (idx_left_child < len && idx_right_child < len && *(arr + idx_left_child) < *(arr + idx_right_child)) {
        idx_max = idx_right_child;
    }

    if (idx_max < len && *(arr + idx_max) > *(arr + idx)) {
        std::swap(*(arr + idx), *(arr + idx_max));
        min_to_max_node(arr, len, idx_max);
    }
}

void min_heap_to_max_heap(int* arr, int len)
{
    int idx_last_non_leaf = (len - 2) / 2;
    for (int idx = idx_last_non_leaf; idx >= 0; --idx) {
        min_to_max_node(arr, len, idx);
    }
}

void run_min_heap_to_max_heap()
{
    std::cout << __FILE__ << ":" << __FUNCTION__ << std::endl;

    // input: A[] = [1 2]
    // output: [2 1]
    {
        int arr[] = {1, 2};
        std::cout << "\n" << util::dump_array(arr);
        min_heap_to_max_heap(arr, ARRAY_SIZE(arr));
        std::cout << "\n" << util::dump_array(arr);

        build_min_heap(arr, ARRAY_SIZE(arr));
        std::cout << "\n" << util::dump_array(arr);
    }

    // input: A[] = [1 3 2]
    // output: [3 1 2]
    {
        int arr[] = {1, 3, 2};
        std::cout << "\n" << util::dump_array(arr);
        min_heap_to_max_heap(arr, ARRAY_SIZE(arr));
        std::cout << "\n" << util::dump_array(arr);

        build_min_heap(arr, ARRAY_SIZE(arr));
        std::cout << "\n" << util::dump_array(arr);
    }

    // input: A[] = [3 5 9 6 8 20 10 12 18 9]
    // output: A[] = [20 18 10 12 9 9 3 5 6 8]
    {
        int arr[] = {3, 5, 9, 6, 8, 20, 10, 12, 18, 9};
        std::cout << "\n" << util::dump_array(arr);
        min_heap_to_max_heap(arr, ARRAY_SIZE(arr));
        std::cout << "\n" << util::dump_array(arr);

        build_min_heap(arr, ARRAY_SIZE(arr));
        std::cout << "\n" << util::dump_array(arr);
    }

    std::cout << std::endl;
}

