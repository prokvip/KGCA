#pragma once
#include "TNodeBox.h"
template<class T>
class TLinkedList
{
public:
	int			 m_iNumNode;
	TNodeBox<T>* m_pHeadNode;
	TNodeBox<T>* m_pEndNode;
public:
	TNodeBox<T>* GetHead()
	{
		return m_pHeadNode;
	}
public:
	// 1) 연결리스트 노드 추가
	void AddLink(TNodeBox<T>* const pUser);

	// 2) 연결리스트 노드 삭제
	void DeleteAll();
	TLinkedList()
	{
		m_iNumNode = 0;
		m_pHeadNode = 0;
		m_pEndNode = 0;
	}
};

