#pragma once
#include "TBaseMgr.h"
#include "DDSTextureLoader.h"
#include "WICTextureLoader.h"
#pragma comment	(lib, "d3dcompiler.lib")
class TTexture
{
public:
	int				m_iIndex;
	std::wstring	m_csName;
	std::wstring	m_szMsg;
	TCHAR			m_szBuffer[256];
	ComPtr<ID3D11Texture2D> m_pTexture;
	ComPtr<ID3D11ShaderResourceView> m_pSRV;
	D3D11_TEXTURE2D_DESC		m_TextureDesc;
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
	bool	Load(ID3D11Device* pd3dDevice, std::wstring filename);
public:
	TTexture();
	virtual ~TTexture();
};

class TTextureMgr : public TBaseMgr<TTexture, TTextureMgr>					
{
	friend class TSingleton<TTextureMgr>;
private:
	TTextureMgr() {};
public:
	~TTextureMgr() {};
};
#define I_Texture TTextureMgr::Get()