#pragma once
#include <iostream>


template < class T>
struct TNode
{
	T		m_Value;
	TNode*	m_pNext;
};
template < class T>
class TLinkedList
{
public:
	TNode<T>* m_pHead;
	TNode<T>* m_pTail;
	TNode<T>* m_pEnd;
public:
	//void	Show();
	T		Pop();
	void   AddForwardLink(TNode<T>* pNew);
	void   AddBackwardLink(TNode<T>* pNew);
public:
	TLinkedList();
	~TLinkedList();
};

template < class T>
TLinkedList<T>::TLinkedList()
{
    m_pHead = new TNode<T>;    
    m_pHead->m_pNext = nullptr;

    m_pTail = new TNode<T>;    
    m_pTail->m_pNext = nullptr;
    m_pHead->m_pNext = m_pTail;

    m_pEnd = m_pHead;
}
template < class T>
TLinkedList<T>::~TLinkedList()
{
    for (TNode<T>* pNode = m_pHead->m_pNext;
        pNode != m_pTail;
        )
    {
        TNode<T>* pNext = pNode->m_pNext;
        delete pNode;
        pNode = pNext;
    }
    delete m_pHead;
    delete m_pTail;
}
template < class T>
void TLinkedList<T>::AddForwardLink(TNode<T>* pNew)
{
    TNode<T>* pTemp = m_pHead->m_pNext;
    m_pHead->m_pNext = pNew;
    pNew->m_pNext = pTemp;
}
template < class T>
void   TLinkedList<T>::AddBackwardLink(TNode<T>* pNew)
{
    m_pEnd->m_pNext = pNew;
    pNew->m_pNext = m_pTail;
    m_pEnd = pNew;
}
//template < class T>
//void TLinkedList<T>::Show()
//{
//    for (TNode<T>* pNode = m_pHead->m_pNext;
//        pNode != m_pTail;
//        pNode = pNode->m_pNext)
//    {
//        std::cout << pNode->m_Value.iIndex << "\n";
//    }
//}
template < class T>
T TLinkedList<T>::Pop()
{
    TNode<T>* pDelNode = m_pHead->m_pNext;
    T iRet = pDelNode->m_Value;
    m_pHead->m_pNext = pDelNode->m_pNext;
    delete pDelNode;
    return iRet;
}
