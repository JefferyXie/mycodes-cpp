#ifndef BST_TO_BALANCED_BST_H
#define BST_TO_BALANCED_BST_H

#include "../main/header.h"

// 
/*
 * http://www.geeksforgeeks.org/convert-normal-bst-balanced-bst/
 * Given a BST (Binary Search Tree) that may be unbalanced, convert it into a balanced
 * BST that has minimum possible height.
 *
 * This solution (bst > sorted array > balanced bst) can also be applied to trees' merge!
 * http://www.geeksforgeeks.org/merge-two-balanced-binary-search-trees/
 * 
 * Examples :
 * 
	Input:
		   30
		  /
		 20
		/
	   10
	Output:
		 20
	   /   \
	 10     30

	Input:
			 4
			/
		   3
		  /
		 2
		/
	   1
	Output:
		  3            3           2
		/  \         /  \        /  \
	   1    4   OR  2    4  OR  1    3   OR ..
		\          /                   \
		 2        1                     4 

	Input:
			  4
			/   \
		   3     5
		  /       \
		 2         6 
		/           \
	   1             7
	Output:
		   4
		/    \
	   2      6
	 /  \    /  \
	1    3  5    7 
 * 
 */

//
// 1) Traverse given BST in inorder and store result in an array. This step takes O(n) time.
//    Note that this array would be sorted as inorder traversal of BST always produces sorted
//    sequence.
// 2) Build a balanced BST from the above created sorted array using the recursive approach
//    discussed in array_to_balanced_bst.h. This step also takes O(n) time as we traverse
//    every element exactly once and processing an element takes O(1) time.

struct Node
{
    int data;
    Node* left,  *right;
};
 
/* This function traverse the skewed binary tree and
   stores its nodes pointers in vector nodes[] */
void storeBSTNodes(Node* root, vector<Node*> &nodes)
{
    // Base case
    if (!root) return;
 
    // Store nodes in Inorder (which is sorted
    // order for BST)
    storeBSTNodes(root->left, nodes);
    nodes.push_back(root);
    storeBSTNodes(root->right, nodes);
}
 
/* Recursive function to construct binary tree */
Node* buildTreeUtil(vector<Node*> &nodes, int start,
                   int end)
{
    // base case
    if (start > end)
        return NULL;
 
    /* Get the middle element and make it root */
    int mid = (start + end)/2;
    Node *root = nodes[mid];
 
    /* Using index in Inorder traversal, construct
       left and right subtress */
    root->left  = buildTreeUtil(nodes, start, mid-1);
    root->right = buildTreeUtil(nodes, mid+1, end);
 
    return root;
}
 
// This functions converts an unbalanced BST to
// a balanced BST
Node* bst_to_balanced_bst(Node* root)
{
    // Store nodes of given BST in sorted order
    vector<Node *> nodes;
    storeBSTNodes(root, nodes);
 
    // Constucts BST from nodes[]
    int n = nodes.size();
    return buildTreeUtil(nodes, 0, n-1);
}
 
// Utility function to create a new node
Node* newNode(int data)
{
    Node* node = new Node;
    node->data = data;
    node->left = node->right = NULL;
    return (node);
}
 
/* Function to do preorder traversal of tree */
void preOrder(Node* node)
{
    if (node == NULL)
        return;
    printf("%d ", node->data);
    preOrder(node->left);
    preOrder(node->right);
}
 
// Driver program
void run_bst_to_balanced_bst()
{
    /* Constructed skewed binary tree is
                10
               /
              8
             /
            7
           /
          6
         /
        5   */
 
    Node* root = newNode(10);
    root->left = newNode(8);
    root->left->left = newNode(7);
    root->left->left->left = newNode(6);
    root->left->left->left->left = newNode(5);
 
    root = bst_to_balanced_bst(root);
 
    printf("Preorder traversal of balanced "
            "BST is : \n");
    preOrder(root);
}

#endif

