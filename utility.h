#ifndef UTILITY_H
#define UTILITY_H

#include "header.h"

template<class T> string ToString(T v)
{
    ostringstream ss;
    ss << v;
    return ss.str();
}

// swap values without introducing temp variable
template<class T> void swap_plus(T& a, T& b)
{
    a = a + b;
    b = a - b;
    a = a - b;
}

// sorting algorithm, http://blog.csdn.net/semo_1006/article/details/4706890
// method 1: insertion sort
template<class T> void sort_inserttion(const vector<T>& src, vector<T>& out)
{

}

// method 2: merge sort
// http://blog.chinaunix.net/uid-24774106-id-2240452.html
template<class T> void sort_merge(const vector<T>& src, vector<T>& out)
{

}

// method 3: quick sort
template<class T> void sort_quick(const vector<T>& src, vector<T>& out)
{

}

// major container's time complexity -
// http://en.wikipedia.org/wiki/Search_data_structure
template<class T> void quickSort(T s[], int l, int r)
{
    if (l < r)
    {
        int i = l, j = r;
        T x = s[l];
        while (i < j)
        {
            while(i < j && s[j] >= x) // 从右向左找第一个小于x的数
                j--;
            if(i < j)
                s[i++] = s[j];
            while(i < j && s[i] < x) // 从左向右找第一个大于等于x的数
                i++;
            if(i < j)
                s[j--] = s[i];
        }
        s[i] = x;
        quickSort(s, l, i - 1); // 递归调用
        quickSort(s, i + 1, r);
    }
}

// maximum continuous subarray
template<class T> T maxSubArray(const vector<T>& src)
{
    return *src.begin();
}

#endif
