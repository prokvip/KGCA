#pragma once
//#define _CRT_SECURE_NO_WARNINGS
/*
*  후위 연결리스트 && 양방향 구현
*  전위 연결리스트 && 양방향 구현
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

TNode  g_pHead; // 머리 ( 빈머리 )
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


// 1개의 노드 생성 및 초기화
TNode* NewNode()
{
    // 데이터 생성
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
    // 순회
    for (TNode* pNode = g_pHead.pNext;
        pNode != NULL;
        pNode = pNode->pNext)
    {
        // 출력
        std::cout << pNode->iValue << " ";
    }
}
void BackwardPrint()
{
    // 순회
    for (TNode* pNode = g_pEndNode;
        pNode != &g_pHead;
        pNode = pNode->pPrev)
    {
        // 출력
        //std::cout << pNode->iValue << " ";
    }
}
void ForwardPrint(TNode* pStartNode)
{
    if (pStartNode == NULL)
    {
        //std::cout << "해당하는 id를 검색하지 못했습니다." << std::endl;
        return;
    }
    // 순회
    for (TNode* pNode = pStartNode;
        pNode != NULL;
        pNode = pNode->pNext)
    {
        // 출력
        //std::cout << pNode->iValue << " ";
    }
    //std::cout << std::endl;
}
void BackwardPrint(TNode* pStartNode)
{
    if (pStartNode == NULL) return;
    // 순회
    for (TNode* pNode = pStartNode;
        pNode != &g_pHead;
        pNode = pNode->pPrev)
    {
        // 출력
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