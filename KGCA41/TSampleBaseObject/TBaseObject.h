#pragma once
#include "TDevice.h"
struct SimpleVertex
{
	float  x; // 위치
	float  y;
	float  z;
};

class TBaseObject 
{
public:
	ID3D11Device* m_pd3dDevice = nullptr;// 디바이스 객체
	ID3D11DeviceContext* m_pImmediateContext = nullptr;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11VertexShader* m_pVS;
	ID3D11PixelShader* m_pPS;
	ID3DBlob* m_pVSCode = nullptr;
	ID3DBlob* m_pPSCode = nullptr;

	std::vector<SimpleVertex>    m_VertexList;
	std::vector<DWORD>			 m_IndexList;
public:
	virtual HRESULT		CreateVertexBuffer();
	virtual HRESULT		CreateIndexBuffer();
	virtual HRESULT     CreateVertexShader();
	virtual HRESULT     CreatePixelShader();
	virtual HRESULT     CreateVertexLayout();
public:
	virtual bool		Init() ;
	virtual bool		Frame() ;
	virtual bool		Render() ;
	virtual bool		Release() ;
};

