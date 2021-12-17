#include "TNode.h"
template<class T>
TNode<T>::TNode()
{
	m_pNext	= nullptr;
	m_pPrev = nullptr;
	//m_pData = -1;
}
template<class T>
TNode<T>::TNode(T pData) : m_pData( pData)
{
	m_pNext = nullptr;
	m_pPrev = nullptr;
}
template<class T>
TNode<T>::~TNode()
{

}