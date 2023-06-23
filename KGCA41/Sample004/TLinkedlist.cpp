#include "TLinkedlist.h"
int  TLinkedlist::m_iCounter=0;
//void  TLinkedlist::Set2(const TLinkedlist* list)
//{
//    const TLinkedlist* list = this;
//    TLinkedlist* listTemp = const_cast<TLinkedlist*>(list);
//    listTemp->m_pEndNode = NULL;
//}
void  TLinkedlist::Set(const TLinkedlist* list)
{
    TLinkedlist* listTemp = const_cast<TLinkedlist*>(list);
    listTemp->m_pEndNode = NULL;
}
bool TLinkedlist::Init()
{
    m_pEndNode = &m_pHead;   
    m_pHead.m_pNext = NULL;
    m_pHead.m_pPrev = NULL;
    return true;
}
bool TLinkedlist::Release()
{
    TBox* pNode = m_pHead.m_pNext;
    while (pNode != NULL)
    {
        TBox* pNext = pNode->m_pNext;       
        delete pNode;
        pNode = nullptr;
        pNode = pNext;
    }
    return true;
}
// 1���� ��� ���� �� �ʱ�ȭ
TBox* TLinkedlist::NewNode()
{
    // ������ ����
    TBox* node = new TBox;
    node->m_pData = NULL;
    node->m_pNext = NULL;
    m_iCounter++;
    return node;
}
void TLinkedlist::Push_Back(TBox* pNewNode)
{
    // Head -> newNode
    m_pEndNode->m_pNext = pNewNode;
    // newNode -> Head
    pNewNode->m_pPrev = m_pEndNode;

    m_pEndNode = pNewNode;
}
void TLinkedlist::ForwardPrint() const
{
    //error C3490: 'm_pEndNode'��(��) const ��ü�� ���� �׼����ǰ� �����Ƿ� ������ �� �����ϴ�.
    //m_pEndNode = NULL;
    //m_iCounter = 0;
    //Set(this);
    // ��ȸ
    for (TBox* pNode = m_pHead.m_pNext;
        pNode != NULL;
        pNode = pNode->m_pNext)
    {
        pNode->m_pData->Print();
        //// ���
        //std::cout <<
        //    pNode->m_iID << " " <<
        //    pNode->m_iKor << " " <<
        //    pNode->m_iEng << " " <<
        //    pNode->m_iMat << std::endl;
    }
}
void TLinkedlist::BackwardPrint()const
{
    // ��ȸ
    for (TBox* pNode = m_pEndNode;
        pNode != &m_pHead;
        pNode = pNode->m_pPrev)
    {
        pNode->m_pData->Print();
    }
}
void TLinkedlist::ForwardPrint(TBox* pStartNode)const
{
    if (pStartNode == NULL)
    {
        //std::cout << "�ش��ϴ� id�� �˻����� ���߽��ϴ�." << std::endl;
        return;
    }
    // ��ȸ
    for (TBox* pNode = pStartNode;
        pNode != NULL;
        pNode = pNode->m_pNext)
    {
        pNode->m_pData->Print();
        // ���
        //std::cout << pNode->iValue << " ";
    }
    //std::cout << std::endl;
}
void TLinkedlist::BackwardPrint(TBox* pStartNode)const
{
    if (pStartNode == NULL) return;
    // ��ȸ
    for (TBox* pNode = pStartNode;
        pNode != &m_pHead;
        pNode = pNode->m_pPrev)
    {
        pNode->m_pData->Print();
    }
    //std::cout << std::endl;
}
void TLinkedlist::Push_Front(TBox* pNewNode)
{
    TBox* m_pNext = NULL;
    if (m_pHead.m_pNext != NULL)
    {
        m_pNext = m_pHead.m_pNext;
    }
    //  head -> newNode
    m_pHead.m_pNext = pNewNode;
    pNewNode->m_pNext = m_pNext;
    // newNode -> Head
    // m_pNext->m_pPrev = pNewNode
    pNewNode->m_pPrev = &m_pHead;
    if (m_pNext != NULL)
    {
        m_pNext->m_pPrev = pNewNode;
    }

    if (m_pEndNode == &m_pHead)
    {
        m_pEndNode = pNewNode;
    }
}
const TBox* TLinkedlist::Find(int id)
{
    TBox* pFindNode = NULL;
    for (TBox* pNode = m_pHead.m_pNext;
        pNode != NULL;
        pNode = pNode->m_pNext)
    {
        //todo
        if (pNode->m_pData->m_iID == id)
        {
            pFindNode = pNode;
            break;
        }
    }
    return pFindNode;
}
