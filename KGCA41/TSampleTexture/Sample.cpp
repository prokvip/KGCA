#include "Sample.h"
bool Sample::Init()
{	
	std::wstring shaderfilename = L"../../data/shader/DefaultShape.txt";

	m_Object.Create(	m_pd3dDevice, m_pImmediateContext,
						shaderfilename, shaderfilename);

	HRESULT hr = DirectX::CreateWICTextureFromFile(
		m_pd3dDevice,
		m_pImmediateContext,
		L"../../data/kgcabk.bmp",
		(ID3D11Resource**)&m_pTexture,
		&m_pTextureSRV);
	if (FAILED(hr))
	{
		hr = DirectX::CreateDDSTextureFromFile(
			m_pd3dDevice,
			m_pImmediateContext,
			L"../../data/kgcabk.bmp",
			(ID3D11Resource**)&m_pTexture,
			&m_pTextureSRV);
	}

	m_pTexture->GetDesc(&m_Desc);

	D3D11_SAMPLER_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;// 최근점 필터링
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	hr = m_pd3dDevice->CreateSamplerState(&sd, &g_pDefaultSS);
	return true;
}
bool Sample::Frame()
{		
	m_Object.Frame();
	return true;
}
bool Sample::Render()
{	
	m_pImmediateContext->PSSetSamplers(0, 1, &g_pDefaultSS);
	m_pImmediateContext->PSSetShaderResources(0, 1, &m_pTextureSRV);
	m_Object.Render();
	return true;
}
bool Sample::Release()
{	
	if(g_pDefaultSS)g_pDefaultSS->Release();
	if (m_pTexture)m_pTexture->Release();
	if (m_pTextureSRV)m_pTextureSRV->Release();
	m_Object.Release();
	return true;
}

GAME_RUN(TSampleTexture, 800,600)