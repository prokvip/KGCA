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
typedef std::vector<DWORD>  DWORD_VECTOR;
struct TLodPatch
{
	UINT   iLodLevel;
	//std::vector<DWORD_VECTOR>  IndexList;
	DWORD_VECTOR		IndexList[16];
	ID3D11Buffer* IndexBufferList[16] = { nullptr, };
	//std::vector<ID3D11Buffer*> IndexBufferList;
	void Release()
	{
		for (int iBuffer = 0; iBuffer < 16; iBuffer++)
		{
			if (IndexBufferList[iBuffer])
			{
				IndexBufferList[iBuffer]->Release();
				IndexBufferList[iBuffer] = nullptr;
			}

		}
	}
	TLodPatch() {}
	~TLodPatch()
	{
		Release();
	}
};
// 8421 code 1111
// 0 ~ 16
//     1
// 8   x   2
//     4
// 

class TNode : public TModel
{
public:
	UINT			m_iLodLevel;
public:
	static      int g_iNewCounter;
	int			m_iIndex;
	POINT		m_Element;
	TRect		m_tRect;
	std::vector<TVector2>  m_ObjectList;
	std::vector<DWORD>  m_CornerList;
	TVector3			m_vCenter;
	TNode*		m_pChild[4];
	TNode*		m_NeighborList[4];
	int			m_iDepth;
	bool		m_bLeaf;
	TNode*		m_pParent;
public:	
	bool		AddObject(float fX, float fY);
	bool		AddObject(TVector2 pos);
	bool		IsRect(TVector2 pos);
	void		SetRect(float x, float y, float w, float h);
public:
	TNode() {};
	TNode(UINT x, UINT y, UINT w, UINT h);
	~TNode();
};

