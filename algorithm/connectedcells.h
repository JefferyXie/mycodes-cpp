#ifndef CONNECTEDCELLS_H
#define CONNECTEDCELLS_H

#include "../main/header.h"

// https://www.hackerrank.com/challenges/connected-cell-in-a-grid
struct MatrixInfo {
    int m, n; // rows, columns
    vector<vector<int> > arr;
    vector<vector<bool> > checked;
};

int checkcell(MatrixInfo& info, int i/*row*/, int j/*column*/) {
    if (i < 0 || j < 0 || i >= info.m || j >= info.n ||
        !info.arr[i][j] || info.checked[i][j]) return 0;

    int count_connected = 1;
    info.checked[i][j] = true;
    // check horizontal (right)
    count_connected += checkcell(info, i, j+1);
    // check horizontal (left)
    count_connected += checkcell(info, i, j-1);
    // check vertical (down)
    count_connected += checkcell(info, i+1, j);
    // check vertical (up)
    count_connected += checkcell(info, i-1, j);
    // check diagonal (right-down) 
    count_connected += checkcell(info, i+1, j+1);
    // check diagonal (right-up) 
    count_connected += checkcell(info, i-1, j+1);
    // check diagonal (left-down) 
    count_connected += checkcell(info, i+1, j-1);
    // check diagonal (left-up) 
    count_connected += checkcell(info, i-1, j-1);
    return count_connected;
}

int MaxConnectedCells(vector<vector<int> >& matrix) {
    MatrixInfo info;
    info.m = matrix.size();
    info.n = matrix[0].size();
    info.arr = matrix;
    int count_connected = 0;
    for (int i = 0; i < info.m; ++i) {
        for (int j = 0; j < info.n; ++j) {
            info.checked.resize(info.m, vector<bool>(info.n, false));
            count_connected = max(count_connected, checkcell(info, i, j));
        }
    }
    return count_connected;

    /*MatrixInfo info;
    cin >> info.m >> info.n;
    for (int i = 0; i < m; ++i) {
        arr.push_back(vector<int>());
        for (int j = 0; j < n; ++j) {
            int v;
            cin >> v;
            arr[i].push_back(v);
        }
    }
    int count_connected = 0;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            checked.resize(m, vector<bool>(n, false));
            count_connected = max(count_connected, checkcell(i, j));
        }
    }
    cout << count_connected << endl;
    return 0;*/
}

#endif

