#ifndef TRAVERSETREE_H
#define TRAVERSETREE_H

#include "../header.h"

struct tNode
{
    int data;
    tNode* left;
    tNode* right;
    tNode(int d) : data(d), left(NULL), right(NULL) {}
};

// http://stackoverflow.com/questions/1294701/post-order-traversal-of-binary-tree-without-recursion
// 1), keep pushing right and left children nodes to stack
// 2), in each loop, pick top node - left node added in step 1), and do 1) again
// 3), if no children or all children branches have been visited, pop this node
// 4), keep looping until stack is empty
void TraverseInorder(tNode* root) {
    if (root == 0) return;

    cout << "Traverse tree inorder (left-right-parent): " << endl;
    std::stack<tNode*> oStack;
    oStack.push(root);

    tNode* prior = root;
    while (!oStack.empty()) {
        tNode* current = oStack.top();

        bool finishedSubtrees = (current->right == prior || current->left == prior);
        bool isLeaf = (current->left == 0 && current->right == 0);
        if (finishedSubtrees || isLeaf) {
            oStack.pop();
            cout << current->data << " ";
            prior = current;
        }
        else {
            if (current->right != NULL) {
                oStack.push(current->right);
            }
            if (current->left != NULL) {
                oStack.push(current->left);
            }
        }
    }
    cout << endl;
};

// http://www.geeksforgeeks.org/inorder-tree-traversal-without-recursion/
// 1) Create an empty stack.
// 2) Initialize current node as root
// 3) Push the current node to stack and set current = current->left until current is NULL
// 4) If current is NULL and stack is not empty then 
//      a) Pop the top item from stack.
//      b) Print the popped item, set current = popped_item->right 
//      c) Go to step 3.
// 5) If current is NULL and stack is empty then we are done.
void TraversePostorder(tNode* root) {
    if (root == NULL) return;

    cout << "Traverse tree postorder (left-parent-right): " << endl;

    tNode *current = root;
    std::stack<tNode*> oStack;
    while (true) {
        /* Reach the left most tNode of the current tNode */
        if(current !=  NULL) {
            /* place pointer to a tree node on the stack before traversing 
             * the node's left subtree */
            oStack.push(current);
            current = current->left;  
        }

        /* backtrack from the empty subtree and visit the tNode 
         *  at the top of the stack; however, if the stack is empty,
         *  you are done */
        else {
            if (oStack.empty()) break; 

            current = oStack.top();
            oStack.pop();
            cout << current->data << " ";

            /* we have visited the node and its left subtree.
             *  Now, it's right subtree's turn */
            current = current->right;
        }
    } /* end of while */ 
    cout << endl;
};

#endif

