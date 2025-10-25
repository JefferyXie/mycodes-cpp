#ifndef NODE_H
#define NODE_H
#include "header.h"

template <class T>
struct tree_node {
    T             data{};
    tree_node<T>* left{nullptr};
    tree_node<T>* right{nullptr};
};

template <typename T>
using TreeNode        = tree_node<T>;
using tree_node_int_t = tree_node<int>;

template <class T>
struct list_node {
    T             data{};
    list_node<T>* next{nullptr};
};

template <typename T>
using ListNode        = list_node<T>;
using list_node_int_t = list_node<int>;

#endif
