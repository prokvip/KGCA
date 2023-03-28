#include "TTextureMgr.h"
#include <cstdlib>

HRESULT		TTexture::Load(ID3D11Device* pDevice, const TCHAR* strFilePath)
{
	if (strFilePath == NULL) return S_OK;

	
	HRESULT hr = S_OK;
	TCHAR szFileName[MAX_PATH] = { 0, };
	_stprintf_s(szFileName, _T("%s%s"), m_szPath.c_str(), strFilePath);
	m_szName = strFilePath;

	ComPtr<ID3D11Resource> res;
	DDS_ALPHA_MODE alphaMode;
	hr = DirectX::CreateWICTextureFromFile(pDevice, szFileName, res.GetAddressOf(), m_pTextureSRV.GetAddressOf());
	if (FAILED(hr))
	{
		hr = DirectX::CreateDDSTextureFromFile(pDevice, szFileName, res.GetAddressOf(), m_pTextureSRV.GetAddressOf(), 0, &alphaMode);
	}
	if (FAILED(hr))
	{
		//ErrorQuit(szFileName);
		return hr;
	}	
	

	hr = res.As(&m_Texture);
	if (FAILED(hr))
	{
		//ErrorQuit(szFileName);
		return hr;
	}
	m_Texture->GetDesc(&m_Desc);

	return hr;
}
HRESULT		TTexture::Load(ID3D11Device* pDevice, ID3D11DeviceContext*    pContextconst, TCHAR* strFilePath)
{
	if (strFilePath == NULL) return S_OK;


	HRESULT hr = S_OK;
	TCHAR szFileName[MAX_PATH] = { 0, };
	_stprintf_s(szFileName, _T("%s%s"), m_szPath.c_str(), strFilePath);
	m_szName = strFilePath;

	ComPtr<ID3D11Resource> res;
	DDS_ALPHA_MODE alphaMode;
	hr = DirectX::CreateWICTextureFromFile(pDevice, pContextconst, szFileName, res.GetAddressOf(), m_pTextureSRV.GetAddressOf());
	if (FAILED(hr))
	{
		hr = DirectX::CreateDDSTextureFromFile(pDevice, pContextconst, szFileName, res.GetAddressOf(), m_pTextureSRV.GetAddressOf(), 0, &alphaMode);
	}
	if (FAILED(hr))
	{
		//ErrorQuit(szFileName);
		return hr;
	}

	hr = res.As(&m_Texture);
	if (FAILED(hr))
	{
		//ErrorQuit(szFileName);
		return hr;
	}
	m_Texture->GetDesc(&m_Desc);

	return hr;
}
bool TTexture::Apply(ID3D11DeviceContext*    pImmediateContext)
{
	assert(pImmediateContext);
	if (!m_pTextureSRV)
	{
		return false;
	}
	pImmediateContext->PSSetShaderResources(0, 1, m_pTextureSRV.GetAddressOf());
	//pImmediateContext->PSSetSamplers(0, 1, &m_pSamplerState);
	return true;
}

bool TTexture::Release()
{
	return true;
}
TTexture::TTexture(void)
{
	m_szPath = _T("");	
}

TTexture::~TTexture(void)
{
}
void TTexture::SetPath(const TCHAR* pPath)
{
	m_szPath = pPath;
}

