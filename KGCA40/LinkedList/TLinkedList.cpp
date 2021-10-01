#include "TLinkedList.h"
TLinkedList::TLinkedList()
{
    m_pHead = new TNode;
    m_pHead->m_iValue = 99999;
    m_pHead->m_pNext = nullptr;

    m_pTail = new TNode;
    m_pTail->m_iValue = -99999;
    m_pTail->m_pNext = nullptr;

    m_pHead->m_pNext = m_pTail;

    m_pEnd = m_pHead;
}
TLinkedList::~TLinkedList()
{	
    for (TNode* pNode = m_pHead->m_pNext;
        pNode != m_pTail;
        )
    {
        TNode* pNext = pNode->m_pNext;
        delete pNode;
        pNode = pNext;
    }
    delete m_pHead;
    delete m_pTail;
}
void TLinkedList::AddForwardLink(TNode* pNew)
{
    TNode* pTemp = m_pHead->m_pNext;
    m_pHead->m_pNext = pNew;
    pNew->m_pNext = pTemp;
}
void   TLinkedList::AddBackwardLink(TNode* pNew)
{    
    m_pEnd->m_pNext = pNew;
    pNew->m_pNext = m_pTail;
    m_pEnd = pNew;
}
void TLinkedList::Show()
{
    for (TNode* pNode = m_pHead->m_pNext;
        pNode != m_pTail;
        pNode = pNode->m_pNext)
    {
        std::cout << pNode->m_iValue << "\n";
    }
}

int TLinkedList::Pop()
{    
    TNode* pDelNode = m_pHead->m_pNext;
    int iRet = pDelNode->m_iValue;
    m_pHead->m_pNext = pDelNode->m_pNext;
    delete pDelNode;
    return iRet;
}
