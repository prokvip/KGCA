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
	// 1) ���Ḯ��Ʈ ��� �߰�
	void AddLink(TNodeBox<T>* const pUser);

	// 2) ���Ḯ��Ʈ ��� ����
	void DeleteAll();
	TLinkedList()
	{
		m_iNumNode = 0;
		m_pHeadNode = 0;
		m_pEndNode = 0;
	}
};

