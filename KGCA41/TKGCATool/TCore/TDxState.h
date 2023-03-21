#pragma once
#include "TStd.h"
class TDxState
{
	static ID3D11Device* m_pd3dDevice;
public:
	//tool
	static Microsoft::WRL::ComPtr<ID3D11BlendState>	g_pBSEdit;
	static D3D11_BLEND_DESC  g_BlendDesc;
	static FLOAT g_fBlendFactor[4];
	static UINT  g_iMask;
	static Microsoft::WRL::ComPtr < ID3D11RasterizerState>  g_pRSEdit;
	static D3D11_RASTERIZER_DESC  g_RasterizerDesc;
	static Microsoft::WRL::ComPtr < ID3D11SamplerState>   g_pSSEdit;
	static D3D11_SAMPLER_DESC  g_SamplerDesc;
	static UINT g_iSlot;
	static UINT g_iArray;
	static Microsoft::WRL::ComPtr < ID3D11DepthStencilState>   g_pDSSEdit;
	static D3D11_DEPTH_STENCIL_DESC  g_DepthStencilDesc;
	static UINT g_iRef;
public:
	static ID3D11SamplerState* g_pDefaultSSWrap;
	static ID3D11SamplerState* g_pDefaultSSMirror;
	static ID3D11RasterizerState* g_pDefaultRSWireFrame;
	static ID3D11RasterizerState* g_pDefaultRSSolid;
	static ID3D11RasterizerState* g_pRSSlopeScaledDepthBias;
	static ID3D11BlendState* g_pAlphaBlend;
	static ID3D11BlendState* g_pDualSourceBlend;
	static ID3D11DepthStencilState* g_pDefaultDepthStencil;
	static ID3D11DepthStencilState* g_pDefaultDepthStencilAndNoWrite;
	static ID3D11DepthStencilState* g_pGreaterDepthStencil;
	static ID3D11DepthStencilState* g_pDisableDepthStencil;
	static bool SetState(ID3D11Device* pd3dDevice);
	static bool Release();
	
	static HRESULT SetBlendState(ID3D11Device* pd3dDevice,
		ID3D11DeviceContext* pContext,
		D3D11_BLEND_DESC& ds,
		const FLOAT fBlendFactor[] = 0,
		UINT iMask = 0xff);
	static HRESULT SetRasterizerState(ID3D11Device* pd3dDevice,
		ID3D11DeviceContext* pContext,
		D3D11_RASTERIZER_DESC& rsDesc);
	static HRESULT SetSamplerState(ID3D11Device* pd3dDevice,
		ID3D11DeviceContext* pContext,
		D3D11_SAMPLER_DESC& sd,
		UINT iSlot = 0,
		UINT iArray = 1);
	static HRESULT SetDipthStencilState(ID3D11Device* pd3dDevice,
		ID3D11DeviceContext* pContext,
		D3D11_DEPTH_STENCIL_DESC& dsd,
		UINT iRef = 1);
};
static void ApplyRS(ID3D11DeviceContext* pContext,
	ID3D11RasterizerState* pState)
{
	assert(pContext);
	pContext->RSSetState(pState);
}
static void ApplyDSS(ID3D11DeviceContext* pContext,
	ID3D11DepthStencilState* pDepthStencilState,
	UINT iRef = 0x01)
{
	assert(pContext);
	pContext->OMSetDepthStencilState(pDepthStencilState, iRef);

};
static void ApplyBS(ID3D11DeviceContext* pContext,
	ID3D11BlendState* pBlendState,
	const FLOAT fBlendFactor[] = 0,
	UINT iMask = 0xffffffff)
{
	assert(pContext);
	pContext->OMSetBlendState(pBlendState, fBlendFactor, iMask);
}

static void ApplySS(ID3D11DeviceContext* pContext, ID3D11SamplerState* pSamplerState,
	UINT iSlot = 0, UINT iArray = 1)
{
	assert(pContext);
	pContext->PSSetSamplers(iSlot, iArray, &pSamplerState);
}

