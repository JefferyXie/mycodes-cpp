#ifndef GRAPH_SHORTEST_PATH_H
#define GRAPH_SHORTEST_PATH_H

#include "../main/header.h"

// 
// https://www.hackerrank.com/challenges/bfsshortreach
// 

// breath-first-search verion
vector<int>
graph_shortest_path(int n, int m, vector<vector<int>> edges, int s)
{
/*
    n = 10;
    m = 6;
	s = 3;
    edges.assign({
    vector<int>{3, 1},
    vector<int>{10, 1},
    vector<int>{10, 1},
    vector<int>{3, 1},
    vector<int>{1, 8},
    vector<int>{5, 2}
    });
    // 6 -1 -1 -1 -1 -1 12 -1 12

*/
    int weight = 6;
	// by using array, we don't need to construct the graph
    vector<vector<int>> matrix(n+1, vector<int>(n+1, -1));
    for (auto& eg : edges) {
        matrix[eg[0]][eg[1]] = 1*weight;
        matrix[eg[1]][eg[0]] = 1*weight;
    }
	// TODO: need to find a way to remove this set
    unordered_set<int> poped;
    queue<int> qs;
    qs.push(s);
    do {
        int node = qs.front();
		// TODO: here is a trick, SHOULD be able to potentially reduce loop
        for (unsigned j = 1; j <= n; ++j) {
            if (matrix[node][j] == 1*weight && poped.count(j) == 0) {
                qs.push(j);

                if (matrix[s][j] < 0) {
                    matrix[s][j] = matrix[s][node] + 1*weight;
                    matrix[j][s] = matrix[s][node] + 1*weight;
                }
            }
        }
        poped.emplace(node);
        qs.pop();
    } while (!qs.empty());
    auto result = matrix[s];
    result.erase(result.begin() + s);
    result.erase(result.begin());
    return result;
}

#endif

