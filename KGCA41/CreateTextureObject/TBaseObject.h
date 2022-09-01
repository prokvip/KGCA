#pragma once
#include "TStd.h"
#include "TVector.h"
// p(위치)n(노말)c(컬러)t(텍스처좌표)
struct SimpleVertex
{
	TVector		p; // 위치
	TVector4D	c;
	TVector2D   t;
};
class TBaseObject
{
public:
	ID3D11Device* m_pd3dDevice = nullptr;// 디바이스 객체
	ID3D11DeviceContext* m_pImmediateContext = nullptr;
	void		  SetDevice(ID3D11Device* pd3dDevice,ID3D11DeviceContext* pContext);
public:
	ID3D11Resource* m_Texture = nullptr;
	ID3D11ShaderResourceView* m_pTextureSRV = nullptr;
	std::vector< SimpleVertex>  m_VertexList;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11VertexShader* m_pVS;
	ID3D11PixelShader* m_pPS;
	ID3DBlob* m_pVSCode = nullptr;
	ID3DBlob* m_pPSCode = nullptr;
public:
	HRESULT		CreateVertexBuffer();
	HRESULT		CreateShader();
	HRESULT     CreateVertexLayout();
	HRESULT		LoadTexture(std::wstring filename);
public:
	virtual bool		Create(std::wstring texName);
public:
	virtual bool		Init(); // 초기화
	virtual bool		Frame();// 실시간 계산
	virtual bool		Render();// 실시간 랜더링
	virtual bool		Release();// 소멸 및 삭제
};

