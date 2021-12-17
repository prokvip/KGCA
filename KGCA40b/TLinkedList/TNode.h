#pragma once
#include <iostream>
#include <assert.h>
class TLinkedList; // 전방선언
class TNode
{
public:
	int    m_iValue;
private:
	TNode* m_pNext;
	TNode* m_pPrev;
	friend TLinkedList;
public:
	TNode* next() { return m_pNext; }
public:
	TNode();
	TNode(int iData);
	virtual ~TNode();
};

