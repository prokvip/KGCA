#pragma once
#include "TStd.h"
class TNode
{
public:
	static      int g_iNewCounter;
	TRect		m_tRect;
	std::vector<TVector2>  m_ObjectList;
	TNode*		m_pChild[4];
	int			m_iDepth;
	TNode*		m_pParent;
public:	
	bool		AddObject(float fX, float fY);
	bool		AddObject(TVector2 pos);
	bool		IsRect(TVector2 pos);
public:
	TNode() {};
	TNode(float x, float y, float w, float h);
	~TNode();
};

