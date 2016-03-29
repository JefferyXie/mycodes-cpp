#ifndef GRIDSEARCH_H
#define GRIDSEARCH_H

#include "../header.h"

// https://www.hackerrank.com/challenges/the-grid-search
// using string vector rather than char/int array makes the code easier
bool GridSearch(const vector<string>& G, const vector<string>& P) {
    int rows = P.size();
    int cols = P[0].size();
    bool found = false;
    for (int i = 0; i < (int)G.size()-rows+1; ++i) {
        for (int j = 0; j < (int)G[0].size()-cols+1; ++j) {
            for (int k = 0; k < rows; ++k) {
                auto sub = G[i+k].substr(j, cols);
                found = sub.compare(P[k]) == 0;
                if (!found) break;
            }
            if (found) break;
        }
        if (found) break;
    }
    return found;
}

#endif


