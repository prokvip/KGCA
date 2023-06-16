/*
*  후위 연결리스트 && 단방향 구현
*/
#include <iostream>
struct TNode
{
    int   iID;
    int   iValue;
    TNode* pNext;
};
TNode g_pHead; // 머리 ( 빈머리 )
TNode* g_pEndNode = NULL;
int   g_iCounter = 0;
TNode* Create();
void Link(TNode* pNewNode);
void Print();

int main()
{
    g_pEndNode = &g_pHead;
    g_pHead.iID = -1; g_pHead.iValue = -1; g_pHead.pNext = NULL;
    for (int iNode = 0; iNode < 300; iNode++)
    {
        TNode* pNewNode = Create();
        Link(pNewNode);
    }
    Print();

    TNode* pNode = g_pHead.pNext;
    while (pNode != NULL)
    {
        TNode* pNext = pNode->pNext;
        free(pNode);
        pNode = pNext;
    }
    // 탐색, 삭제, 수정, 정렬, 저장, 불러오기, 삽입, 소멸
}

// 1개의 노드 생성 및 초기화
TNode* Create()
{
    // 데이터 생성
    TNode* node = (TNode*)malloc(sizeof(TNode));
    node->iID = g_iCounter++;
    node->iValue = g_iCounter * 10;
    node->pNext = NULL;
    return node;
}
void Link(TNode* pNewNode)
{
    g_pEndNode->pNext = pNewNode;
    g_pEndNode = pNewNode;
    /* TNode* pEndNode = NULL;
     for (TNode* pNode = g_pHead.pNext;
         pNode != NULL;
         pNode = pNode->pNext)
     {
         if (pNode->pNext == NULL)
         {
             pEndNode = pNode;
             break;
         }
     }
     if (pEndNode != NULL)
     {
         pEndNode->pNext = pNewNode;
     }
     else
     {
         g_pHead.pNext = pNewNode;
     }*/
}
void Print()
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