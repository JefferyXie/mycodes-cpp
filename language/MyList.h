#include "../main/header.h"
#include "../main/node.h"

template<class T> class MyList
{
public:
    MyList();
    virtual ~MyList();
    MyList(const MyList& other);
    MyList& operator=(const MyList& other);

    ListNode<T>* Insert(ListNode<T>* pBefore, T& v);
    ListNode<T>* Insert2Head(T& v);
    ListNode<T>* Insert2End(T& v);
    ListNode<T>* Add(T& v);
    void Remove(T& v);
    ListNode<T>* Find(T& v);
    ListNode<T>* Reverse();
    int Size();
    void Display();

protected:
    ListNode<T>* GetCurrent() { return m_pCurrent; }

private:
    ListNode<T>* m_pHead;
    ListNode<T>* m_pCurrent;
};

template<class T> MyList<T>::MyList()
{
    m_pHead = NULL;
    m_pCurrent = NULL;
}

template<class T> MyList<T>::~MyList()
{
    ListNode<T>* pNode = m_pHead;
    while (NULL != pNode)
    {
        ListNode<T>* pTemp = pNode;
        pNode = pNode->GetPNext();
        delete pTemp;
    }
}

template<class T> MyList<T>::MyList(const MyList<T>& other)
{
    if (this != &other)
    {
        m_pHead = other.GetHead();
        m_pCurrent = other.GetCurrent();
    }
}

template<class T> MyList<T>& MyList<T>::operator=(const MyList<T>& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    m_pHead = rhs.GetHead();
    m_pCurrent = rhs.GetCurrent();
    return *this;
}

template<class T> ListNode<T>* MyList<T>::Insert(ListNode<T>* pBefore, T& v)
{
    ListNode<T>* pNew = new ListNode<T>(v, NULL);
    if (NULL != pBefore)
    {
        ListNode<T>* pNext = pBefore->GetPNext();
        pNew->SetPNext(pNext);
        pBefore->SetPNext(pNew);
    }
    else
    {
        m_pHead = pNew;
        m_pCurrent = pNew;
    }
    return pNew;
}


template<class T> ListNode<T>* MyList<T>::Insert2Head(T& v)
{
    m_pHead = new ListNode<T>(v, m_pHead);
    return m_pHead;
}

template<class T> ListNode<T>* MyList<T>::Insert2End(T& v)
{
    ListNode<T>* pTail = m_pCurrent;
    while (NULL != pTail && NULL != pTail->GetPNext())
    {
        pTail = pTail->GetPNext();
    }
    return Insert(pTail, v);
}

template<class T> ListNode<T>* MyList<T>::Add(T& v)
{
    return Insert2End(v);
}

template<class T> void MyList<T>::Remove(T& v)
{
    ListNode<T>* p = m_pHead;
    ListNode<T>* pPre = NULL;
    while (NULL != p)
    {
        ListNode<T>* pNext = p->GetPNext();
        if (p->GetValue() == v)
        {
            if (pPre != NULL)
            {
                pPre->SetPNext(pNext);
            }
            else
            {
                m_pHead = pNext;
            }
            delete p;
        }
        else
        {
            pPre = p;
        }
        p = pNext;
    }
}

template<class T> ListNode<T>* MyList<T>::Find(T& v)
{
    ListNode<T>* p = m_pHead;
    while (NULL != p)
    {
        if (p->GetValue() == v)
            break;
        p = p->GetPNext();
    }
    return p;
}

template<class T> ListNode<T>* MyList<T>::Reverse()
{
    ListNode<T>* p = m_pHead;
    ListNode<T>* pOldPre = NULL;
    ListNode<T>* pOldNext = NULL;
    while (NULL != p)
    {
        pOldNext = p->GetPNext();
        p->SetPNext(pOldPre);
        pOldPre = p;
        p = pOldNext;
    }
    m_pHead = pOldPre;
    return m_pHead;
}

template<class T> int MyList<T>::Size()
{
    int len = 0;
    ListNode<T>* p = m_pHead;
    while (NULL != p)
    {
        p = p->GetPNext();
        ++len;
    }
    return len;
}

template<class T> void MyList<T>::Display()
{
    cout << "List size is " << Size() << endl;
    int i = 0;
    ListNode<T>* p = m_pHead;
    while (NULL != p)
    {
        cout << i++ << "  : " << p->GetValue() << endl;
        p = p->GetPNext();
    }
}


