#ifndef MY_LIST_H
#define MY_LIST_H

#include "../main/header.h"
#include "../main/node.h"

template <class T>
class my_list
{
public:
    my_list();
    virtual ~my_list();
    my_list(const my_list& other);
    my_list& operator=(const my_list& other);

    list_node_t<T>* Insert(list_node_t<T>* pBefore, T& v);
    list_node_t<T>* Insert2Head(T& v);
    list_node_t<T>* Insert2End(T& v);
    list_node_t<T>* Add(T& v);
    void            Remove(T& v);
    list_node_t<T>* Find(T& v);
    list_node_t<T>* Reverse();
    list_node_t<T>* GetHead() const;
    int             Size();
    void            Display();

protected:
    list_node_t<T>* GetCurrent() { return m_pCurrent; }

private:
    list_node_t<T>* m_pHead;
    list_node_t<T>* m_pCurrent;
};

template <class T>
my_list<T>::my_list()
{
    m_pHead    = NULL;
    m_pCurrent = NULL;
}

template <class T>
my_list<T>::~my_list()
{
    list_node_t<T>* pNode = m_pHead;
    while (NULL != pNode) {
        list_node_t<T>* pTemp = pNode;
        pNode                 = pNode->next;
        delete pTemp;
    }
}

template <class T>
my_list<T>::my_list(const my_list<T>& other)
{
    if (this != &other) {
        m_pHead    = other.GetHead();
        m_pCurrent = other.GetCurrent();
    }
}

template <class T>
my_list<T>& my_list<T>::operator=(const my_list<T>& rhs)
{
    if (this == &rhs)
        return *this;    // handle self assignment
    m_pHead    = rhs.GetHead();
    m_pCurrent = rhs.GetCurrent();
    return *this;
}

template <class T>
list_node_t<T>* my_list<T>::Insert(list_node_t<T>* pBefore, T& v)
{
    list_node_t<T>* pNew = new list_node_t<T>(v, NULL);
    if (NULL != pBefore) {
        list_node_t<T>* pNext = pBefore->next;
        pNew->next            = pNext;
        pBefore->next         = pNew;
    } else {
        m_pHead    = pNew;
        m_pCurrent = pNew;
    }
    return pNew;
}

template <class T>
list_node_t<T>* my_list<T>::Insert2Head(T& v)
{
    m_pHead = new list_node_t<T>(v, m_pHead);
    return m_pHead;
}

template <class T>
list_node_t<T>* my_list<T>::Insert2End(T& v)
{
    list_node_t<T>* pTail = m_pCurrent;
    while (NULL != pTail && NULL != pTail->next) {
        pTail = pTail->next;
    }
    return Insert(pTail, v);
}

template <class T>
list_node_t<T>* my_list<T>::Add(T& v)
{
    return Insert2End(v);
}

template <class T>
void my_list<T>::Remove(T& v)
{
    list_node_t<T>* p    = m_pHead;
    list_node_t<T>* pPre = NULL;
    while (NULL != p) {
        list_node_t<T>* pNext = p->next;
        if (p->data == v) {
            if (pPre != NULL) {
                pPre->next = pNext;
            } else {
                m_pHead = pNext;
            }
            delete p;
        } else {
            pPre = p;
        }
        p = pNext;
    }
}

template <class T>
list_node_t<T>* my_list<T>::Find(T& v)
{
    list_node_t<T>* p = m_pHead;
    while (NULL != p) {
        if (p->data == v)
            break;
        p = p->next;
    }
    return p;
}

template <class T>
list_node_t<T>* my_list<T>::Reverse()
{
    list_node_t<T>* p        = m_pHead;
    list_node_t<T>* pOldPre  = NULL;
    list_node_t<T>* pOldNext = NULL;
    while (NULL != p) {
        pOldNext = p->next;
        p->next  = pOldPre;
        pOldPre  = p;
        p        = pOldNext;
    }
    m_pHead = pOldPre;
    return m_pHead;
}

template <class T>
int my_list<T>::Size()
{
    int             len = 0;
    list_node_t<T>* p   = m_pHead;
    while (NULL != p) {
        p = p->next;
        ++len;
    }
    return len;
}
template <class T>
list_node_t<T>* my_list<T>::GetHead() const
{
    return m_pHead;
}

template <class T>
void my_list<T>::Display()
{
    std::cout << "List size is " << Size() << std::endl;
    int             i = 0;
    list_node_t<T>* p = m_pHead;
    while (NULL != p) {
        std::cout << i++ << "  : " << p->data << std::endl;
        p = p->next;
    }
}

#endif

