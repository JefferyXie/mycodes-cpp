#ifndef TREE_NEXT_RIGHT_H
#define TREE_NEXT_RIGHT_H

#include "../main/header.h"

//
// https://leetcode.com/problems/populating-next-right-pointers-in-each-node-ii/
//

struct Node {
    int   value = 0;
    Node* left  = nullptr;
    Node* right = nullptr;
    Node* next  = nullptr;
    Node(int v, Node* l, Node* r, Node* n) : value(v), left(l), right(r), next(n) {}
};

//
// solution 1)
//
void helper(Node* node, unsigned level, vector<Node*>& vec)
{
    if (!node)
        return;

    // If the 'previous' node on this level hasn't been recorded yet, set
    // current node as 'previous'.
    if (vec.size() <= level) {
        vec.push_back(node);
    } else {
        vec[level]->next = node;
        vec[level]       = node;
    }
    helper(node->left, level + 1, vec);
    helper(node->right, level + 1, vec);
}

Node* tree_next_right_v1(Node* root)
{
    if (!root)
        return NULL;
    vector<Node*> vec;
    helper(root, 0, vec);
    return root;
}

//
// solution 2): iterative breadth first search, O(1) space
//
Node* tree_next_right_v2(Node* root)
{
    Node dummy(-1, NULL, NULL, NULL), *prev, *cur;

    // Connect one level of nodes below root, then the next level, etc. Traverse
    // left to right using the next pointers we connected in the previous loops.
    // A 'dummy' node is used so that prev is always initialized and at the end
    // of a loop dummy.next points to the left-most node in the row below, which
    // we just connected.
    dummy.next = root;
    while (dummy.next) {
        cur        = dummy.next;
        dummy.next = NULL;
        for (prev = &dummy; cur; cur = cur->next) {
            if (cur->left)
                prev->next = cur->left, prev = cur->left;
            if (cur->right)
                prev->next = cur->right, prev = cur->right;
        }
    }
    return root;
}

// should be same as v2 but clearer without dummy thing
// TODO: create test cases and verify...
Node* tree_next_right_v3(Node* root)
{
    while (root) {
        Node* leftMost = nullptr;
        Node* leftNode = nullptr;
        while (root) {
            if (root->left) {
                if (!leftMost)
                    leftMost = root->left;
                if (leftNode)
                    leftNode->next = root->left;
                leftNode = root->left;
            }
            if (root->right) {
                if (!leftMost)
                    leftMost = root->right;
                if (leftNode)
                    leftNode->next = root->right;
                leftNode = root->right;
            }
            root = root->next;
        }
        root = leftMost;
    }
    return root;
}

//
// solution 4): C++ 100% time and 100% space
//
Node* tree_next_right_v4(Node* root)
{
    if (!root)
        return nullptr;

    // left node: first try assign right to left, if NULL then iterate through
    // next to try assign next->left or next->right
    if (root->left && (root->left->next = root->right) == nullptr) {
        for (auto next = root->next; next; next = next->next) {
            if ((root->left->next = next->left) != nullptr || (root->left->next = next->right) != nullptr)
                break;
        }
    }
    // right node: we just iterate through next to try assign next->left or
    // next->right
    if (root->right) {
        for (auto next = root->next; next; next = next->next) {
            if ((root->right->next = next->left) != nullptr || (root->right->next = next->right) != nullptr)
                break;
        }
    }

    // we must fill right first then left
    tree_next_right_v4(root->right);
    tree_next_right_v4(root->left);
    return root;
}

#endif
