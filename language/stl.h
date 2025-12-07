#pragma once

#include "../core/header.h"

class STL
{
public:
    static void Swap()
    {
        std::vector<int> v1{2, 1, 3, 4, 5, 0};
        std::vector<int> v2{1, 2, 3, 4};

        std::vector<int>().swap(v1);
        std::swap(v1, v2);
    }
    static void ostream_vector()
    {
        std::vector<int>           myVec;
        std::ostream_iterator<int> out(std::cout, " ");
        // generate array
        for (long i = 0; i < 10; i++)
            myVec.push_back(i);
        // shuffle the array
        std::random_shuffle(myVec.begin(), myVec.end());
        std::copy(myVec.begin(), myVec.end(), out);
        // sort the array in ascending order
        std::sort(myVec.begin(), myVec.end());
        std::copy(myVec.begin(), myVec.end(), out);
    }
    static void ostream_array()
    {
        std::ostream_iterator<int> out(std::cout, " ");
        // generate array (note: one extra element, end in STL is one element past last valid)
        int myVec[11];
        for (long i = 0; i < 10; i++)
            myVec[i] = i;
        int* begin = &myVec[0];
        int* end   = &myVec[10];
        // shuffle the array
        std::random_shuffle(begin, end);
        std::copy(begin, end, out);
        // sort the array in ascending order
        std::sort(begin, end);
        std::copy(begin, end, out);
    }
};

