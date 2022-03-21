#pragma once
#include "TObject3D.h"
class TMap : public TObject3D
{
	UINT		m_iNumCols=0;
	UINT		m_iNumRows = 0;
	UINT        m_iNumVertices = 0;
	UINT		m_iNumCellCols = 0;
	UINT		m_iNumCellRows = 0;
	UINT        m_iNumFaces = 0;
	float       m_iCellDistance = 1.0f;
public:
	virtual bool		CreateMap(UINT width, UINT height, float fDistance);
	virtual bool		SetVertexData();
	virtual bool		SetIndexData();
public:
	TMap();
	virtual ~TMap();
};

