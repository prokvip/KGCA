#pragma once
/*
*  ���� ���Ḯ��Ʈ && ����� ����
*  ���� ���Ḯ��Ʈ && ����� ����
*/
#include <iostream>
struct TNode
{
    int   iID;
    int   iValue;
    TNode* pNext;
    TNode* pPrev;
};
extern TNode g_pHead; // �Ӹ� ( ��Ӹ� )
extern TNode* g_pEndNode;
extern int   g_iCounter;

TNode* NewNode();
void Push_Back(TNode* pNewNode);
void Push_Front(TNode* pNewNode);
TNode* Find(int id);
void ForwardPrint();
void BackwardPrint();
void ForwardPrint(TNode* pNode);
void BackwardPrint(TNode* pNode);


