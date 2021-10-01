#pragma once
#include <iostream>

struct TNode
{
	int		m_iValue;
	TNode*	m_pNext;
};

class TLinkedList
{
public:
	TNode* m_pHead;
	TNode* m_pTail;
	TNode* m_pEnd;
public:
	void	Show();
	int		Pop();
	void   AddForwardLink(TNode* pNew);
	void   AddBackwardLink(TNode* pNew);
public:
	TLinkedList();
	~TLinkedList();
};

