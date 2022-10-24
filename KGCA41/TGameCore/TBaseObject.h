#pragma once
#include "TDevice.h"
#include "TTextureManager.h"
#include "TShaderManager.h"
#include "TCollision.h"
struct SimpleVertex
{
	TVector    p;	
	TVector4  c;
	TVector2  t;
	SimpleVertex() {}
	SimpleVertex(TVector vp, TVector4 vc, TVector2 vt)
	{
		p = vp;
		c = vc;
		t = vt;
	}
};
struct VS_CONSTANT_BUFFER
{
	TMatrix  matWorld;
	TMatrix  matView;
	TMatrix  matProj;	
	float    x;
	float    y;
	float    fTimer;
	float    d;
};
class TBaseObject 
{
public:
	bool		m_IsNullable=true;
	TMatrix		m_matWorld;
	TMatrix		m_matView;
	TMatrix		m_matProj;
	DWORD		m_dwFace;
public:
	ID3D11Device* m_pd3dDevice = nullptr;// ����̽� ��ü
	ID3D11DeviceContext* m_pImmediateContext = nullptr;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	VS_CONSTANT_BUFFER  m_cbData;
	ID3D11Buffer* m_pConstantBuffer;
	ID3D11InputLayout* m_pVertexLayout;
	TShader* m_pShader;
	TTexture* m_pTexture;
	std::wstring m_szShaderName;
	std::wstring m_szTextureName;
	ID3D11ShaderResourceView*    m_pTextureSRV = nullptr;
	std::vector<SimpleVertex>    m_VertexList;
	std::vector<SimpleVertex>    m_InitVertexList;
	std::vector<DWORD>			 m_IndexList;
	TRect						 m_rtCollision;
public:
	ID3D11VertexShader* m_pVS;
	ID3D11PixelShader* m_pPS;
	ID3DBlob* m_pVSCode = nullptr;
	ID3DBlob* m_pPSCode = nullptr;
public:
	virtual bool		Create(
		ID3D11Device* pd3dDevice,// ����̽� ��ü
		ID3D11DeviceContext* pImmediateContext,
		std::wstring shadername, std::wstring texturename);
	virtual void		CreateVertexData();
	virtual void		CreateIndexData();
	virtual void		CreateConstantData();
	virtual HRESULT		CreateVertexBuffer();
	virtual HRESULT		CreateIndexBuffer();
	virtual HRESULT		CreateConstantBuffer();
	virtual bool		CreateShader(std::wstring filename);
	virtual HRESULT     CreateVertexShader(std::wstring filename);
	virtual HRESULT     CreatePixelShader(std::wstring filename);
	virtual HRESULT     CreateVertexLayout();
	virtual void		UpdateVertexBuffer();
	virtual void		UpdateConstantBuffer();
public:
	virtual bool		Init() ;
	virtual bool		Frame() ;
	virtual bool		PreRender();
	virtual bool		Render();
	virtual bool		PostRender();
	virtual bool		Release() ;	
	virtual void		SetMatrix(TMatrix* matWorld, TMatrix* matView, TMatrix* matProj);
public:
	TBaseObject()
	{
		m_IsNullable = false;
	}
};

