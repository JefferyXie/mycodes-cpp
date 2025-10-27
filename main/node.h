#pragma once

#include "header.h"

template <class T>
struct tree_node_t {
    T               data{};
    tree_node_t<T>* left{nullptr};
    tree_node_t<T>* right{nullptr};
};

using tree_node_int_t = tree_node_t<int>;

template <class T>
struct list_node_t {
    T               data{};
    list_node_t<T>* next{nullptr};
};

using list_node_int_t = list_node_t<int>;

