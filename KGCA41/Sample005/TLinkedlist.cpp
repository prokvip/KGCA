#include "TLinkedlist.h"
//template<class T>
//int  TLinkedlist<T>::m_iCounter=0;
////void  TLinkedlist::Set2(const TLinkedlist* list)
////{
////    const TLinkedlist* list = this;
////    TLinkedlist* listTemp = const_cast<TLinkedlist*>(list);
////    listTemp->m_pEndNode = NULL;
////}
//template<class T>
//void  TLinkedlist<T>::Set(const TLinkedlist* list)
//{
//    TLinkedlist* listTemp = const_cast<TLinkedlist*>(list);
//    listTemp->m_pEndNode = NULL;
//}
//template<class T>
//bool TLinkedlist<T>::Init()
//{
//    m_pEndNode = &m_pHead;   
//    m_pHead.m_pNext = NULL;
//    m_pHead.m_pPrev = NULL;
//    return true;
//}
//template<class T>
//bool TLinkedlist<T>::Release()
//{
//    T* pNode = m_pHead.m_pNext;
//    while (pNode != NULL)
//    {
//        T* pNext = pNode->m_pNext;       
//        delete pNode;
//        pNode = nullptr;
//        pNode = pNext;
//    }
//    return true;
//}
//template<class T>
//// 1���� ��� ���� �� �ʱ�ȭ
//T* TLinkedlist<T>::NewNode()
//{
//    // ������ ����
//    T* node = new T;
//    node->m_pData = NULL;
//    node->m_pNext = NULL;
//    m_iCounter++;
//    return node;
//}
//template<class T>
//void TLinkedlist<T>::Push_Back(T* pNewNode)
//{
//    // Head -> newNode
//    m_pEndNode->m_pNext = pNewNode;
//    // newNode -> Head
//    pNewNode->m_pPrev = m_pEndNode;
//
//    m_pEndNode = pNewNode;
//}
//template<class T>
//void TLinkedlist<T>::ForwardPrint() const
//{
//    //error C3490: 'm_pEndNode'��(��) const ��ü�� ���� �׼����ǰ� �����Ƿ� ������ �� �����ϴ�.
//    //m_pEndNode = NULL;
//    //m_iCounter = 0;
//    //Set(this);
//    // ��ȸ
//    for (T* pNode = m_pHead.m_pNext;
//        pNode != NULL;
//        pNode = pNode->m_pNext)
//    {
//        pNode->m_pData->Print();
//        //// ���
//        //std::cout <<
//        //    pNode->m_iID << " " <<
//        //    pNode->m_iKor << " " <<
//        //    pNode->m_iEng << " " <<
//        //    pNode->m_iMat << std::endl;
//    }
//}
//template<class T>
//void TLinkedlist<T>::BackwardPrint()const
//{
//    // ��ȸ
//    for (T* pNode = m_pEndNode;
//        pNode != &m_pHead;
//        pNode = pNode->m_pPrev)
//    {
//        pNode->m_pData->Print();
//    }
//}
//template<class T>
//void TLinkedlist<T>::ForwardPrint(T* pStartNode)const
//{
//    if (pStartNode == NULL)
//    {
//        //std::cout << "�ش��ϴ� id�� �˻����� ���߽��ϴ�." << std::endl;
//        return;
//    }
//    // ��ȸ
//    for (T* pNode = pStartNode;
//        pNode != NULL;
//        pNode = pNode->m_pNext)
//    {
//        pNode->m_pData->Print();
//        // ���
//        //std::cout << pNode->iValue << " ";
//    }
//    //std::cout << std::endl;
//}
//template<class T>
//void TLinkedlist<T>::BackwardPrint(T* pStartNode)const
//{
//    if (pStartNode == NULL) return;
//    // ��ȸ
//    for (T* pNode = pStartNode;
//        pNode != &m_pHead;
//        pNode = pNode->m_pPrev)
//    {
//        pNode->m_pData->Print();
//    }
//    //std::cout << std::endl;
//}
//template<class T>
//void TLinkedlist<T>::Push_Front(T* pNewNode)
//{
//    T* m_pNext = NULL;
//    if (m_pHead.m_pNext != NULL)
//    {
//        m_pNext = m_pHead.m_pNext;
//    }
//    //  head -> newNode
//    m_pHead.m_pNext = pNewNode;
//    pNewNode->m_pNext = m_pNext;
//    // newNode -> Head
//    // m_pNext->m_pPrev = pNewNode
//    pNewNode->m_pPrev = &m_pHead;
//    if (m_pNext != NULL)
//    {
//        m_pNext->m_pPrev = pNewNode;
//    }
//
//    if (m_pEndNode == &m_pHead)
//    {
//        m_pEndNode = pNewNode;
//    }
//}
//template<class T>
//const T* TLinkedlist<T>::Find(int id)
//{
//    T* pFindNode = NULL;
//    for (T* pNode = m_pHead.m_pNext;
//        pNode != NULL;
//        pNode = pNode->m_pNext)
//    {
//        //todo
//        if (pNode->m_pData->m_iID == id)
//        {
//            pFindNode = pNode;
//            break;
//        }
//    }
//    return pFindNode;
//}
