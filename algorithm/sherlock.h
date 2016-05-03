#ifndef SHERLOCK_H
#define SHERLOCK_H

#include "../main/header.h"

// https://www.hackerrank.com/challenges/sherlock-and-queries
// http://codereview.stackexchange.com/questions/77975/time-limit-exceeded-for-sherlock-and-queries-hackerrank-challenge
int Sherlock() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */
    int N = 0;
    int M = 0;
    cin >> N >> M;
    if (N < 1 || M < 1 || N > 100000 || M > 100000) return -1;
    int A[N];// = {0};
    int B[M];// = {0};
    int C[M];// = {0};

    std::string line;
    std::istringstream ss(line);
   
    int temp = 0;
    int index = 0;
    while (std::getline(cin, line)) {
        if (line.empty()) continue;
        ss.str(line);
        ss.seekg(0, ss.beg);
        break;
    }
    while (ss >> temp) {
        if (temp < 1 || temp > 100000) return -1;
        A[index++] = temp;
        if (index >= N) break;
    }
    index = 0;
    while (std::getline(cin, line)) {
        if (line.empty()) continue;
        ss.str(line);
        ss.seekg(0, ss.beg);
        break;
    }
    while (ss >> temp) {
        if (temp < 1 || temp > N) return -1;
        B[index++] = temp;
        if (index >= M) break;
    }
    index = 0;
    while (std::getline(cin, line)) {
        if (line.empty()) continue;
        ss.str(line);
        ss.seekg(0, ss.beg);
        break;
    }
    while (ss >> temp) {
        if (temp < 1 || temp > 100000) return -1;
        C[index++] = temp;
        if (index >= M) break;
    }
    
    int B_divisor[N+1];// = {0};
    for (int i = 0; i < M; ++i) {
        // use B[i] as index, multipled by C[i], in this way, duplicate elements
        // in B are removed without information loss from C.
        if (B_divisor[B[i]] == 0)
            B_divisor[B[i]] = C[i];
        else
            B_divisor[B[i]] *= C[i];
    }
    for (int divisor = 0; divisor <= N; ++divisor) {
        int multiplier = B_divisor[divisor];
        // if value is 0, means original B array doesn't include this value
        if (multiplier <= 1) continue;
        // 1), here the divisor is the element index of original B, and the value b_divisor 
        // combines all information from array C
        // 2), starting from divisor instead of 0, since this is the first element that index 
        // is divisible by divisor
        // 3), step size is divisor, to make sure the index is divisible by divisor
        for (int j = divisor; j <= N; j = j + divisor) {
            A[j-1] *= multiplier;
        }
    }
    /*for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            if ((j+1) % B[i] == 0)
                A[j] *= C[i];
        }
    }*/
    for (int i = 0; i < N; ++i) {
        cout << A[i] << " ";
    }
    cout << endl;

    return 0;
}


#endif // SHERLOCK

