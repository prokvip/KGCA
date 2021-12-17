#pragma once
#include "TNode.cpp"
template<class T>
class TLinkedList
{
private:
	TNode<T>* m_pHead;
	TNode<T>* m_pTail;
	TNode<T>* m_pCurrent;
public:
	TNode<T>* begin();
	TNode<T>* end();
	TNode<T>* rbegin();
	TNode<T>* rend();
	TNode<T>* operator++(int);
	TNode<T>* operator++(void);
public:
	void   PushBack(TNode<T>* pNode);
	void   PushFront(TNode<T>* pNode);
	void   insert(TNode<T>* pPosNode, TNode<T>* pNode);
	TNode<T>* find(int iValue);
	void   clear();
	TNode<T>* erase(TNode<T>* pNode);
public:
	TLinkedList();
	virtual ~TLinkedList();
};

