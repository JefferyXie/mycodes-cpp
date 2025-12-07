#pragma once

#include "../core/header.h"

//
// http://www.geeksforgeeks.org/find-a-pair-with-given-sum-in-bst/
//
// Given a Balanced Binary Search Tree and a target sum, write a function that returns
// true if there is a pair with sum equals to target sum, otherwise return false.
// Expected time complexity is O(n) and only O(Logn) extra space can be used.
//
// The Brute Force Solution is to consider each pair in BST and check whether the sum
// equals to X. The time complexity of this solution will be O(n^2).
//
// A Better Solution is to create an auxiliary array and store Inorder traversal of BST
// in the array. The array will be sorted as Inorder traversal of BST always produces
// sorted data. Once we have the Inorder traversal, we can pair in O(n) time (See this
// for details). This solution works in O(n) time, but requires O(n) auxiliary space.
//
// A space optimized solution is discussed in previous post. The idea was to first in-place
// convert BST to Doubly Linked List (DLL), then find pair in sorted DLL in O(n) time.
// This solution takes O(n) time and O(Logn) extra space, but it modifies the given BST.
//
// The solution discussed below takes O(n) time, O(Logn) space and doesn’t modify BST.
// The idea is same as finding the pair in sorted array (See method 1 of this for details).
// We traverse BST in Normal Inorder and Reverse Inorder simultaneously. In reverse inorder,
// we start from the rightmost node which is the maximum value node. In normal inorder, we
// start from the left most node which is minimum value node. We add sum of current nodes
// in both traversals and compare this sum with given target sum. If the sum is same as
// target sum, we return true. If the sum is more than target sum, we move to next node
// in reverse inorder traversal, otherwise we move to next node in normal inorder traversal.
// If any of the traversals is finished without finding a pair, we return false.
//

struct node {
    int          val;
    struct node *left, *right;
};

struct node* NewNode(int val)
{
    struct node* tmp = (struct node*)malloc(sizeof(struct node));
    tmp->val         = val;
    tmp->right = tmp->left = NULL;
    return tmp;
}

bool sum_pair_target_bst(struct node* root, int sum)
{
    if (!root)
        return false;

    std::stack<node*> S1, S2;
    node*             curr1 = root;
    node*             curr2 = root;
    while (true) {
        if (curr1) {
            // nodes in S1 are ordered smallest nodes
            S1.push(curr1);
            curr1 = curr1->left;
        } else if (curr2) {
            // nodes in S1 are ordered largest nodes
            S2.push(curr2);
            curr2 = curr2->right;
        } else if (!S1.empty() && !S2.empty()) {
            curr1 = S1.top();
            curr2 = S2.top();
            if (curr1->val + curr2->val == sum) {
                std::cout << curr1->val << "+" << curr2->val << "=" << sum;
                return true;
            } else if (curr1->val + curr2->val > sum) {
                S2.pop();
                curr2 = curr2->left;
                curr1 = nullptr;
            } else {
                S1.pop();
                curr1 = curr1->right;
                curr2 = nullptr;
            }
        } else
            break;
    }
    return false;
}

void run_sum_pair_target_bst()
{
    /*
            15
         /      \
       10       20
       / \     /  \
      8  12   16  25
    */
    struct node* root  = NewNode(15);
    root->left         = NewNode(10);
    root->right        = NewNode(20);
    root->left->left   = NewNode(8);
    root->left->right  = NewNode(12);
    root->right->left  = NewNode(16);
    root->right->right = NewNode(25);

    int target = 33;
    if (sum_pair_target_bst(root, target) == false)
        printf("\n No such values are found\n");
}

