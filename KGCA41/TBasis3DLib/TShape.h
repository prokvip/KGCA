#pragma once
#include "TActor.h"
class TShape : public TActor
{

public:
	void	CreateOBBBox(	float fExtX = 1.0f, float fExtY = 1.0f, float fExtZ = 1.0f,
							TVector3 vCenter = TVector3(0.0f, 0.0f, 0.0f),
							TVector3 vDirX = TVector3(1.0f, 0.0f, 0.0f),
							TVector3 vDirY = TVector3(0.0f, 1.0f, 0.0f),
							TVector3 vDirZ = TVector3(0.0f, 0.0f, 1.0f));
	void	CreateAABBBox(	TVector3 vMax = TVector3(1.0f, 1.0f, 1.0f),
							TVector3 vMin = TVector3(-1.0f, -1.0f, -1.0f));
public:
	TShape() {};
	virtual ~TShape(void) {};
};
class TLineShape : public TShape
{
public:
	vector<PC_VERTEX>	m_LineVertexList;
public:
	HRESULT				SetInputLayout();
	bool				CreateVertexData();
	bool				CreateIndexData();
	HRESULT				CreateVertexBuffer();
	HRESULT				CreateResource();
	bool				Draw(ID3D11DeviceContext* pContext, TVector3 vStart, TVector3 vEnd, TVector4 dwColor);
public:
	TLineShape(void);
	virtual ~TLineShape(void);
};

class TDirectionLineShape : public TShape
{
public:
	vector<PC_VERTEX>	m_LineVertexList;
public:
	HRESULT			SetInputLayout();
	bool			CreateVertexData();
	bool			CreateIndexData();
	HRESULT			CreateVertexBuffer();
	HRESULT			CreateResource();
public:
	TDirectionLineShape(void);
	virtual ~TDirectionLineShape(void);
};

class TBoxShape : public TShape
{
public:
	bool			CreateVertexData();
	bool			CreateIndexData();
	HRESULT			CreateResource();
public:
	TBoxShape(void);
	virtual ~TBoxShape(void);
};

class TPlaneShape : public TShape
{
public:
	bool			CreateVertexData();
	bool			CreateIndexData();
	HRESULT			CreateResource();
	bool SetScreenVertex(float x,
		float y,
		float w,
		float h,
		TVector2 vScreen);
public:
	TPlaneShape(void);
	virtual ~TPlaneShape(void);
};
class TSphereShape : public TShape
{
public:
	bool			CreateVertexData();
	bool			CreateIndexData();
	bool			Render(ID3D11DeviceContext*	pImmediateContext);
	HRESULT			CreateResource();
	void CreateSphere(UINT Slices = 20, float fRadius = 1.0f);
	void MakeSpherePoint(std::vector<TVector3> &spherePoints,
		UINT Slices = 20, double r = 1.0f,
		TVector3 center = TVector3(0, 0, 0));
public:
	TSphereShape(void);
	virtual ~TSphereShape(void);
};