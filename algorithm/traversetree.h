#ifndef TRAVERSETREE_H
#define TRAVERSETREE_H

#include "../main/header.h"
#include "../main/node.h"

// http://stackoverflow.com/questions/1294701/post-order-traversal-of-binary-tree-without-recursion
// 1), keep pushing right and left children nodes to stack
// 2), in each loop, pick top node - left node added in step 1), and do 1) again
// 3), if no children or all children branches have been visited, pop this node
// 4), keep looping until stack is empty
void TraverseInorder(tree_node_int_t* root)
{
    if (root == 0)
        return;

    std::cout << "Traverse tree inorder (left-right-parent): " << std::endl;
    std::stack<tree_node_int_t*> oStack;
    oStack.push(root);

    tree_node_int_t* prior = root;
    while (!oStack.empty()) {
        tree_node_int_t* current = oStack.top();

        bool finishedSubtrees = (current->right == prior || current->left == prior);
        bool isLeaf           = (current->left == 0 && current->right == 0);
        if (finishedSubtrees || isLeaf) {
            oStack.pop();
            std::cout << current->data << " ";
            prior = current;
        } else {
            if (current->right != NULL) {
                oStack.push(current->right);
            }
            if (current->left != NULL) {
                oStack.push(current->left);
            }
        }
    }
    std::cout << std::endl;
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
void TraversePostorder(tree_node_int_t* root)
{
    if (root == NULL)
        return;

    std::cout << "Traverse tree postorder (left-parent-right): " << std::endl;

    tree_node_int_t*             current = root;
    std::stack<tree_node_int_t*> oStack;
    while (true) {
        /* Reach the left most tree_node_int_t of the current tree_node_int_t */
        if (current != NULL) {
            /* place pointer to a tree node on the stack before traversing
             * the node's left subtree */
            oStack.push(current);
            current = current->left;
        }

        /* backtrack from the empty subtree and visit the tree_node_int_t
         *  at the top of the stack; however, if the stack is empty,
         *  you are done */
        else {
            if (oStack.empty())
                break;

            current = oStack.top();
            oStack.pop();
            std::cout << current->data << " ";

            /* we have visited the node and its left subtree.
             *  Now, it's right subtree's turn */
            current = current->right;
        }
    } /* end of while */
    std::cout << std::endl;
};

#endif

