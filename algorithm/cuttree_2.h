#ifndef CUTTREE_2_H
#define CUTTREE_2_H

#include "../header.h"

// https://www.hackerrank.com/challenges/cut-the-tree
// https://github.com/derekhh/HackerRank/blob/master/cut-the-tree.cpp
// http://www.cnblogs.com/sunshineatnoon/p/3914990.html
// this version follows these steps -
// 1) construct nodes container, since we don't know which is parent in the pair, we record 
// both vertices in the container with node @value as vertice index
// 2) calculate each node's sum recursively
// 3) get the min_diff
// advantange vs. version 1 -
// 1) no needs to create the tree
// 2) utilize index (@value) to identify parent/child node, which is much faster
struct TreeNode_Cut_2 {
    bool visisted;
    int value;
    int sum;
    vector<int> children;
};
int SumBranchVertices_2(vector<TreeNode_Cut_2>& nodes, int current) {
    auto& node = nodes[current];
    if (node.visisted) return 0;

    node.visisted = true;
    int sumBranch = node.value;
    for (int i = 0; i < (int)node.children.size(); ++i) {
        int k = node.children[i];
        sumBranch += SumBranchVertices_2(nodes, k);
    }
    node.sum = sumBranch;
    return node.sum;
}

int CutTree_2(const vector<int>& vertices, const vector<pair<int, int> >& edges) {
    vector<TreeNode_Cut_2> nodes(vertices.size()+1);
    for (int i = 0; i < (int)edges.size(); ++i) {
        int f = edges[i].first;
        int s = edges[i].second;
        nodes[f].visisted = false;
        nodes[f].sum = 0;
        nodes[f].value = vertices[f-1];
        nodes[f].children.push_back(s);
        nodes[s].visisted = false;
        nodes[s].sum = 0;
        nodes[s].value = vertices[s-1];
        nodes[s].children.push_back(f);
    }
    int min_diff = SumBranchVertices_2(const_cast<vector<TreeNode_Cut_2>& >(nodes), 1);
    for (int i = 0; i < (int)nodes.size(); ++i) {
        const auto& node = nodes[i];
        min_diff = min(abs(nodes[1].sum-2*node.sum), min_diff);
    }
    return min_diff;
}

#endif


