#pragma once

#include "../core/header.h"

// [TBD] this problem has not yet been solved!
//
// https://www.hackerrank.com/challenges/similarpair
// http://stackoverflow.com/questions/16505216/similar-pairs-in-a-tree
// http://blog.csdn.net/tspatial_thunder/article/details/42832687
struct IntNode {
    int parent;
    int child;
    IntNode(int p, int c) : parent(p), child(c) {}
};
// with map
int countSimilar(int T, int originalChild, int curParent, std::map<int, int>& tree)
{
    int count = 0;
    if (abs(originalChild - curParent) <= T)
        count++;
    auto parentNode = tree.find(curParent);
    if (parentNode != tree.end()) {
        count += countSimilar(T, originalChild, parentNode->second, tree);
    }
    return count;
}
// with unordered_map
int countSimilar(int T, int originalChild, int curParent, std::unordered_map<int, int>& tree)
{
    int count = 0;
    if (abs(originalChild - curParent) <= T)
        count++;
    auto parentNode = tree.find(curParent);
    if (parentNode != tree.end()) {
        count += countSimilar(T, originalChild, parentNode->second, tree);
    }
    return count;
}
// with vector
int countSimilar(int T, int originalChild, int curParent, std::vector<IntNode>& tree)
{
    int count = 0;
    if (abs(originalChild - curParent) <= T)
        count++;
    for (int i = 0; i < (int)tree.size(); ++i) {
        const auto& node = tree[i];
        if (curParent != node.child)
            continue;
        count += countSimilar(T, originalChild, node.parent, tree);
    }
    return count;
}

int SimilarPair(int T, int arr[][2])
{
    std::unordered_map<int, int> treeM;
    for (int i = 0; i < (int)sizeof(arr) / sizeof(arr[0]); ++i) {
        int parent, child;
        parent = arr[i][0];
        child  = arr[i][1];
        treeM.insert(std::make_pair(child, parent));
    }
    int count = 0;
    for (auto it = treeM.begin(); it != treeM.end(); ++it) {
        std::cout << it->first << "," << it->second << std::endl;
        count += countSimilar(T, it->first, it->second, treeM);
    }
    return count;
}

