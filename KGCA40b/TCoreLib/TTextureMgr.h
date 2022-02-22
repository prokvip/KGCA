#pragma once
#include "TStd.h"
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
	ID3D11Texture2D*			m_pTexture;
	ID3D11ShaderResourceView*	m_pSRV;
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

class TTextureMgr : public TSingleton<TTextureMgr>
{
	int		m_iIndex;
public:
	ID3D11Device* m_pd3dDevice;
	friend class TSingleton<TTextureMgr>;
	std::map<std::wstring, TTexture* >  m_list;
public:
	void		  Set(ID3D11Device* pd3dDevice)
	{
		m_pd3dDevice = pd3dDevice;
	}
	TTexture*     Load(std::wstring filename);
	TTexture*		GetPtr(std::wstring key);
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
private:
	TTextureMgr();
public:
	~TTextureMgr();
};

#define I_Texture TTextureMgr::Get()