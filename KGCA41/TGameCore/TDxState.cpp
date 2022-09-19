#include "TDxState.h"
ID3D11SamplerState* TDxState::g_pDefaultSSWrap = nullptr;
ID3D11SamplerState* TDxState::g_pDefaultSSMirror = nullptr;
ID3D11BlendState* TDxState::g_pDefaultBS = nullptr;
bool TDxState::SetState(ID3D11Device* pd3dDevice)
{
    HRESULT hr;
	// SS
	D3D11_SAMPLER_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;// 최근점 필터링
    sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    /* sd.BorderColor[0] = 0.5f;
    sd.BorderColor[1] = 0.5f;
    sd.BorderColor[2] = 0.5f;
    sd.BorderColor[3] = 1.0f;
    FLOAT MipLODBias;
    UINT MaxAnisotropy;
    D3D11_COMPARISON_FUNC ComparisonFunc;
    FLOAT BorderColor[4];
    FLOAT MinLOD;
    FLOAT MaxLOD;*/
	hr = pd3dDevice->CreateSamplerState(&sd, &g_pDefaultSSWrap);

    sd.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;// 최근점 필터링
    sd.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
    sd.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
    sd.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
    hr = pd3dDevice->CreateSamplerState(&sd, &g_pDefaultSSMirror);
	return true;
}
bool TDxState::Release() 
{
	if (g_pDefaultSSWrap) g_pDefaultSSWrap->Release();
    if (g_pDefaultSSMirror) g_pDefaultSSMirror->Release();
	return true;
}