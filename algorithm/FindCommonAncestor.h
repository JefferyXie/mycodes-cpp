#ifndef FINDCOMMONANCESTOR_H
#define FINDCOMMONANCESTOR_H

#include "../main/header.h"
#include "../main/node.h"

// http://stackoverflow.com/questions/1484473/how-to-find-the-lowest-common-ancestor-of-two-nodes-in-any-binary-tree
template<class T> TreeNode<T>* FindCommonAncestor(TreeNode<T>* root, TreeNode<T>* n1, TreeNode<T>* n2) {
    // if root is a or b, it is the common ancestor
    if (root == nullptr || root == n1 || root == n2) return root;

    auto left = FindCommonAncestor(root->left, n1, n2);
    auto right = FindCommonAncestor(root->right, n1, n2);

    // if both nodes lie in left or right, the common ancestor should be in left branch
    // or right branch. Otherwise, root is the ancestor. This happens when @left is @n1
    // and @right is @n2.
    if (left != nullptr && right != nullptr) return root;

    return left != nullptr ? left : right;
}

#endif

