#include "TNode.h"
int   TNode::g_iNewCounter = 0;
bool TNode::AddObject(float fX, float fY)
{
	m_ObjectList.push_back(XVector2(fX, fY));
	return true;
}

bool TNode::AddObject(XVector2 pos)
{
	m_ObjectList.push_back(pos);
	return true;
}
bool		TNode::IsRect(XVector2 pos)
{
	if (this->m_tRect.p0.x <= pos.x &&
		this->m_tRect.p1.x >= pos.x &&
		this->m_tRect.p0.y <= pos.y &&
		this->m_tRect.p1.y >= pos.y)
	{
		return true;
	}
	return false;
}
TNode::TNode(float x, float y, float w, float h)
{
	m_pParent = nullptr;
	m_iDepth = 0;
	this->m_tRect.wh = XVector2(w,h);
	this->m_tRect.p0 = XVector2(x, y);
	this->m_tRect.p1 = 
		this->m_tRect.p0 + XVector2(w, h);
}
TNode::~TNode()
{
	for (int iChild = 0; iChild < 4; iChild++)
	{
		if (m_pChild[iChild] != nullptr)
		{
			delete m_pChild[iChild];
			m_pChild[iChild] = nullptr;
			TNode::g_iNewCounter--;
		}
	}
}