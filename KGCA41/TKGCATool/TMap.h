#pragma once
#include "TBaseObject.h"
#include "TCameraDebug.h"

struct tFaceInfo
{
	UINT     vertexArray[3];
	TVector3 vNormal;
};
struct tVertexInfo
{
	std::vector<UINT >  faceIndexArray;
	TVector3 vNormal;
};

class TMap : public TBaseObject
{
public:	
	std::vector<tFaceInfo> m_FaceNormals;
	std::vector<tVertexInfo> m_VertexInfo;
	void ComputeVertexNormal(UINT iVertex);
public:
	UINT         m_iNumCellCols;
	UINT         m_iNumCellRows;
	DWORD        m_iNumRows;
	DWORD        m_iNumCols;
	float		 m_fCellDistance = 1.0f;
	float        m_fScaleHeight = 100.0f;
	DWORD_VECTOR m_UpdateIndexList;
	std::vector<float>   m_fHeightList;
public:
	bool  Build(UINT iWidth, UINT iHeight);
	bool  UpdateBuffer(TCameraDebug* pMainCamera);
	bool  LoadHeightMap(ID3D11Device* pd3dDevice,ID3D11DeviceContext* pImmediateContext, W_STR loadTexture);
	void  GenVertexNormal();
	TVector3 ComputeFaceNormal(UINT i0, UINT i1, UINT i2);
	float GetHeight(float fPosX, float fPosZ);
	float GetHeightmap(int row, int col);
	float Lerp(float fStart, float fEnd, float fTangent);
	bool  Release() override;
};

