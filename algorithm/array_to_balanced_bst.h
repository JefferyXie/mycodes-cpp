#pragma once

#include "../core/header.h"

// clang-format off
/*
 * http://www.geeksforgeeks.org/sorted-array-to-balanced-bst/
 * Given a sorted array, create a Balanced Binary Search Tree using array elements.
 *
 * Examples :
 *
    Input:  Array {1, 2, 3}
    Output: A Balanced BST
            2
           /  \
          1    3

    Input: Array {1, 2, 3, 4}
    Output: A Balanced BST
            3
           /  \
          2    4
         /
        1
 *
 */
// clang-format on

// 1) Get the Middle of the array and make it root.
// 2) Recursively do same for left half and right half.
//    a) Get the middle of left half and make it left child of the root created in step 1.
//    b) Get the middle of right half and make it right child of the root created in step 1.

struct node_t {
    int            data;
    struct node_t* left;
    struct node_t* right;
};

node_t* array_to_balanced_bst(int arr[], int start, int end)
{
    if (start > end)
        return NULL;

    // Get the middle element and make it root
    const int mid  = (start + end) / 2;
    auto      root = (struct node_t*)malloc(sizeof(struct node_t));
    root->data     = arr[mid];
    root->left     = array_to_balanced_bst(arr, start, mid - 1);
    root->right    = array_to_balanced_bst(arr, mid + 1, end);
    return root;
}

void pre_order(struct node_t* node)
{
    if (node == NULL)
        return;
    printf("%d ", node->data);
    pre_order(node->left);
    pre_order(node->right);
}

void run_array_to_balanced_bst()
{
    int  arr[] = {1, 2, 3, 4, 5, 6, 7};
    int  n     = sizeof(arr) / sizeof(arr[0]);
    auto root  = array_to_balanced_bst(arr, 0, n - 1);
    printf("\n pre_order traversal of constructed BST ");
    pre_order(root);
}

