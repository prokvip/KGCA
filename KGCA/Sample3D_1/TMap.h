#pragma once
#include "TObject3D.h"
struct TFace
{
	UINT v0, v1, v2; // vb index;
	T::TVector3 vNomal; // vb index;
};
//struct TVertexFace
//{
//	std::vector<UINT> iFaceList;
//};
class TMap : public TObject3D
{
public:
	std::vector< TFace> m_FaceList;
	//std::vector< TVertexFace> m_VertexFaceList;

	UINT		m_iNumCols=0;
	UINT		m_iNumRows = 0;
	UINT        m_iNumVertices = 0;
	UINT		m_iNumCellCols = 0;
	UINT		m_iNumCellRows = 0;
	UINT        m_iNumFaces = 0;
	float       m_fCellDistance = 1.0f;
	std::vector<float>  m_fHeightList;
public:
	virtual bool		CreateHeightMap(const TCHAR* strHeightMapTex);
	virtual bool		CreateMap(UINT width, UINT height, float fDistance);
	virtual bool		SetVertexData();
	virtual bool		SetIndexData();
	virtual bool		Frame() override;
	float GetHeight(float fPosX, float fPosZ);
	float GetHeightmap(int row, int col);
	float Lerp(float fStart, float fEnd, float fTangent);
public:
	TMap();
	virtual ~TMap();
};

