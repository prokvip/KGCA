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
	TTexture			m_Tex;
	UINT				m_iVertexSize=0;
	UINT				m_iNumIndex=0;
	CB_DATA				m_cbData;
	ID3DBlob* m_pVSBlob = nullptr;
	ID3D11Buffer* m_pVertexBuffer = nullptr;
	ID3D11Buffer* m_pIndexBuffer = nullptr;
	ID3D11Buffer* m_pConstantBuffer = nullptr;
	ID3D11InputLayout* m_pVertexLayout = nullptr;
	ID3D11VertexShader* m_pVS = nullptr;
	ID3D11PixelShader* m_pPS = nullptr;
	ID3D11PixelShader* m_pMainPS = nullptr;
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
	virtual bool		LoadTexture(std::wstring szTextureName);
public:
	virtual bool	Create(std::wstring vsFile, std::wstring psFile,
						   std::wstring szTextureName );
	virtual bool   Init();
	virtual bool   Frame();
	virtual bool   PreRender(ID3D11DeviceContext* pContext);
	virtual bool   Render(ID3D11DeviceContext* pContext);
	virtual bool   PostRender(	ID3D11DeviceContext* pContext, 
								UINT iNumIndex=0);
	virtual bool   Release();
public:
	static ID3DBlob*	LoadShaderBlob(std::wstring vs, std::string function, std::string version);
public:
	TModel();
	virtual ~TModel();
};

