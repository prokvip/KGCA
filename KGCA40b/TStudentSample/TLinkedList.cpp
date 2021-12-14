#include "TLinkedList.h"
template<class T>
void TLinkedList<T>::AddLink(TNodeBox<T>* const pNode)
{
	if (m_pHeadNode == NULL)
	{
		m_pHeadNode = pNode;
		m_pEndNode = pNode;
		m_iNumNode++;
		return;
	}
	m_pEndNode->m_pNext = pNode;
	m_pEndNode = pNode;
	m_iNumNode++;
}
template<class T>
void TLinkedList<T>::DeleteAll()
{
	TNodeBox<T>* m_pNext = m_pHeadNode;
	while (m_pNext)
	{
		TNodeBox<T>* pDeleteNode = m_pNext;
		m_pNext = pDeleteNode->m_pNext;
		delete pDeleteNode;
		pDeleteNode = nullptr;
		m_iNumNode--;
	}
	m_pHeadNode = nullptr;
}