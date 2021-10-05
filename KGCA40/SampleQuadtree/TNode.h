#pragma once
#include "TStd.h"
#include "TMap.h"
struct Rect
{
	float  x, y, w, h;
};
struct TRect
{
	TVector2  p0;
	TVector2  p1;
	TVector2  wh;
	// x1,y1(p0)
	//
	//          x2, y2(p1)
};

class TNode
{
public:
	static      int g_iNewCounter;
	int			m_iIndex;
	TRect		m_tRect;
	std::vector<TVector2>  m_ObjectList;
	std::vector<DWORD>  m_CornerList;
	std::vector<DWORD>  m_IndexList;
	ID3D11Buffer*		m_pIndexBuffer;
	TNode*		m_pChild[4];
	int			m_iDepth;
	bool		m_bLeaf;
	TNode*		m_pParent;
public:	
	bool		AddObject(float fX, float fY);
	bool		AddObject(TVector2 pos);
	bool		IsRect(TVector2 pos);
public:
	TNode() {};
	TNode(UINT x, UINT y, UINT w, UINT h);
	~TNode();
};

