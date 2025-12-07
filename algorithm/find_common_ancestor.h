#pragma once

#include "../core/node.h"

//
// http://stackoverflow.com/questions/1484473/how-to-find-the-lowest-common-ancestor-of-two-nodes-in-any-binary-tree
// https://www.geeksforgeeks.org/dsa/lowest-common-ancestor-binary-tree-set-1/
//
// Use recursive
//
template <class T>
tree_node_t<T>* find_common_ancestor(tree_node_t<T>* root, tree_node_t<T>* n1, tree_node_t<T>* n2)
{
    // 1) root == nullptr: wrong branch, no node any more
    // 2) root == n1 || root == n2: reach node itself, return itself
    if (root == nullptr || root == n1 || root == n2)
        return root;

    auto left  = find_common_ancestor(root->left, n1, n2);
    auto right = find_common_ancestor(root->right, n1, n2);

    // two nodes dont stay in same branch, then root is common ancestor
    if (left != nullptr && right != nullptr)
        return root;

    // both nodes lie in same branch, common ancestor should be that branch
    return left != nullptr ? left : right;
}

// Find the whole path/route from root to n1 and n2 respectively, then get the common ancestor
template <class T>
tree_node_t<T>* find_common_ancestor_v2(tree_node_t<T>* root, tree_node_t<T>* n1, tree_node_t<T>* n2)
{
    using NodePtr_t    = tree_node_t<T>*;
    using NodePtrVec_t = std::vector<NodePtr_t>;
    using Lambda_t     = std::function<bool(NodePtr_t, NodePtr_t, NodePtrVec_t&)>;
    Lambda_t impl      = [&](NodePtr_t node_root, NodePtr_t node_target, NodePtrVec_t& route) {
        if (!node_root)
            return false;

        route.push_back(node_root);

        if (node_root == node_target)
            return true;

        if (impl(node_root->left, node_target, route) || impl(node_root->right, node_target, route)) {
            return true;
        }

        route.pop_back();
        return false;
    };

    NodePtrVec_t route_n1;
    NodePtrVec_t route_n2;

    impl(root, n1, route_n1);
    impl(root, n2, route_n2);

    NodePtr_t ancestor = nullptr;
    for (int i = 0; i < route_n1.size() && i < route_n2.size(); ++i) {
        if (route_n1[i] != route_n2[i]) {
            break;
        }
        ancestor = route_n1[i];
    }
    return ancestor;
}

//
// Another variant: find the min or max value on the path connecting n1 and n2
// https://www.geeksforgeeks.org/minimum-and-maximum-node-that-lies-in-the-path-connecting-two-nodes-in-a-binary-tree/#
//

