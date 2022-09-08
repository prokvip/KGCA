#pragma once
#include "TDevice.h"
#include "TTextureManager.h"
struct SimpleVertex
{
	TVector    p;	
	TVector4D  c;
	TVector2D  t;
};

class TBaseObject 
{
public:
	ID3D11Device* m_pd3dDevice = nullptr;// ����̽� ��ü
	ID3D11DeviceContext* m_pImmediateContext = nullptr;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11VertexShader* m_pVS;
	ID3D11PixelShader* m_pPS;
	ID3DBlob* m_pVSCode = nullptr;
	ID3DBlob* m_pPSCode = nullptr;
	TTexture* m_pTexture;
	std::vector<SimpleVertex>    m_VertexList;
	std::vector<DWORD>			 m_IndexList;
public:
	virtual bool		Create(
		ID3D11Device* pd3dDevice,// ����̽� ��ü
		ID3D11DeviceContext* pImmediateContext,
		std::wstring shadername, std::wstring texturename);
	virtual void		CreateVertexData();
	virtual void		CreateIndexData();
	virtual HRESULT		CreateVertexBuffer();
	virtual HRESULT		CreateIndexBuffer();
	virtual HRESULT     CreateVertexShader(std::wstring filename);
	virtual HRESULT     CreatePixelShader(std::wstring filename);
	virtual HRESULT     CreateVertexLayout();
	virtual void		UpdateVertexBuffer();
public:
	virtual bool		Init() ;
	virtual bool		Frame() ;
	virtual bool		Render() ;
	virtual bool		Release() ;
};

