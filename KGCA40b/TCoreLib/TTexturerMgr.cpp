#include "TTextureMgr.h"
bool	TTexture::Load(ID3D11Device* pd3dDevice, std::wstring filename)
{
	HRESULT hr = DirectX::CreateWICTextureFromFile(
		pd3dDevice,
		filename.c_str(),
		(ID3D11Resource**)&m_pTexture,
		&m_pSRV);
	if (FAILED(hr))
	{
		hr = DirectX::CreateDDSTextureFromFile(
			pd3dDevice,
			filename.c_str(),
			(ID3D11Resource**)&m_pTexture,
			&m_pSRV);
		if (FAILED(hr))
		{
			return false;
		}
	}
	m_pTexture->GetDesc(&m_TextureDesc);
	return true;
}
bool	TTexture::Init()
{
	return true;
}
bool	TTexture::Frame()
{	
	return true;
}
bool	TTexture::Render()
{
	return true;
}
bool	TTexture::Release()
{
	if (m_pTexture) m_pTexture->Release();
	if (m_pSRV) m_pSRV->Release();
	return true;
}
TTexture::TTexture()
{

}
TTexture::~TTexture()
{

}

TTexture* TTextureMgr::GetPtr(std::wstring key)
{
	auto iter = m_list.find(key);
	if (iter != m_list.end())
	{
		return (*iter).second;
	}
	return nullptr;
}
TTexture* TTextureMgr::Load(std::wstring filename)
{
	TCHAR szFileName[MAX_PATH] = { 0, };
	TCHAR Dirve[MAX_PATH] = { 0, };
	TCHAR Dir[MAX_PATH] = { 0, };
	TCHAR FileName[MAX_PATH] = { 0, };
	TCHAR FileExt[MAX_PATH] = { 0, };

	std::wstring fullpathname = filename;
	_tsplitpath_s(fullpathname.c_str(), Dirve, Dir, FileName, FileExt);
	std::wstring name = FileName;
	name += FileExt;

	for (auto data : m_list)
	{
		if (data.second->m_csName == name)
		{
			return data.second;
		}
	}

	

	TTexture* pData = new TTexture;
	if( pData->Load(m_pd3dDevice,filename) ==false)
	{
		delete pData;
		return nullptr;
	}
	pData->m_csName = name;
	m_list.insert(make_pair(name, pData));
	m_iIndex++;
	return pData;
}
bool	TTextureMgr::Init()
{
	return true;
}
bool	TTextureMgr::Frame()
{
	return true;
}
bool	TTextureMgr::Render()
{
	return true;
}
bool	TTextureMgr::Release()
{
	for (auto data : m_list)
	{
		data.second->Release();
		delete data.second;
	}
	m_list.clear();
	return true;
}
TTextureMgr::TTextureMgr()
{
	m_iIndex = 0;
}
TTextureMgr::~TTextureMgr()
{
	Release();
}