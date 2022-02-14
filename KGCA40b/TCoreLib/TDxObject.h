#pragma once
#include "TStd.h"
#include <d3dcompiler.h>
#include "DDSTextureLoader.h"
#include "WICTextureLoader.h"
#pragma comment	(lib, "d3dcompiler.lib")
struct SimpleVertex
{
	TVector2 v;
	TVector2 t;
};
class TDxObject 
{
public:
	ID3D11ShaderResourceView*	m_pSRV0;
	ID3D11Texture2D*			m_pTexture0;
	ID3D11ShaderResourceView*	m_pSRV1;
	ID3D11Texture2D*			m_pTexture1;
	ID3D11BlendState*			m_AlphaBlend;
	D3D11_TEXTURE2D_DESC		m_TextureDesc;
public:
	float		m_fSpeed;
	TVector2	m_vPos;
	float		m_fWidth;
	float		m_fHeight;
public:
	std::vector<SimpleVertex> m_InitScreenList;
	std::vector<SimpleVertex> m_VertexList;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;

	ID3D11Device*		 m_pd3dDevice;	
	ID3D11DeviceContext* m_pContext;

	ID3DBlob*			m_pVSCodeResult = nullptr;
	ID3DBlob*			m_pErrorMsgs = nullptr;
	ID3DBlob*			m_pPSCodeResult = nullptr;
public:
	void    SetDevice(ID3D11Device* m_pd3dDevice,
					  ID3D11DeviceContext* m_pContext);
	virtual bool    LoadTexture(const TCHAR* szColorFileName,
								const TCHAR* szMaskFileName);
	virtual bool    SetVertexData();
	virtual bool	Create( ID3D11Device* m_pd3dDevice,
					ID3D11DeviceContext* m_pContext,
					TVector2 pos, float fWidth, float fHeight,
					const TCHAR* szTextureFileName=nullptr,
					const TCHAR* szMaskFileName = nullptr);
public:
	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	Render();
	virtual bool	Release();
public:
	TDxObject();
	~TDxObject();
};

