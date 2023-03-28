#pragma once
#include "TBasisStd.h"
#include "TDxHelperEx.h"
#include <wincodec.h>
#include <wincodecsdk.h>
#include "./wic/ScreenGrab.h"
#include <io.h>
#include <mutex>
#pragma comment (lib,"windowscodecs.lib")
class TTexture
{
public:
	
	T_STR		m_szName;
	T_STR		m_szPath;
	//--------------------------------------------------------------------------------------
	// 텍스쳐 로딩 및 리소스버퍼 생성
	//--------------------------------------------------------------------------------------
	//ID3D11ShaderResourceView*	m_pTextureSRV;
	ComPtr<ID3D11ShaderResourceView>		m_pTextureSRV;
	ComPtr<ID3D11Texture2D>					m_Texture;	
	D3D11_TEXTURE2D_DESC					m_Desc;
public:
	HRESULT						Load(ID3D11Device* pDevice, const TCHAR* strFilePath);
	// Generater auto mipmap 
	HRESULT						Load(ID3D11Device* pDevice, ID3D11DeviceContext*    pContextconst, TCHAR* strFilePath);
	bool						Apply(ID3D11DeviceContext*    pImmediateContext);
	bool						Release();
	void						SetPath(const TCHAR* pPath);

public:
	TTexture(void);
	virtual ~TTexture(void);
};

class TTextureMgr : public TSingleton < TTextureMgr >
{
private:
	std::mutex  m_Mutex;
	friend class TSingleton<TTextureMgr>;
	ID3D11Device*			m_pd3dDevice;
	ID3D11DeviceContext*    m_pContext;	
public:
	typedef unordered_map <INT, TTexture*>		TemplateMap;
	typedef TemplateMap::iterator				TemplateMapItor;
	TemplateMapItor								TItor;
	TemplateMap									TMap;
	INT											m_iCurIndex;
	std::vector<std::wstring>  m_fileList;
	TTexture*				m_pDefaultWhiteTexture;
	TTexture*				m_pDefaultBlackTexture;
	std::wstring			m_strDefaultPath;
public:
	void			Set(ID3D11Device*	 pDevice, ID3D11DeviceContext*    pContext);
	INT				Add(ID3D11Device*	 pDevice, const TCHAR *pFileName, bool bAutoMipMap = true);
	INT				Add(ID3D11Device*	 pDevice, const TCHAR *pFileName, const TCHAR* szPath, bool bAutoMipMap = true);
	TTexture* const	GetPtr(INT iIndex);
	TTexture* const	GetPtr(T_STR strFindName);
	bool			Release();

	void LoadDir(std::wstring path);
	static HRESULT SaveFile(ID3D11DeviceContext* pContext, ID3D11Texture2D* pRes, T_STR name);
	static HRESULT SaveFile(ID3D11DeviceContext* pContext, IDXGISwapChain* pSwapChain, T_STR name);
public:
	TTextureMgr();
	virtual ~TTextureMgr();
};
#define I_Texture TTextureMgr::GetInstance()

