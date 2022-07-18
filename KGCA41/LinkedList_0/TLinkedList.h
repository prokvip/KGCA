#pragma once
#include "TStudent.h" 

class TLinkedList
{
protected:
	TStudent* m_pHead = NULL;
	TStudent* m_pTail = NULL;
	TStudent* m_pNext = NULL;
	TStudent* m_pCurNode = NULL;
public:
	static int	  m_iCounter;
public:
	TStudent* operator ++(void);
	TStudent* operator ++(int);
	TStudent* operator [](int);
public:
	TStudent*	begin();
	TStudent*	end();
	TStudent*	next();
	TStudent*	rbegin();
	TStudent*	rend();
	TStudent*	rnext();
	int			size();
public:
	void		clear();	
	void		push_back(TStudent* pNewNode);
	void		push_front(TStudent* pNewNode);
	void		push_back(TStudent* pNewNode, TStudent* pLinkNode);
	void		push_front(TStudent* pNewNode, TStudent* pLinkNode);
	void		erase(TStudent* pNode);
	TStudent*	Find(int iAge);
	void		sort();
public:
	TLinkedList();
	~TLinkedList();
};

