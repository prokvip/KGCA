#pragma once
//#define _CRT_SECURE_NO_WARNINGS
/*
*  ���� ���Ḯ��Ʈ && ����� ����
*  ���� ���Ḯ��Ʈ && ����� ����
*/
#include <iostream>
#include <conio.h>

struct TNode
{
    int   iID;
    int   iValue;
    TNode* pNext;
    TNode* pPrev;
};

TNode  g_pHead; // �Ӹ� ( ��Ӹ� )
TNode* g_pEndNode=NULL;
int    g_iCounter=0;

TNode* NewNode();
void Push_Back(TNode* pNewNode);
void Push_Front(TNode* pNewNode);
TNode* Find(int id);
void ForwardPrint();
void BackwardPrint();
void ForwardPrint(TNode* pNode);
void BackwardPrint(TNode* pNode);


// 1���� ��� ���� �� �ʱ�ȭ
TNode* NewNode()
{
    // ������ ����
    TNode* node  = (TNode*)malloc(sizeof(TNode));
    node->iID    = g_iCounter++;
    node->iValue = g_iCounter * 10;
    node->pNext = NULL;
    return node;
}
void Push_Back(TNode* pNewNode)
{
    // Head -> newNode
    g_pEndNode->pNext = pNewNode;
    // newNode -> Head
    pNewNode->pPrev = g_pEndNode;

    g_pEndNode = pNewNode;
}
void ForwardPrint()
{
    // ��ȸ
    for (TNode* pNode = g_pHead.pNext;
        pNode != NULL;
        pNode = pNode->pNext)
    {
        // ���
        std::cout << pNode->iValue << " ";
    }
}
void BackwardPrint()
{
    // ��ȸ
    for (TNode* pNode = g_pEndNode;
        pNode != &g_pHead;
        pNode = pNode->pPrev)
    {
        // ���
        //std::cout << pNode->iValue << " ";
    }
}
void ForwardPrint(TNode* pStartNode)
{
    if (pStartNode == NULL)
    {
        //std::cout << "�ش��ϴ� id�� �˻����� ���߽��ϴ�." << std::endl;
        return;
    }
    // ��ȸ
    for (TNode* pNode = pStartNode;
        pNode != NULL;
        pNode = pNode->pNext)
    {
        // ���
        //std::cout << pNode->iValue << " ";
    }
    //std::cout << std::endl;
}
void BackwardPrint(TNode* pStartNode)
{
    if (pStartNode == NULL) return;
    // ��ȸ
    for (TNode* pNode = pStartNode;
        pNode != &g_pHead;
        pNode = pNode->pPrev)
    {
        // ���
        //std::cout << pNode->iValue << " ";
    }
    //std::cout << std::endl;
}
void Push_Front(TNode* pNewNode)
{
    TNode* pNext = NULL;
    if (g_pHead.pNext != NULL)
    {
        pNext = g_pHead.pNext;
    }
    //  head -> newNode
    g_pHead.pNext = pNewNode;
    pNewNode->pNext = pNext;
    // newNode -> Head
    // pNext->pPrev = pNewNode
    pNewNode->pPrev = &g_pHead;
    if (pNext != NULL)
    {
        pNext->pPrev = pNewNode;
    }

    if (g_pEndNode == &g_pHead)
    {
        g_pEndNode = pNewNode;
    }
}
TNode* Find(int id)
{
    TNode* pFindNode = NULL;
    for (TNode* pNode = g_pHead.pNext;
        pNode != NULL;
        pNode = pNode->pNext)
    {
        if (pNode->iID == id)
        {
            pFindNode = pNode;
            break;
        }
    }
    return pFindNode;
}