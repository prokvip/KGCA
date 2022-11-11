#pragma once
#include "TBaseObject.h"
#include "TCameraDebug.h"
class TMap : public TBaseObject
{
public:
	DWORD        m_dwNumRows;
	DWORD        m_dwNumColumns;
	DWORD_VECTOR indexlist;
	std::vector<float>   m_fHeightList;
public:
	bool  Build(UINT iWidth, UINT iHeight);
	bool  UpdateBuffer(TCameraDebug* pMainCamera);
	bool  LoadHeightMap(ID3D11Device* pd3dDevice,// 디바이스 객체
		ID3D11DeviceContext* pImmediateContext, W_STR loadTexture);
	void  GenVertexNormal();
	TVector3 ComputeFaceNormal(UINT i0, UINT i1, UINT i2);
};

