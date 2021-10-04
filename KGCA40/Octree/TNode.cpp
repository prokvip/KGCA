#include "TNode.h"
int   TNode::g_iNewCounter = 0;

bool TNode::AddObject(int iIndex)
{
	m_ObjectList.push_back(iIndex);
	return true;
}
bool		TNode::IsRect(XVector3 pos)
{
	if (this->m_tRect.vMin.x <= pos.x &&	this->m_tRect.vMax.x >= pos.x &&
		this->m_tRect.vMin.y <= pos.y &&	this->m_tRect.vMax.y >= pos.y &&
		this->m_tRect.vMin.z <= pos.z && this->m_tRect.vMax.z >= pos.z)
	{
		return true;
	}
	return false;
}
TNode::TNode(float x, float y, float z, float w, float h, float t)
{
	m_pParent = nullptr;
	m_iDepth = 0;
	this->m_tRect.vPivot = XVector3(x, y, z);
	this->m_tRect.vMin = XVector3(x, y, z - t);
	this->m_tRect.vMax = XVector3(x + w, y + h, z);
	this->m_tRect.vSize = XVector3(w,h,t);
}
TNode::TNode(XVector3 vPivot, XVector3 vSize)
{
	m_pParent = nullptr;
	m_iDepth = 0;
	this->m_tRect.vPivot = vPivot;
	this->m_tRect.vMin = XVector3(vPivot.x, vPivot.y, vPivot.z-vSize.z);
	this->m_tRect.vMax = XVector3(vPivot.x+ vSize.x, vPivot.y+ vSize.y, vPivot.z);
	this->m_tRect.vSize = vSize;
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
