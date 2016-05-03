#ifndef DEQUE_MAXK_H
#define DEQUE_MAXK_H

#include "../main/header.h"

// https://www.hackerrank.com/challenges/deque-stl
string printKMax(int arr[], int n, int k) {
    if (n < k) return "";
    string result;
    int maxEle = 0;
    int popped = 0;
    deque<int> dq;
    for (int i = 0; i < n; ++i) {
        int pushed = arr[i];
        dq.push_back(pushed);
        if (popped < maxEle) {
            maxEle = max(maxEle, pushed);
        } else if (popped <= pushed) {
            maxEle = pushed;
        } else {
            maxEle = 0;
            for_each(begin(dq), end(dq), [&maxEle](int a) {
                maxEle = max(maxEle, a);
            });
        }
        if ((int)dq.size() == k) {
            result += to_string(maxEle) + ' ';
            //cout << maxEle << ' ';
            popped = dq.front();
            dq.pop_front();
        }
    }
    return result.erase(result.size()-1,1);
}

// version 2: 
// 1) don't search max value in the deque
// 2) the first element in deque always the biggest
// 3) the last element in deque always the smallest
// 4) keep removing last elements if newly added element is big
deque<int> q;
void add(int x) {
    while (!q.empty() && q.back() < x)
        q.pop_back();
    q.push_back(x);
}
void remove(int x) {
    if (!q.empty() && q.front() == x) {
        q.pop_front();
    }
}
string printKMax_2(int arr[], int n, int k) {
    if (n < k) return "";
    q.clear();
    string result;
    for (int i = 0; i < k; ++i) {
        add(arr[i]);
    }
    for (int i = k; i <= n; ++i) {
        result += to_string(q.front()) + ' ';
        //cout << q.front() << ' ';
        remove(arr[i - k]);
        add(arr[i]);
    }
    return result.erase(result.size()-1,1);
}

#endif


