#pragma once
#include "TStd.h"

struct SimpleVertex
{
	XVector3 pos;
	XVector4 color;
	SimpleVertex()
	{
		color.x = 1.0f;
		color.y = 1.0f;
		color.z = 1.0f;
		color.w = 1.0f;
	}
};
struct CB_DATA
{
	XMatrix  matWorld;
	XMatrix  matView;
	XMatrix  matProj;
};
class TModel 
{
public:
	CB_DATA				m_cbData;
	ID3DBlob* m_pVSBlob = nullptr;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pConstantBuffer;
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11VertexShader* m_pVS;
	ID3D11PixelShader* m_pPS;
	std::vector< SimpleVertex> m_VertexList;
public:
	XMatrix		m_matWorld;
public:
	virtual bool	CreateVertexData();
	HRESULT		CreateConstantBuffer();
	HRESULT		CreateVertexBuffer();
	HRESULT		CreateVertexLayout();
	HRESULT		LoadShader();
	bool		LoadObject(std::wstring filename);
	void		SetMatrix(XMatrix* pMatWorld,
		XMatrix* pMatView, XMatrix* pMatProj);
public:
	bool   Init();	
	bool   Frame();
	bool   Render(ID3D11DeviceContext* pContext);
	bool   Release();
public:
	TModel();
};

