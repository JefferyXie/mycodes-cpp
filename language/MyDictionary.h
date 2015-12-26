#include "../header.h"
#include "../node.h"

template<class T> class MyDictionary
{
public:
    MyDictionary();
    virtual ~MyDictionary();
    MyDictionary(const MyDictionary& other);
    MyDictionary& operator=(const MyDictionary& other);

    TreeNode<T>* Add(const T& v);
    void Remove(const T& v);
    TreeNode<T>* Find(const T& v);

    void Display();
    int Size();

protected:
    TreeNode<T>* Add(const T& v, TreeNode<T>*& root);
    void Remove(const T& v, TreeNode<T>*& root);
    TreeNode<T>* Find(const T& v, TreeNode<T>* root);
    int Destroy(TreeNode<T>* root);

private:
    void _display(TreeNode<T>* root, int& num, string ext);
    int _size(TreeNode<T>* root = NULL);

private:
    TreeNode<T>* m_pRoot;
};

template<class T> MyDictionary<T>::MyDictionary()
{
    m_pRoot = NULL;
}

template<class T> MyDictionary<T>::~MyDictionary<T>()
{
    int num = Destroy(m_pRoot);
    cout << "~MyDictionary(): " << num << " TreeNode(s) are deleted." << endl;
}

template<class T> MyDictionary<T>::MyDictionary(const MyDictionary<T>& other)
{

}

template<class T> MyDictionary<T>& MyDictionary<T>::operator=(const MyDictionary<T>& other)
{
    return *this;
}

template<class T> TreeNode<T>* MyDictionary<T>::Add(const T& v)
{
    TreeNode<T>* pNode = Add(v, m_pRoot);
    if (NULL == pNode)
    {
        cout << "Add failed, duplicate value: " << v << endl;
    }
    return pNode;
}
template<class T> TreeNode<T>* MyDictionary<T>::Add(const T& v, TreeNode<T>*& root)
{
    TreeNode<T>* pNode = NULL;
    if (NULL == root)
    {
        root = new TreeNode<T>(v);
        pNode = root;
    }
    else
    {
        const T& nodeV = root->data;
        if (nodeV > v)
        {
            pNode = Add(v, root->left);
        }
        else if (nodeV < v)
        {
            pNode = Add(v, root->right);
        }
    }
    return pNode;
}

template<class T> TreeNode<T>* MyDictionary<T>::Find(const T& v)
{
    TreeNode<T>* pNode = Find(v, m_pRoot);
    if (NULL == pNode)
    {
        cout << "Find failed, no tree node found: " << v << endl;
    }
    return pNode;
}
template<class T> TreeNode<T>* MyDictionary<T>::Find(const T& v, TreeNode<T>* pStart)
{
    if (NULL == pStart) return NULL;

    TreeNode<T>* pNode = NULL;
    const T& treeV = pStart->data;
    if (treeV > v)
        pNode = Find(v, pStart->left);
    else if (treeV < v)
        pNode = Find(v, pStart->right);
    else if (treeV == v)
        pNode = pStart;

    return pNode;
}

template<class T> void MyDictionary<T>::Remove(const T& v)
{
    Remove(v, m_pRoot);
}
template<class T> void MyDictionary<T>::Remove(const T& v, TreeNode<T>*& pStart)
{
    if (NULL == pStart) return;

    if (pStart->data > v)
        Remove(v, pStart->left);
    else if (pStart->data < v)
        Remove(v, pStart->right);
    else
    {
        if (pStart->left == NULL && pStart->right == NULL)
        {
            delete pStart;
            pStart = NULL;
        }
        else
        {
            TreeNode<T>* temp = NULL;
            if (pStart->left != NULL)
            {
                temp = pStart->left;
                while (temp->right != NULL) temp = temp->right;
            }
            else
            {
                temp = pStart->right;
                while (temp->left != NULL) temp = temp->left;
            }

            T tempV = temp->data;
            Remove(tempV, pStart);
            pStart->data = tempV;
        }
    }
}

template<class T> void MyDictionary<T>::Display()
{
    cout << "Dictionary size is " << Size() << endl;
    int num = 0;
    _display(m_pRoot, num, "root");
}

template<class T> void MyDictionary<T>::_display(TreeNode<T>* root, int& num, string ext)
{
    if (NULL != root)
    {
        cout << ++num << "'s node value(" << ext << "): " << root->data << endl;
        _display(root->left, num, "left");
        _display(root->right, num, "right");
    }
}

template<class T> int MyDictionary<T>::Size()
{
    return _size(m_pRoot);
}
template<class T> int MyDictionary<T>::_size(TreeNode<T>* root)
{
    if (NULL == root) return 0;
    int len = 1;
    len += _size(root->left);
    len += _size(root->right);
    return len;
}

template<class T> int MyDictionary<T>::Destroy(TreeNode<T>* root)
{
    int num = 0;
    if (NULL != root)
    {
        num += Destroy(root->left);
        num += Destroy(root->right);
        delete root;
        root = NULL;
        ++num;
    }
    return num;
}

