#pragma once
#include "TStd.h"
class TNode
{
public:
	static      int g_iNewCounter;
	int			m_iIndex;
	TBox		m_tRect;
	std::vector<int>  m_ObjectList;
	TNode*		m_pChild[8];
	int			m_iDepth;
	TNode*		m_pParent;
public:	
	bool		AddObject(int index);
	bool		IsRect(XVector3 pos);
public:
	TNode() {};
	TNode(float x, float y, float z, float w, float h, float t);
	TNode(XVector3 vPivot, XVector3 vSize);
	~TNode();
};
