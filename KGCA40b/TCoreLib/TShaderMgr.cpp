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

TShader* TShaderMgr::CreateVertexShader(ID3D11Device* pd3dDevice,
	std::wstring filename, std::string entry)
{
	TShader* pData = CheckLoad(filename, to_mw(entry));
	if (pData != nullptr)
	{
		return pData;
	}
	pData = new TShader;
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
	TShader* pData = CheckLoad(filename, to_mw(entry));
	if (pData != nullptr)
	{
		return pData;
	}
	pData = new TShader;
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