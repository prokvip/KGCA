#include "TNode.h"

TNode::TNode()
{
	m_pNext	= nullptr;
	m_pPrev = nullptr;
	m_iValue = -1;
}
TNode::TNode(int iData) : m_iValue( iData)
{
	m_pNext = nullptr;
	m_pPrev = nullptr;
}
TNode::~TNode()
{

}