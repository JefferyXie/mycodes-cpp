#ifndef FROG_JUMP_H
#define FROG_JUMP_H

#include "../main/header.h"

//
// Maintain a reachable flag array, a pos can be reached if its value greater or equal to 2
int frog_jump_1(vector<int> &A, int X, int D) {
    if (X <= D) return 0;

    vector<int> reachable(X+1, 1);
    for (int i = D+1; i < X-D; ++i)
    {
        reachable[i] = 0;
    }
    for (unsigned int t = 0; t < A.size(); ++t)
    {
        if (A[t] == 0) continue;

        int pos_l = max(A[t] - D, 0);
        int pos_r = min(A[t] + D, X);
        for (int i = pos_l; i <= pos_r; ++i)
        {
            ++reachable[i];
        }
        bool success = true;
        for (int i = 0; i <= X; ++i)
        {
            if (reachable[i] < 2)
            {
                success = false;
                break;
            }
        }
        if (success) return t;
    }
    return -1;
}

//
// Dynamic programming
int frog_jump_2(vector<int> &A, int X, int D) {
    if (X <= D) return 0;

    int all_time = A.size();
    vector<int> pos_shortest_time(X+1, -1);
    pos_shortest_time[0] = 0;
    for (int i = 0; i <= all_time; ++i)
    {
        if (pos_shortest_time[A[i]] < 0)
        {
            pos_shortest_time[A[i]] = i;
        }
    }
    int MAX_TIME = 100000;
    int required_time = 0;
    int i = 0;
    while (i < X - D)
    {
        int shortest = MAX_TIME;
        for (int j = i+1; j <= i+D; ++j)
        {
            if (pos_shortest_time[j] >= 0 && pos_shortest_time[j] < shortest)
            {
                shortest = pos_shortest_time[j];
                i = j;
            }
        }
        if (shortest == MAX_TIME)
            return -1;
        else if (shortest > required_time)
            required_time = shortest;
    }
    return required_time;
}

void run_frog_jump()
{
    vector<int> A{1,3,1,4,2,5};
    int X = 7;
    int D = 3;
    cout << frog_jump_1(A, X, D) << endl;
    cout << frog_jump_2(A, X, D) << endl;

    A = {3,3,1,4,2,5};
//    int X = 7;
    D = 4;
    cout << frog_jump_1(A, X, D) << endl;
    cout << frog_jump_2(A, X, D) << endl;

    A = {1,6};
    X = 9;
    D = 3;
    cout << frog_jump_1(A, X, D) << endl;
    cout << frog_jump_2(A, X, D) << endl;
}

#endif

