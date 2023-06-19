#pragma once
#include "Linkedlist.h"
int main()
{
    g_pEndNode = &g_pHead;
    g_pHead.iID = -1; g_pHead.iValue = -1; 
    g_pHead.pNext = NULL;
    g_pHead.pPrev = NULL;
    
    for (int iNode = 0; iNode < 10; iNode++)
    {
        TNode* pNewNode = NewNode();
        Push_Front(pNewNode);
    }
    for (int iNode = 0; iNode < 10; iNode++)
    {
        TNode* pNewNode = NewNode();
        Push_Back(pNewNode);
    }
    BackwardPrint();
    ForwardPrint();
    // 검색 출력
    BackwardPrint(Find(10));
    ForwardPrint(Find(1879670));

    TNode* pNode = g_pHead.pNext;
    while (pNode != NULL)
    {
        TNode* pNext = pNode->pNext;
        free(pNode);
        pNode = pNext;
    }
    // 탐색, 삭제, 수정, 정렬, 저장, 불러오기, 삽입, 소멸
}
