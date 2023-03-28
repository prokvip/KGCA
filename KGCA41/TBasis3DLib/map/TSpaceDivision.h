#pragma once
#include "TNode.h"
#include "../TActor.h"
#include "../TBaseObj.h"
#include "../TCamera.h"
#include "../TTextureMgr.h"
#include <queue>

typedef vector <PNCT_VERTEX>			VertexList;
typedef VertexList::iterator			VertexListItor;

class TSpaceDivision
{
public:
	TCamera*		m_pCamera;
	void			Update( TCamera* pCamera ) { m_pCamera = pCamera; }
public:
	TCHAR			m_szTexturePath[MAX_PATH];
	int				CalculateBox( T_BOX *tBox, TPolygon *Polylist);
public:
	TSpaceDivision(void);
	virtual ~TSpaceDivision(void);
};

