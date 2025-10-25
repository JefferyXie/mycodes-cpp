#ifndef CUTTREE_H
#define CUTTREE_H

#include "../main/header.h"

// https://www.hackerrank.com/challenges/cut-the-tree
// this version follows these steps -
// 1) construct the tree by recursively finding node in top-down manner
// 2) calculate each node's sum recursively
// 3) get the min_diff value
// problems for this solution -
// 1) spend a lot of time when building the tree, O(n^depth), will be time out
// 2) doesn't use information that each std::pair represents vertices' index
template <class T>
class TreeNode_Cut
{
public:
    T                            data;       // index
    int                          vertice;    // vertice value
    std::vector<TreeNode_Cut<T>> children;
};

int SumAllVertices = 0;

TreeNode_Cut<int> BuildTree(
    int parent, const std::vector<int>& vertices, const std::vector<std::pair<int, int>>& edges,
    std::vector<bool>& visited)
{
    TreeNode_Cut<int> nodeParent;
    nodeParent.data    = parent;
    nodeParent.vertice = vertices[nodeParent.data - 1];
    for (int i = 0; i < (int)edges.size(); ++i) {
        if (visited[i])
            continue;

        const auto& edge      = edges[i];
        int         childData = 0;
        if (parent == edge.first) {
            childData = edge.second;
        } else if (parent == edge.second) {
            childData = edge.first;
        } else {
            continue;
        }

        visited[i] = true;

        nodeParent.children.push_back(BuildTree(childData, vertices, edges, visited));
    }
    return nodeParent;
}

int SumBranchVertices(const TreeNode_Cut<int>& root)
{
    int sum = root.vertice;
    for (int i = 0; i < (int)root.children.size(); ++i) {
        sum += SumBranchVertices(root.children[i]);
    }
    return sum;
}
int Calc_treediff(const TreeNode_Cut<int>& root)
{
    int min_diff = SumAllVertices;
    for (int i = 0; i < (int)root.children.size(); ++i) {
        int sumBranch = SumBranchVertices(root.children[i]);
        min_diff      = std::min(abs(SumAllVertices - 2 * sumBranch), min_diff);
        min_diff      = std::min(Calc_treediff(root.children[i]), min_diff);
    }
    return min_diff;
}
int CutTree(const std::vector<int>& vertices, const std::vector<std::pair<int, int>>& edges)
{
    for_each(begin(vertices), end(vertices), [](int v) {
        SumAllVertices += v;
    });
    std::vector<bool> visited(edges.size(), false);
    auto              root = BuildTree(1, vertices, edges, visited);
    return Calc_treediff(root);
}

#endif

