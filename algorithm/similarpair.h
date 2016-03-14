#ifndef SIMILARPAIR_H
#define SIMILARPAIR_H

#include "../header.h"

// https://www.hackerrank.com/challenges/similarpair
// http://stackoverflow.com/questions/16505216/similar-pairs-in-a-tree
// http://blog.csdn.net/tspatial_thunder/article/details/42832687
struct IntNode {
    int parent;
    int child;
    IntNode(int p, int c) : parent(p), child(c) {}
};
// with map
int countSimilar(int T, int originalChild, int curParent, map<int, int>& tree) {
    int count = 0;
    if (abs(originalChild-curParent) <= T) count++;
    auto parentNode = tree.find(curParent);
    if (parentNode != tree.end()) {
        count += countSimilar(T, originalChild, parentNode->second, tree);
    }
    return count;
}
// with unordered_map
int countSimilar(int T, int originalChild, int curParent, unordered_map<int, int>& tree) {
    int count = 0;
    if (abs(originalChild-curParent) <= T) count++;
    auto parentNode = tree.find(curParent);
    if (parentNode != tree.end()) {
        count += countSimilar(T, originalChild, parentNode->second, tree);
    }
    return count;
}
// with vector
int countSimilar(int T, int originalChild, int curParent, vector<IntNode>& tree) {
    int count = 0;
    if (abs(originalChild-curParent) <= T) count++;
    for (int i = 0; i < (int)tree.size(); ++i) {
        const auto& node = tree[i];
        if (curParent != node.child) continue;
        count += countSimilar(T, originalChild, node.parent, tree);
    }
    return count;
}

int SimilarPair(int T, int arr[][2]) {
    unordered_map<int, int> treeM;
    for (int i = 0; i < (int)sizeof(arr)/sizeof(arr[0]); ++i) {
        int parent, child;
        parent = arr[i][0];
        child = arr[i][1];
        treeM.insert(make_pair(child, parent));
    }
    int count = 0;
    for (auto it = treeM.begin(); it != treeM.end(); ++it) {
        cout << it->first << "," << it->second << endl;
        count += countSimilar(T, it->first, it->second, treeM);
    }
    return count;
 }

#endif


