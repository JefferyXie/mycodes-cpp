#ifndef MY_DICTITONARY_H
#define MY_DICTITONARY_H

#include "../main/header.h"
#include "../main/node.h"

template <class T>
class my_dictionary
{
public:
    my_dictionary();
    virtual ~my_dictionary();
    my_dictionary(const my_dictionary& other);
    my_dictionary& operator=(const my_dictionary& other);

    TreeNode<T>* Add(const T& v);
    void         Remove(const T& v);
    TreeNode<T>* Find(const T& v);

    void Display();
    int  Size();

protected:
    TreeNode<T>* Add(const T& v, TreeNode<T>*& root);
    void         Remove(const T& v, TreeNode<T>*& root);
    TreeNode<T>* Find(const T& v, TreeNode<T>* root);
    int          Destroy(TreeNode<T>* root);

private:
    void _display(TreeNode<T>* root, int& num, string ext);
    int  _size(TreeNode<T>* root = NULL);

private:
    TreeNode<T>* m_pRoot;
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
    cout << "~my_dictionary(): " << num << " TreeNode(s) are deleted." << endl;
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
TreeNode<T>* my_dictionary<T>::Add(const T& v)
{
    TreeNode<T>* pNode = Add(v, m_pRoot);
    if (NULL == pNode) {
        cout << "Add failed, duplicate value: " << v << endl;
    }
    return pNode;
}
template <class T>
TreeNode<T>* my_dictionary<T>::Add(const T& v, TreeNode<T>*& root)
{
    TreeNode<T>* pNode = NULL;
    if (NULL == root) {
        root  = new TreeNode<T>(v);
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
TreeNode<T>* my_dictionary<T>::Find(const T& v)
{
    TreeNode<T>* pNode = Find(v, m_pRoot);
    if (NULL == pNode) {
        cout << "Find failed, no tree node found: " << v << endl;
    }
    return pNode;
}
template <class T>
TreeNode<T>* my_dictionary<T>::Find(const T& v, TreeNode<T>* pStart)
{
    if (NULL == pStart)
        return NULL;

    TreeNode<T>* pNode = NULL;
    const T&     treeV = pStart->data;
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
void my_dictionary<T>::Remove(const T& v, TreeNode<T>*& pStart)
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
            TreeNode<T>* temp = NULL;
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
    cout << "Dictionary size is " << Size() << endl;
    int num = 0;
    _display(m_pRoot, num, "root");
}

template <class T>
void my_dictionary<T>::_display(TreeNode<T>* root, int& num, string ext)
{
    if (NULL != root) {
        cout << ++num << "'s node value(" << ext << "): " << root->data << endl;
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
int my_dictionary<T>::_size(TreeNode<T>* root)
{
    if (NULL == root)
        return 0;
    int len = 1;
    len += _size(root->left);
    len += _size(root->right);
    return len;
}

template <class T>
int my_dictionary<T>::Destroy(TreeNode<T>* root)
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

#endif

