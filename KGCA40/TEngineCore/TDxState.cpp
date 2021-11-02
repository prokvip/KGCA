#include "TDxState.h"

ID3D11DepthStencilState* TDxState::g_pCurrentDSS = nullptr;
ID3D11SamplerState* TDxState::g_pCurrentSS[3] = { nullptr,nullptr,nullptr };
ID3D11RasterizerState* TDxState::g_pCurrentRS = nullptr;

ID3D11DepthStencilState* TDxState::g_pLessEqualDSS= nullptr;
ID3D11SamplerState* TDxState::g_pClampSS = nullptr;
ID3D11SamplerState* TDxState::g_pWrapSS = nullptr;
ID3D11RasterizerState* TDxState::g_pRSSolid = nullptr;
ID3D11RasterizerState* TDxState::g_pRSWireFrame = nullptr;
bool TDxState::Init()
{
	CreateDepthStencilState();
	CreateSamplerState();
	CreateRasterizerState();
	return true;
}
HRESULT TDxState::CreateDepthStencilState()
{
	HRESULT hr = S_OK;
	D3D11_DEPTH_STENCIL_DESC sd;
	ZeroMemory(&sd, sizeof(D3D11_DEPTH_STENCIL_DESC));
	sd.DepthEnable = TRUE;
	sd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	sd.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	hr = g_pd3dDevice->CreateDepthStencilState(&sd, &g_pLessEqualDSS);
	if (FAILED(hr))
	{
		return hr;
	}	
	return hr;
}

HRESULT	TDxState::CreateSamplerState()
{
	D3D11_SAMPLER_DESC sd;
	ZeroMemory(&sd, sizeof(D3D11_SAMPLER_DESC));
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	HRESULT hr = g_pd3dDevice->CreateSamplerState(&sd, 
		&g_pClampSS);

	ZeroMemory(&sd, sizeof(D3D11_SAMPLER_DESC));
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	hr = g_pd3dDevice->CreateSamplerState(&sd,&g_pWrapSS);
	return hr;
}

HRESULT    TDxState::CreateRasterizerState()
{
	HRESULT hr = S_OK;
	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));
	rd.FillMode = D3D11_FILL_WIREFRAME;
	rd.CullMode = D3D11_CULL_BACK;
	hr = g_pd3dDevice->CreateRasterizerState(&rd, &g_pRSWireFrame);
	ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_BACK;
	hr = g_pd3dDevice->CreateRasterizerState(&rd, &g_pRSSolid);
	return hr;
}
bool TDxState::Release()
{
	SAFE_RELEASE(g_pLessEqualDSS);
	SAFE_RELEASE(g_pClampSS);
	SAFE_RELEASE(g_pWrapSS);
	SAFE_RELEASE(g_pRSSolid);
	SAFE_RELEASE(g_pRSWireFrame);
	return true;
}
