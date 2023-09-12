#pragma once
#include "TObject.h"
struct TMapInfo
{
	UINT  row;
	UINT  col;
	float fDistance;
	W_STR strDefaultTextureFileName;
	W_STR strShaderFileName;
};
class TMap : public TObject
{
	TMapInfo		m_MapInfo;
	UINT m_iNumVertices;
	UINT m_iNumFace;
	UINT m_iNumCellRows;
	UINT m_iNumCellCols;
	UINT m_iNumCols;
	UINT m_iNumRows;
public:
	float		GetHeightOfVertex(UINT iVertexIndex);
	TVector3	GetNormalOfVertex(UINT iVertexIndex);
	TVector4	GetColorOfVertex(UINT iVertexIndex);
	TVector2	GetTextureOfVertex(UINT, UINT);
public:
	bool	CreateVertexData() override;
	bool	CreateIndexData()override;
	bool	 Load(TMapInfo info);
};

