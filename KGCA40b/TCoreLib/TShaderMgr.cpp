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
		if (m_pErrorMsgs)
		{
			MessageBoxA(NULL,
				(char*)m_pErrorMsgs->GetBufferPointer(),
				"ERROR", MB_OK);
			m_pErrorMsgs->Release();
		}
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
		if (m_pErrorMsgs)
		{
			MessageBoxA(NULL,
				(char*)m_pErrorMsgs->GetBufferPointer(),
				"ERROR", MB_OK);
			m_pErrorMsgs->Release();
		}
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
	std::wstring name = Splitpath(filename, to_mw(entry));
	TShader* pData = GetPtr(name);
	if (pData != nullptr)
	{
		return pData;
	}
	std::shared_ptr<TShader> pNewData = std::make_shared<TShader>();
	if (!pNewData->CreateVertexShader(pd3dDevice, filename, entry))
	{		
		return nullptr;
	}
	pNewData->m_csName = name;
	m_list.insert(make_pair(pNewData->m_csName, pNewData));
	m_iIndex++;
	return pNewData.get();
}
TShader* TShaderMgr::CreatePixelShader(ID3D11Device* pd3dDevice,
	std::wstring filename, std::string entry)
{
	std::wstring name = Splitpath(filename, to_mw(entry));
	TShader* pData = GetPtr(name);
	if (pData != nullptr)
	{
		return pData;
	}
	auto pNewData = std::make_shared<TShader>();
	if (!pNewData->CreatePixelShader(pd3dDevice, filename, entry))
	{
		return nullptr;
	}
	pNewData->m_csName = name;
	m_list.insert(make_pair(pNewData->m_csName, pNewData));
	m_iIndex++;
	return pNewData.get();
}
TShaderMgr::TShaderMgr()
{
	m_iIndex = 0;
}
TShaderMgr::~TShaderMgr()
{
	Release();
}