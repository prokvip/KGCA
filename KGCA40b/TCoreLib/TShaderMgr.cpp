#include "TShaderMgr.h"
bool    TShader::CreateVertexShader(ID3D11Device* pd3dDevice, 
	std::wstring filename,
	std::string entry)
{
	HRESULT hr = D3DCompileFromFile(
		filename.c_str(),
		NULL,
		NULL,
		entry.c_str(),
		"vs_5_0",
		0,
		0,
		&m_pVSCodeResult,
		&m_pErrorMsgs
	);
	if (FAILED(hr))
	{
		MessageBoxA(NULL,
			(char*)m_pErrorMsgs->GetBufferPointer(),
			"ERROR", MB_OK);
		if (m_pErrorMsgs) m_pErrorMsgs->Release();
		return false;
	}
	hr = pd3dDevice->CreateVertexShader(
		m_pVSCodeResult->GetBufferPointer(),
		m_pVSCodeResult->GetBufferSize(),
		NULL,
		&m_pVertexShader);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}
bool    TShader::CreatePixelShader(ID3D11Device* pd3dDevice, 
	std::wstring filename,
	std::string entry)
{
	HRESULT hr = D3DCompileFromFile(
		filename.c_str(),
		NULL,
		NULL,
		entry.c_str(),
		"ps_5_0",
		0,
		0,
		&m_pPSCodeResult,
		&m_pErrorMsgs
	);
	if (FAILED(hr))
	{
		MessageBoxA(NULL,
			(char*)m_pErrorMsgs->GetBufferPointer(),
			"ERROR", MB_OK);
		if (m_pErrorMsgs) m_pErrorMsgs->Release();
		return false;
	}
	hr = pd3dDevice->CreatePixelShader(
		m_pPSCodeResult->GetBufferPointer(),
		m_pPSCodeResult->GetBufferSize(),
		NULL,
		&m_pPixelShader);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}
bool	TShader::Load(ID3D11Device* pd3dDevice, std::wstring filename)
{
	if (!CreateVertexShader(pd3dDevice,filename))
	{
		return false;
	}
	if (!CreatePixelShader(pd3dDevice, filename))
	{
		return false;
	}
	return true;
}
bool	TShader::Init()
{
	return true;
}
bool	TShader::Frame() 
{	
	return true;
}
bool	TShader::Render() 
{
	return true;
}
bool	TShader::Release() 
{
	if (m_pVertexShader) m_pVertexShader->Release();
	if (m_pPixelShader) m_pPixelShader->Release();
	m_pVertexShader = nullptr;
	m_pPixelShader = nullptr;
	return true;
}
TShader::TShader()
{

}
TShader::~TShader()
{

}


/// <summary>
/// 
/// </summary>
/// <param name="filename"></param>
/// <returns></returns>
TShader* TShaderMgr::GetPtr(std::wstring key)
{
	auto iter = m_list.find(key);
	if (iter != m_list.end())
	{
		return (*iter).second;
	}
	return nullptr;
}
TShader* TShaderMgr::Load(std::wstring filename)
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

	TShader* pData = new TShader;		
	if (pData->Load(m_pd3dDevice, filename) == false)
	{
		delete pData;
		return nullptr;
	}
	pData->m_csName = name;
	m_list.insert(make_pair(name, pData));
	m_iIndex++;
	return pData;
}
bool	TShaderMgr::Init()
{
	return true;
}
bool	TShaderMgr::Frame()
{
	return true;
}
bool	TShaderMgr::Render()
{
	return true;
}
bool	TShaderMgr::Release()
{
	for (auto data : m_list)
	{
		data.second->Release();
		delete data.second;
	}
	m_list.clear();
	return true;
}
TShader* TShaderMgr::CreateVertexShader(ID3D11Device* pd3dDevice,
	std::wstring filename, std::string entry)
{
	TShader* pData = new TShader;
	if (!pData->CreateVertexShader(pd3dDevice, filename, entry))
	{		
		delete pData;
		return nullptr;
	}
	pData->m_csName = filename + to_mw(entry);
	m_list.insert(make_pair(pData->m_csName, pData));
	m_iIndex++;
	return pData;
}
TShader* TShaderMgr::CreatePixelShader(ID3D11Device* pd3dDevice,
	std::wstring filename, std::string entry)
{
	TShader* pData = new TShader;
	if (!pData->CreatePixelShader(pd3dDevice, filename, entry))
	{
		delete pData;
		return nullptr;
	}
	pData->m_csName = filename + to_mw(entry);
	m_list.insert(make_pair(pData->m_csName, pData));
	m_iIndex++;
	return pData;
}
TShaderMgr::TShaderMgr()
{
	m_iIndex = 0;
}
TShaderMgr::~TShaderMgr()
{
	Release();
}