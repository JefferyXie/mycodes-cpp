#ifndef COUNTLUCK_H
#define COUNTLUCK_H

#include "../header.h"

// there have better solutions available from other developers on website!!
// https://www.hackerrank.com/challenges/count-luck
//
bool FindPortKey(const vector<vector<char> >& forest, 
                vector<vector<bool> >& visited, 
                int startN, int startM, vector<pair<int,int> >& waveHandList) {
    int N = (int)forest.size();
    int M = (int)forest.at(0).size();
    if (startN < 0 || startM < 0 || startN >= N || startM >= M || 
        visited[startN][startM] || 
        forest[startN][startM] == 'X') return false;
    if (forest[startN][startM] == '*') return true;

    int choices = (int)(forest[startN][startM] == 'M') + 
                (int)(startM+1 < M && (forest[startN][startM+1] == '.' ||
                                        forest[startN][startM+1] == 'M' ||
                                        forest[startN][startM+1] == '*')) +
                (int)(startM-1 >= 0 && (forest[startN][startM-1] == '.' ||
                                        forest[startN][startM-1] == 'M' || 
                                        forest[startN][startM-1] == '*')) +
                (int)(startN+1 < N && (forest[startN+1][startM] == '.' ||
                                        forest[startN+1][startM] == 'M' ||
                                        forest[startN+1][startM] == '*')) +
                (int)(startN-1 >= 0 && (forest[startN-1][startM] == '.' ||
                                        forest[startN-1][startM] == 'M' ||
                                        forest[startN-1][startM] == '*'));
    if (choices > 2) {
        waveHandList.push_back({startN, startM});
    }

    visited[startN][startM] = true;

    vector<pair<int,int> > waveHandListTemp;
    bool isFound = FindPortKey(forest, visited, startN, startM+1, waveHandListTemp);
    if (isFound) {
        waveHandList.insert(end(waveHandList), 
            make_move_iterator(begin(waveHandListTemp)),
            make_move_iterator(end(waveHandListTemp)));
        waveHandListTemp.clear();
    } else {
        waveHandListTemp.clear();
        isFound = FindPortKey(forest, visited, startN, startM-1, waveHandListTemp);
    }
    if (isFound) {
        waveHandList.insert(end(waveHandList), 
            make_move_iterator(begin(waveHandListTemp)),
            make_move_iterator(end(waveHandListTemp)));
        waveHandListTemp.clear();
    } else {
        waveHandListTemp.clear();
        isFound = FindPortKey(forest, visited, startN+1, startM, waveHandListTemp);
    }
    if (isFound) {
        waveHandList.insert(end(waveHandList), 
            make_move_iterator(begin(waveHandListTemp)),
            make_move_iterator(end(waveHandListTemp)));
        waveHandListTemp.clear();
    } else {
        waveHandListTemp.clear();
        isFound = FindPortKey(forest, visited, startN-1, startM, waveHandListTemp);
    }

    if (isFound) {
        waveHandList.insert(end(waveHandList), 
            make_move_iterator(begin(waveHandListTemp)),
            make_move_iterator(end(waveHandListTemp)));
        waveHandListTemp.clear();
    }

    return isFound;
}
int CalcWaveHandTimes(const vector<vector<char> >& forest) {
    int N = (int)forest.size();
    int M = (int)forest.at(0).size();
    int startN = -1;
    int startM = -1;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            if (forest[i][j] == 'M') {
                startN = i;
                startM = j;
                break;
            }
        }
        if (startN >= 0 && startM >= 0) break;
    }
    vector<vector<bool> > visited(N, vector<bool>(M, false));
    vector<pair<int,int> > waveHandList;
    FindPortKey(forest, visited, startN, startM, waveHandList);
    for_each(begin(waveHandList), end(waveHandList), [](pair<int,int>& p) {
        cout << "(" << p.first << "," << p.second << ")" << endl;
    });
    return waveHandList.size();;
}

#endif

