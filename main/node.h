#ifndef NODE_H
#define NODE_H
#include "header.h"

template<class T> class TreeNode {
public:
   T data;
   TreeNode* left   = nullptr;
   TreeNode* right  = nullptr;
   
   TreeNode(const T& value) {
    data = value;
    left = nullptr;
    right = nullptr;
   }
   TreeNode(const T& value, TreeNode<T>* l, TreeNode<T>* r) : TreeNode(value) {
    left = l;
    right = r;
   }
};

template<class T> class ListNode
{
public:
    ListNode() : m_pNext(NULL) { }
    ListNode(T& v, ListNode<T>* p) : m_value(v), m_pNext(p) { }
    ListNode(const ListNode<T>& obj) : m_value(obj.m_value), m_pNext(obj.m_pNext) { }

    void SetValue(T& v) { m_value = v; }
    T GetValue() { return m_value; }
    void SetPNext(ListNode<T>* p) { m_pNext = p; }
    ListNode<T>* GetPNext() { return m_pNext; }

private:
    T m_value;
    ListNode<T>* m_pNext;
};

#endif

