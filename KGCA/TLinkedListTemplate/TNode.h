#pragma once
#include <iostream>
#include <assert.h>

template<class T>
class TLinkedList; // 전방선언

template<class T>
class TNode
{
public:
	T    m_pData;
private:
	TNode<T>* m_pNext;
	TNode<T>* m_pPrev;

	friend TLinkedList<T>;
public:
	TNode<T>* next() { return m_pNext; }
public:
	TNode();
	TNode(T pData);
	virtual ~TNode();
};

