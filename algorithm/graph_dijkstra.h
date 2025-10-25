#ifndef GRAPH_DIJKSTRA_H
#define GRAPH_DIJKSTRA_H

#include "../main/header.h"

//
// https://www.hackerrank.com/challenges/dijkstrashortreach
//

// This is actually Dijkstra’s shortest path algorithm used to calc
// single source shortest path
// https://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-greedy-algo-7/
// https://www.techiedelight.com/single-source-shortest-paths-dijkstras-algorithm/
//
// In contrast, Floyd Warshall algorithm is using to calc shortest path between
// any pairs, time complexity is O(n^3)
// https://en.wikipedia.org/wiki/Floyd%E2%80%93Warshall_algorithm
// https://www.geeksforgeeks.org/comparison-dijkstras-floyd-warshall-algorithms/

// Another practice can be solved by Dijkstra's shortest path algorithm
// https://www.hackerrank.com/challenges/jack-goes-to-rapture/

// TODO: idea is right, somehow the code is buggy
// this is actually not Dijkstra algorithm, check above geeksforgeeks link for details
vector<int> graph_dijkstra(int n, vector<vector<int>> edges, int s)
{
    vector<int> results(n + 1, -1);
    // < 0: child and length, 0: not connected, 1: parent, > 1: higher ancestor
    vector<vector<int>> ancestors(n + 1, vector<int>(n + 1, 0));
    for (auto& eg : edges) {
        auto p1           = eg[0];
        auto p2           = eg[1];
        auto length       = eg[2];
        ancestors[p1][p2] = -1 * length;
        ancestors[p2][p1] = -1 * length;
    }
    queue<int> node_q;
    // breath first search
    node_q.push(s);
    do {
        auto node = node_q.front();
        for (int i = 1; i <= n; ++i) {
            auto v = ancestors[node][i];
            if (v == 0)
                continue;
            // v < 0: child and not yet reached with same path before
            if (v < 0) {
                auto length = -v;
                // results[i] < 0: not yet have path from s -> i
                if (results[i] < 0) {
                    // results[node] < 0: not yet have path from s-> node
                    if (results[node] < 0) {
                        results[i] = length;
                    } else {
                        results[i] = results[node] + length;
                    }
                } else {
                    // if having had path from s -> node and s -> i
                    // pick the min path s -> node -> i, or s -> i
                    results[i] = min(results[node] + length, results[i]);
                }
                // won't consider [node, i] any more
                ancestors[node][i] = 1;
                node_q.push(i);
            } else {
                // won't consider [node, i] any more
                ++ancestors[node][i];
            }
        }

        node_q.pop();
    } while (!node_q.empty());
    results.erase(results.begin() + s);
    results.erase(results.begin());
    return results;
}

#endif

