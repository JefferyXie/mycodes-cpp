#pragma once

#include "../core/node.h"

template <class T>
class my_dictionary
{
public:
    my_dictionary();
    virtual ~my_dictionary();
    my_dictionary(const my_dictionary& other);
    my_dictionary& operator=(const my_dictionary& other);

    tree_node_t<T>* Add(const T& v);
    void            Remove(const T& v);
    tree_node_t<T>* Find(const T& v);

    void Display();
    int  Size();

protected:
    tree_node_t<T>* Add(const T& v, tree_node_t<T>*& root);
    void            Remove(const T& v, tree_node_t<T>*& root);
    tree_node_t<T>* Find(const T& v, tree_node_t<T>* root);
    int             Destroy(tree_node_t<T>* root);

private:
    void _display(tree_node_t<T>* root, int& num, std::string ext);
    int  _size(tree_node_t<T>* root = NULL);

private:
    tree_node_t<T>* m_pRoot;
};

template <class T>
my_dictionary<T>::my_dictionary()
{
    m_pRoot = NULL;
}

template <class T>
my_dictionary<T>::~my_dictionary<T>()
{
    int num = Destroy(m_pRoot);
    std::cout << "~my_dictionary(): " << num << " tree_node_t(s) are deleted." << std::endl;
}

template <class T>
my_dictionary<T>::my_dictionary(const my_dictionary<T>& other)
{
}

template <class T>
my_dictionary<T>& my_dictionary<T>::operator=(const my_dictionary<T>& other)
{
    return *this;
}

template <class T>
tree_node_t<T>* my_dictionary<T>::Add(const T& v)
{
    tree_node_t<T>* pNode = Add(v, m_pRoot);
    if (NULL == pNode) {
        std::cout << "Add failed, duplicate value: " << v << std::endl;
    }
    return pNode;
}
template <class T>
tree_node_t<T>* my_dictionary<T>::Add(const T& v, tree_node_t<T>*& root)
{
    tree_node_t<T>* pNode = NULL;
    if (NULL == root) {
        root  = new tree_node_t<T>(v);
        pNode = root;
    } else {
        const T& nodeV = root->data;
        if (nodeV > v) {
            pNode = Add(v, root->left);
        } else if (nodeV < v) {
            pNode = Add(v, root->right);
        }
    }
    return pNode;
}

template <class T>
tree_node_t<T>* my_dictionary<T>::Find(const T& v)
{
    tree_node_t<T>* pNode = Find(v, m_pRoot);
    if (NULL == pNode) {
        std::cout << "Find failed, no tree node found: " << v << std::endl;
    }
    return pNode;
}
template <class T>
tree_node_t<T>* my_dictionary<T>::Find(const T& v, tree_node_t<T>* pStart)
{
    if (NULL == pStart)
        return NULL;

    tree_node_t<T>* pNode = NULL;
    const T&        treeV = pStart->data;
    if (treeV > v)
        pNode = Find(v, pStart->left);
    else if (treeV < v)
        pNode = Find(v, pStart->right);
    else if (treeV == v)
        pNode = pStart;

    return pNode;
}

template <class T>
void my_dictionary<T>::Remove(const T& v)
{
    Remove(v, m_pRoot);
}
template <class T>
void my_dictionary<T>::Remove(const T& v, tree_node_t<T>*& pStart)
{
    if (NULL == pStart)
        return;

    if (pStart->data > v)
        Remove(v, pStart->left);
    else if (pStart->data < v)
        Remove(v, pStart->right);
    else {
        if (pStart->left == NULL && pStart->right == NULL) {
            delete pStart;
            pStart = NULL;
        } else {
            tree_node_t<T>* temp = NULL;
            if (pStart->left != NULL) {
                temp = pStart->left;
                while (temp->right != NULL)
                    temp = temp->right;
            } else {
                temp = pStart->right;
                while (temp->left != NULL)
                    temp = temp->left;
            }

            T tempV = temp->data;
            Remove(tempV, pStart);
            pStart->data = tempV;
        }
    }
}

template <class T>
void my_dictionary<T>::Display()
{
    std::cout << "Dictionary size is " << Size() << std::endl;
    int num = 0;
    _display(m_pRoot, num, "root");
}

template <class T>
void my_dictionary<T>::_display(tree_node_t<T>* root, int& num, std::string ext)
{
    if (NULL != root) {
        std::cout << ++num << "'s node value(" << ext << "): " << root->data << std::endl;
        _display(root->left, num, "left");
        _display(root->right, num, "right");
    }
}

template <class T>
int my_dictionary<T>::Size()
{
    return _size(m_pRoot);
}
template <class T>
int my_dictionary<T>::_size(tree_node_t<T>* root)
{
    if (NULL == root)
        return 0;
    int len = 1;
    len += _size(root->left);
    len += _size(root->right);
    return len;
}

template <class T>
int my_dictionary<T>::Destroy(tree_node_t<T>* root)
{
    int num = 0;
    if (NULL != root) {
        num += Destroy(root->left);
        num += Destroy(root->right);
        delete root;
        root = NULL;
        ++num;
    }
    return num;
}

