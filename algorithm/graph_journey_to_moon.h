#pragma once

#include "../core/header.h"

//
// https://www.hackerrank.com/challenges/journey-to-the-moon
//

int graph_journey_to_moon(int n, std::vector<std::vector<int>> astronaut)
{
    /* fail with below case
    n = 100000;
    astronaut.assign({
        vector<int>{1, 2},
        vector<int>{3, 4}
    }); // 4999949998
    */

    // use map than array can save memory
    std::unordered_map<int, std::unordered_set<int>> astronaut_map;
    for (auto& p : astronaut) {
        astronaut_map[p[0]].insert(p[1]);
        astronaut_map[p[1]].insert(p[0]);
    }
    std::vector<int>  countries;
    std::vector<bool> visited(n, false);
    std::queue<int>   q;
    for (int i = 0; i < n; ++i) {
        q.push(i);
        int num = 0;
        do {
            auto ast = q.front();
            if (!visited[ast]) {
                auto it = astronaut_map.find(ast);
                if (it != astronaut_map.end()) {
                    for (auto v : it->second) {
                        q.push(v);
                    }
                }
                /*
                 for (int j = 0; j < n; ++j) {
                     if (arr[ast][j]) {
                         q.push(j);
                     }
                 }*/
                visited[ast] = true;
                ++num;
            }
            q.pop();
        } while (!q.empty());
        if (num) {
            countries.push_back(num);
        }
    }

    int total_pairs          = 0;
    int num_picked_astronaut = 0;
    for (auto& num_astronaut : countries) {
        total_pairs += num_astronaut * (n - num_picked_astronaut - num_astronaut);
        num_picked_astronaut += num_astronaut;
    }
    return total_pairs;
}

