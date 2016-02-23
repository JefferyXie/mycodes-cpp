#ifndef STL_H
#define STL_H

#include "../header.h"

class STL {
public:
    static void Swap() {
        vector<int> v1{2,1,3,4,5,0};
        vector<int> v2{1,2,3,4};

        vector<int>().swap(v1);
        swap(v1, v2);
    }
    static void ostream_vector() {
        vector<int>              myVec;
        vector<int>::iterator    item;
        ostream_iterator<int>    out(cout," ");
        // generate array
        for (long i=0; i<10; i++)
            myVec.push_back(i);
        // shuffle the array
        random_shuffle(myVec.begin(), myVec.end());
        copy(myVec.begin(), myVec.end(), out);
        // sort the array in ascending order
        sort(myVec.begin(), myVec.end());
        copy(myVec.begin(), myVec.end(), out);
    }
    static void ostream_array() {
        ostream_iterator<int>  out(cout," ");
        // generate array (note: one extra element, end in STL is one element past last valid)
        int myVec[11];
        for (long i=0; i<10; i++)
            myVec[i] = i;
        int* begin = &myVec[0];
        int* end = &myVec[10];
        // shuffle the array
        random_shuffle(begin, end);
        copy(begin, end, out);
        // sort the array in ascending order
        sort(begin, end);
        copy(begin, end, out);
    }
};

#endif // STL_H
