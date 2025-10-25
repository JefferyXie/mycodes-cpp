#ifndef GRAPH_SHORTEST_PATH_H
#define GRAPH_SHORTEST_PATH_H

#include "../main/header.h"

//
// https://www.hackerrank.com/challenges/bfsshortreach
//

// breath-first-search version
std::vector<int> graph_shortest_path(int num_nodes, int edge_weight, std::vector<vector<int>> edges, int start_node)
{
    /*
        num_nodes = 10;
        edge_weight = 6;
        start_node = 3;
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

    // by using array, we don't need to construct the graph
    std::vector<std::vector<int>> matrix(num_nodes + 1, std::vector<int>(num_nodes + 1, -1));
    for (auto& eg : edges) {
        matrix[eg[0]][eg[1]] = 1 * edge_weight;
        matrix[eg[1]][eg[0]] = 1 * edge_weight;
    }
    // TODO: need to find a way to remove this set
    std::unordered_set<int> visited;
    std::queue<int>         qs;
    qs.push(start_node);
    do {
        int node = qs.front();
        // TODO: here is a trick, SHOULD be able to potentially reduce loop
        for (int j = 1; j <= num_nodes; ++j) {
            if (matrix[node][j] == 1 * edge_weight && visited.count(j) == 0) {
                qs.push(j);

                if (matrix[start_node][j] < 0) {
                    matrix[start_node][j] = matrix[start_node][node] + 1 * edge_weight;
                    matrix[j][start_node] = matrix[start_node][node] + 1 * edge_weight;
                }
            }
        }
        visited.emplace(node);
        qs.pop();
    } while (!qs.empty());

    auto result = matrix[start_node];
    result.erase(result.begin() + start_node);
    result.erase(result.begin());
    return result;
}

#endif
