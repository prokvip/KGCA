#pragma once
#include "TTexture.h"
struct PC_VERTEX
{
	TVector3 pos;
	TVector4 color;
	PC_VERTEX()
	{
		color.x = 1.0f;
		color.y = 1.0f;
		color.z = 1.0f;
		color.w = 1.0f;
	}
};
struct PCT_VERTEX
{
	TVector3 pos;
	TVector4 color;
	TVector2 tex;
	PCT_VERTEX()
	{
		color.x = 1.0f;
		color.y = 1.0f;
		color.z = 1.0f;
		color.w = 1.0f;
	}
};
struct PNCT_VERTEX
{
	TVector3 pos;
	TVector3 normal;
	TVector4 color;
	TVector2 tex;
	PNCT_VERTEX()
	{
		color.x = 1.0f;
		color.y = 1.0f;
		color.z = 1.0f;
		color.w = 1.0f;
	}
};

struct CB_DATA
{
	TMatrix  matWorld;
	TMatrix  matView;
	TMatrix  matProj;
	TVector4 vValue;
};

class TModel 
{
public:
	UINT				m_iVertexSize;
	UINT				m_iNumIndex;
	CB_DATA				m_cbData;
	ID3DBlob* m_pVSBlob = nullptr;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	ID3D11Buffer* m_pConstantBuffer;
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11VertexShader* m_pVS;
	ID3D11PixelShader* m_pPS;
	std::vector<PNCT_VERTEX> m_pVertexList;	
	std::vector< DWORD> m_IndexList;
public:
	TMatrix		m_matWorld;
public:
	virtual bool	CreateVertexData();
	virtual bool	CreateIndexData();
	virtual HRESULT		CreateConstantBuffer();
	virtual HRESULT		CreateVertexBuffer();
	virtual HRESULT		CreateIndexBuffer();
	virtual HRESULT		CreateVertexLayout();
	virtual HRESULT		LoadShader(std::wstring vs, std::wstring ps);
	virtual bool		LoadObject(std::wstring filename);
	virtual void		SetMatrix(TMatrix* pMatWorld,
		TMatrix* pMatView, TMatrix* pMatProj);
public:
	virtual bool	Create(std::wstring vsFile, std::wstring psFile);
	virtual bool   Init();
	virtual bool   Frame();
	virtual bool   PreRender(ID3D11DeviceContext* pContext);
	virtual bool   Render(ID3D11DeviceContext* pContext);
	virtual bool   PostRender(	ID3D11DeviceContext* pContext, 
								UINT iNumIndex=0);
	virtual bool   Release();
public:
	TModel();
	virtual ~TModel();
};

