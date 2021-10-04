#pragma once
#include "TStd.h"
class TNode
{
public:
	static      int g_iNewCounter;
	int			m_iIndex;
	TRect		m_tRect;
	std::vector<XVector2>  m_ObjectList;
	TNode*		m_pChild[4];
	int			m_iDepth;
	TNode*		m_pParent;
public:	
	bool		AddObject(float fX, float fY);
	bool		AddObject(XVector2 pos);
	bool		IsRect(XVector2 pos);
public:
	TNode() {};
	TNode(float x, float y, float w, float h);
	~TNode();
};