void TTextureMgr::LoadDir(std::wstring path)
{
	std::wstring dirPath = path + L"*.*";
	intptr_t handle; // fopen
	struct _wfinddata_t fd;
	handle = _wfindfirst(dirPath.c_str(), &fd);
	if (handle == -1L)
	{
		return;
	}
	do
	{
		// 디렉토리
		if ((fd.attrib & _A_SUBDIR) && fd.name[0] != '.')
		{
			LoadDir(path + fd.name + L"/");
		}
		else if (fd.name[0] != '.')// 파일, 
		{
			m_fileList.push_back(fd.name);
		}
	} while (_wfindnext(handle, &fd) == 0);
	_findclose(handle);

	for (std::wstring& data : m_fileList)
	{
		std::wstring pullpath = m_strDefaultPath + data;
		bool ret = Add(m_pd3dDevice, pullpath.c_str());
		if (!ret)
		{
			OutputDebugString(L"중복\n");
		}
	}
}
void TTextureMgr::Set(ID3D11Device*	 pDevice, ID3D11DeviceContext*    pContext)
{	
	m_pd3dDevice = pDevice;
	m_pContext= pContext;	
	m_pDefaultWhiteTexture = GetPtr(Add(m_pd3dDevice, Res(L"white.png").c_str()));
	m_pDefaultBlackTexture = GetPtr(Add(m_pd3dDevice, Res(L"black.bmp").c_str()));
}
INT TTextureMgr::Add(ID3D11Device*	 pDevice, const TCHAR *pFileName, bool bAutoMipMap)
{
	TCHAR szFileName[MAX_PATH];
	TCHAR Drive[MAX_PATH];
	TCHAR Dir[MAX_PATH];
	TCHAR FName[MAX_PATH];
	TCHAR Ext[MAX_PATH];
	if (pFileName)
	{
		_tsplitpath(pFileName, Drive, Dir, FName, Ext);
		Ext[4] = 0;
		memset(szFileName, 0, sizeof(TCHAR) * MAX_PATH);
		//D3DX11_IMAGE_FILE_FORMAT 
		if (_tcsicmp(Ext, _T(".tga")) == 0)
		{
			_tcscpy(Ext, _T(".bmp"));
		}
		_stprintf_s(szFileName, _T("%s%s"), FName, Ext);

		for (TemplateMapItor itor = TMap.begin(); itor != TMap.end(); itor++)
		{
			TTexture *pPoint = (TTexture *)(*itor).second;
			if (!_tcsicmp(pPoint->m_szName.c_str(), szFileName))
			{
				return (*itor).first;
			}
		}
	}
	TTexture *pPoint = NULL;
	SAFE_NEW(pPoint, TTexture);

	TCHAR szPath[MAX_PATH] = { 0, };
	_stprintf_s(szPath, _T("%s%s"), Drive, Dir);
	pPoint->SetPath(szPath);
	//std::lock_guard<std::mutex> lock(m_Mutex);
	m_Mutex.lock();
	if (bAutoMipMap == true)
	{
		if (FAILED(pPoint->Load(pDevice, m_pContext, szFileName)))
		{
			delete pPoint;
			m_Mutex.unlock();
			return 0;
		}
	}
	else
	{
		if (FAILED(pPoint->Load(pDevice, szFileName)))
		{
			delete pPoint;
			m_Mutex.unlock();
			return 0;
		}
	}
	m_Mutex.unlock();
	TMap.insert(make_pair(++m_iCurIndex, pPoint));
	return m_iCurIndex;
}
INT TTextureMgr::Add(ID3D11Device*	 pDevice, const TCHAR *pFileName, const TCHAR* szPath, bool bAutoMipMap)
{
	TCHAR szFileName[MAX_PATH];
	TCHAR Drive[MAX_PATH];
	TCHAR Dir[MAX_PATH];
	TCHAR FName[MAX_PATH];
	TCHAR Ext[MAX_PATH];
	if (pFileName)
	{
		_tsplitpath(pFileName, Drive, Dir, FName, Ext);
		Ext[4] = 0;
		memset(szFileName, 0, sizeof(TCHAR) * MAX_PATH);
		if (_tcsicmp(Ext, _T(".tga")) == 0)
		{
			_tcscpy(Ext, _T(".dds"));
		}
		_stprintf_s(szFileName, _T("%s%s"), FName, Ext);

		for (TemplateMapItor itor = TMap.begin(); itor != TMap.end(); itor++)
		{
			TTexture *pPoint = (TTexture *)(*itor).second;
			if (!_tcsicmp(pPoint->m_szName.c_str(), szFileName))
			{
				return (*itor).first;
			}
		}
	}
	TTexture *pPoint = NULL;
	SAFE_NEW(pPoint, TTexture);
	pPoint->SetPath(szPath);
	m_Mutex.lock();
	if (bAutoMipMap == true)
	{
		if (FAILED(pPoint->Load(pDevice, m_pContext, szFileName)))
		{
			m_Mutex.unlock();
			return 0;
		}
	}
	else
	{
		if (FAILED(pPoint->Load(pDevice, szFileName)))
		{
			m_Mutex.unlock();
			return 0;
		}
	}
	m_Mutex.unlock();

	std::pair<TemplateMapItor, bool> ret;
	ret = TMap.insert(make_pair(++m_iCurIndex, pPoint));
	if (ret.second == false)
	{
		return 0;
	}
	return m_iCurIndex;
}
bool TTextureMgr::Release()
{
	TTexture *pPoint;
	for (TemplateMapItor itor = TMap.begin(); itor != TMap.end(); itor++)
	{
		pPoint = (TTexture *)(*itor).second;
		pPoint->Release();
		SAFE_DEL(pPoint);
	}
	TMap.clear();
	return true;
}
TTexture* const TTextureMgr::GetPtr(INT iIndex)
{
	TemplateMapItor itor = TMap.find(iIndex);
	if (itor == TMap.end()) return nullptr;
	TTexture *pPoint = (*itor).second;
	return pPoint;
}
TTexture* const TTextureMgr::GetPtr(T_STR strFindName)
{
	for (TemplateMapItor itor = TMap.begin(); itor != TMap.end(); itor++)
	{
		TTexture *pPoint = (TTexture *)(*itor).second;
		if (pPoint->m_szName == strFindName)
		{
			return (*itor).second;
		}
	}
	return NULL;
}
HRESULT TTextureMgr::SaveFile(ID3D11DeviceContext* pContext,ID3D11Texture2D* pRes, T_STR name )
{
	HRESULT hr = S_OK;
	ComPtr<ID3D11Texture2D> tex=pRes;	
	GUID fileformat = GUID_ContainerFormatPng;
	WCHAR ext[_MAX_EXT];
	_wsplitpath_s(name.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, ext, _MAX_EXT);
	T_STR tExt = ext;
	if (tExt == L".dds" || tExt.empty())
	{
		if ( tExt.empty())
		{
			hr = DirectX::SaveDDSTextureToFile(pContext, tex.Get(), (name + L".dds").c_str());
		}
		else
		{
			hr = DirectX::SaveDDSTextureToFile(pContext, tex.Get(), name .c_str());
		}
	}
	else
	{
		if (tExt == L".bmp") fileformat = GUID_ContainerFormatBmp;
		if (tExt == L".png") fileformat = GUID_ContainerFormatPng;
		if (tExt == L".ico") fileformat = GUID_ContainerFormatIco;
		if (tExt == L".jpg") fileformat = GUID_ContainerFormatJpeg;
		if (tExt == L".tif") fileformat = GUID_ContainerFormatTiff;
		hr = DirectX::SaveWICTextureToFile(pContext, tex.Get(), fileformat, name.c_str());
	}
	
	if (pRes == nullptr)
	{
		tex->Release();
	}

	return hr;
}
HRESULT TTextureMgr::SaveFile(ID3D11DeviceContext* pContext,IDXGISwapChain* pSwapChain, T_STR name)
{
	HRESULT hr = S_OK;
	ComPtr<ID3D11Texture2D> tex;
	//time_t t;
	//struct tm *ts;
	//t = time(NULL);
	//ts = localtime(&t);
	//TCHAR buffer[80] = { 0, };
	//wcsftime(buffer, 80, L"%F:%T", ts);
	//name += buffer;
	hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)tex.GetAddressOf());
	
	if (SUCCEEDED(hr))
	{
		WCHAR ext[_MAX_EXT];
		_wsplitpath_s(name.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, ext, _MAX_EXT);
		GUID fileformat = GUID_ContainerFormatPng;
		T_STR tExt = ext;
		if (tExt == L".dds" || tExt.empty())
		{
			if (tExt.empty())
			{
				hr = DirectX::SaveDDSTextureToFile(pContext, tex.Get(), (name + L".dds").c_str());
			}
			else
			{
				hr = DirectX::SaveDDSTextureToFile(pContext, tex.Get(), name.c_str());
			}
		}
		else
		{
			if (tExt == L".bmp") fileformat = GUID_ContainerFormatBmp;
			if (tExt == L".png") fileformat = GUID_ContainerFormatPng;
			if (tExt == L".ico") fileformat = GUID_ContainerFormatIco;
			if (tExt == L".jpg") fileformat = GUID_ContainerFormatJpeg;
			if (tExt == L".tif") fileformat = GUID_ContainerFormatTiff;
			hr = DirectX::SaveWICTextureToFile(pContext, tex.Get(), fileformat, name.c_str());
		}
	}
	return hr;
}
TTextureMgr::TTextureMgr(void)
{
	m_pd3dDevice = NULL;
	m_iCurIndex = 0;
	TMap.clear();
	m_strDefaultPath = g_szDefaultDataPath;
}

TTextureMgr::~TTextureMgr(void)
{
	TTexture *pPoint;
	for (TemplateMapItor itor = TMap.begin(); itor != TMap.end(); itor++)
	{
		pPoint = (TTexture *)(*itor).second;
		pPoint->Release();
		SAFE_DEL(pPoint);
	}
	TMap.clear();
	m_iCurIndex = 0;
}
