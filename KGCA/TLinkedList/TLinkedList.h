#pragma once
#include "TNode.h"
class TLinkedList
{
private:
	TNode* m_pHead;
	TNode* m_pTail;
	TNode* m_pCurrent;
public:
	TNode* begin();
	TNode* end();
	TNode* rbegin();
	TNode* rend();
	TNode* operator++(int);
	TNode* operator++(void);
public:
	void   PushBack(TNode* pNode);
	void   PushFront(TNode* pNode);
	void   insert(TNode* pPosNode, TNode* pNode);
	TNode* find(int iValue);
	void   clear();
	TNode* erase(TNode* pNode);
public:
	TLinkedList();
	virtual ~TLinkedList();
};

