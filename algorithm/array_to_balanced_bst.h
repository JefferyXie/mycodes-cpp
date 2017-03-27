#ifndef ARRAY_TO_BALANCED_BST_H
#define ARRAY_TO_BALANCED_BST_H

#include "../main/header.h"

// 
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

// 1) Get the Middle of the array and make it root.
// 2) Recursively do same for left half and right half.
//    a) Get the middle of left half and make it left child of the root
//       created in step 1.
//    b) Get the middle of right half and make it right child of the
//       root created in step 1. 

/* A Binary Tree node */
struct TNode
{
    int data;
    struct TNode* left;
    struct TNode* right;
};
 
struct TNode* newNode(int data);
 
/* A function that constructs Balanced Binary Search Tree from a sorted array */
struct TNode* array_to_balanced_bst(int arr[], int start, int end)
{
    /* Base Case */
    if (start > end)
      return NULL;
 
    /* Get the middle element and make it root */
    int mid = (start + end)/2;
    struct TNode *root = newNode(arr[mid]);
 
    /* Recursively construct the left subtree and make it
       left child of root */
    root->left = array_to_balanced_bst(arr, start, mid-1);
 
    /* Recursively construct the right subtree and make it
       right child of root */
    root->right = array_to_balanced_bst(arr, mid+1, end);
 
    return root;
}
 
/* Helper function that allocates a new node with the
   given data and NULL left and right pointers. */
struct TNode* newNode(int data)
{
    struct TNode* node = (struct TNode*)
                         malloc(sizeof(struct TNode));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
 
    return node;
}
 
/* A utility function to print preorder traversal of BST */
void preOrder(struct TNode* node)
{
    if (node == NULL)
        return;
    printf("%d ", node->data);
    preOrder(node->left);
    preOrder(node->right);
}
 
/* Driver program to test above functions */
void run_array_to_balanced_bst()
{
    int arr[] = {1, 2, 3, 4, 5, 6, 7};
    int n = sizeof(arr)/sizeof(arr[0]);
 
    /* Convert List to BST */
    struct TNode *root = array_to_balanced_bst(arr, 0, n-1);
    printf("\n PreOrder Traversal of constructed BST ");
    preOrder(root);
}

#endif

