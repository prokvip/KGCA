#include "TDxState.h"

ID3D11Device* TDxState::m_pd3dDevice = nullptr;
ID3D11SamplerState* TDxState::g_pDefaultSSWrap = nullptr;
ID3D11SamplerState* TDxState::g_pDefaultSSMirror = nullptr;
ID3D11BlendState* TDxState::g_pAlphaBlend = nullptr;
ID3D11BlendState* TDxState::g_pDualSourceBlend = nullptr;
ID3D11RasterizerState* TDxState::g_pDefaultRSWireFrame = nullptr;
ID3D11RasterizerState* TDxState::g_pDefaultRSSolid = nullptr;
ID3D11RasterizerState* TDxState::g_pRSSlopeScaledDepthBias = nullptr;
ID3D11DepthStencilState* TDxState::g_pDefaultDepthStencil = nullptr;
ID3D11DepthStencilState* TDxState::g_pDefaultDepthStencilAndNoWrite = nullptr;
ID3D11DepthStencilState* TDxState::g_pGreaterDepthStencil = nullptr;
ID3D11DepthStencilState* TDxState::g_pDisableDepthStencil = nullptr;

/// <summary>
///  Tool & Editer
/// </summary>
Microsoft::WRL::ComPtr<ID3D11BlendState>		TDxState::g_pBSEdit = 0;
Microsoft::WRL::ComPtr<ID3D11RasterizerState>	TDxState::g_pRSEdit = 0;
Microsoft::WRL::ComPtr<ID3D11SamplerState>      TDxState::g_pSSEdit = 0;
Microsoft::WRL::ComPtr<ID3D11DepthStencilState> TDxState::g_pDSSEdit = 0;
D3D11_BLEND_DESC			TDxState::g_BlendDesc = { 0, };
D3D11_RASTERIZER_DESC		TDxState::g_RasterizerDesc;
D3D11_SAMPLER_DESC			TDxState::g_SamplerDesc;
D3D11_DEPTH_STENCIL_DESC	TDxState::g_DepthStencilDesc = { 0, };
FLOAT TDxState::g_fBlendFactor[4] = { 0, };
UINT  TDxState::g_iMask = 0xff;
UINT TDxState::g_iSlot = 0;
UINT TDxState::g_iArray = 1;
UINT TDxState::g_iRef = 1;

HRESULT TDxState::SetBlendState(ID3D11Device* pd3dDevice,
    ID3D11DeviceContext* pContext,
    D3D11_BLEND_DESC& ds,
    const FLOAT fBlendFactor[],
    UINT iMask)
{
    HRESULT hr = S_OK;
    g_pBSEdit.Reset();
    g_BlendDesc = ds;
    memcpy(&g_fBlendFactor, fBlendFactor, sizeof(FLOAT) * 4);
    g_iMask = iMask;
    if (FAILED(hr = pd3dDevice->CreateBlendState(&ds, g_pBSEdit.GetAddressOf())))
    {
        return hr;
    }
    if (pContext != nullptr)
    {
        ApplyBS(pContext, g_pBSEdit.Get(), fBlendFactor, iMask);
    }
    return hr;
}
HRESULT TDxState::SetRasterizerState(ID3D11Device* pd3dDevice,
    ID3D11DeviceContext* pContext,
    D3D11_RASTERIZER_DESC& rsDesc)
{
    HRESULT hr = S_OK;
    g_pRSEdit.Reset();
    g_RasterizerDesc = rsDesc;
    if (FAILED(hr =
        pd3dDevice->CreateRasterizerState(&rsDesc, g_pRSEdit.GetAddressOf())))
    {
        return hr;
    }
    if (pContext != nullptr)
    {
        ApplyRS(pContext, g_pRSEdit.Get());
    }
    return hr;
}
HRESULT TDxState::SetSamplerState(ID3D11Device* pd3dDevice,
    ID3D11DeviceContext* pContext,
    D3D11_SAMPLER_DESC& sd,
    UINT iSlot,
    UINT iArray)
{
    HRESULT hr = S_OK;
    g_pSSEdit.Reset();
    g_SamplerDesc = sd;
    g_iSlot = iSlot;
    g_iArray = iArray;
    if (FAILED(hr = pd3dDevice->CreateSamplerState(
        &sd,
        g_pSSEdit.GetAddressOf())))
    {
        return hr;
    }
    if (pContext != nullptr)
    {
        ApplySS(pContext, g_pSSEdit.Get(), iSlot, iArray);
    }
    return hr;
}
HRESULT TDxState::SetDipthStencilState(ID3D11Device* pd3dDevice,
    ID3D11DeviceContext* pContext,
    D3D11_DEPTH_STENCIL_DESC& dsd,
    UINT iRef)
{
    HRESULT hr;
    g_pDSSEdit.Reset();
    g_DepthStencilDesc = dsd;
    g_iRef = iRef;
    if (FAILED(hr = pd3dDevice->CreateDepthStencilState(&dsd, g_pDSSEdit.GetAddressOf())))
    {
        return hr;
    }
    if (pContext != nullptr)
    {
        ApplyDSS(pContext, g_pDSSEdit.Get(), iRef);
    }
    return hr;
}

bool TDxState::SetState(ID3D11Device* pd3dDevice)
{
    HRESULT hr;
    m_pd3dDevice = pd3dDevice;
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

    sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;// 최근점 필터링
    sd.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
    sd.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
    sd.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
    hr = pd3dDevice->CreateSamplerState(&sd, &g_pDefaultSSMirror);

    D3D11_RASTERIZER_DESC rd;
    ZeroMemory(&rd, sizeof(rd));
    rd.DepthClipEnable = TRUE;
    rd.FillMode = D3D11_FILL_WIREFRAME;
    rd.CullMode = D3D11_CULL_BACK;
    pd3dDevice->CreateRasterizerState(&rd, 
        &g_pDefaultRSWireFrame);

    rd.FillMode = D3D11_FILL_SOLID;
    pd3dDevice->CreateRasterizerState(&rd,  &g_pDefaultRSSolid);


    rd.DepthBias = 500;
    rd.DepthBiasClamp = 0.0f;
    rd.SlopeScaledDepthBias = 1.0f;
    g_RasterizerDesc = rd;
    if (FAILED(hr = pd3dDevice->CreateRasterizerState(&rd,  &g_pRSSlopeScaledDepthBias)))
    {
        return false;
    }

    D3D11_BLEND_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    //bd.IndependentBlendEnable = TRUE;
    // 혼합 : 섞는다. 
    // 배경(목적지) d-RGBA : 백버퍼에 이미 랜더링 되어 있는 결과,
    // 현재 소스 :  s-RGBA : 지금 랜더링 하려는 객체, 픽셀쉐이더
    //  RGB 성분을 혼합하는 명령
    // 알파블랜딩 공식
    // finalColor = SrcColor*SrcAlpha+DestColor*(1.0f-SrcApha);
    // apha= 1.0f (불투명), alpha=0.0f (투명), alpha 0~1 (반투명)
    // finalColor = SrcColor*1.0f+DestColor*(1.0f-1.0f);
    // finalColor = SrcColor*0.0f+DestColor*(1.0f-0.0f);
    // finalColor = SrcColor*0.5f+DestColor*(1.0f-0.5f);
    bd.RenderTarget[0].BlendEnable = TRUE;
    bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    //  A 성분을 혼합하는 명령
    // finalAlpha = SrcAlpha*1.0f+DestAlpha*0.0f;
    bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

    bd.RenderTarget[0].RenderTargetWriteMask =
        D3D11_COLOR_WRITE_ENABLE_ALL;
    pd3dDevice->CreateBlendState(&bd, &g_pAlphaBlend);


    bd.AlphaToCoverageEnable = FALSE; // 알파 (0.5) 기준 알파테스팅 활성화 여부
    bd.IndependentBlendEnable = TRUE; // TRUE: 모든 랜더타켓 상태 적용, FALSE:0번만 적용됨.
    bd.RenderTarget[0].BlendEnable = TRUE;
    bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    bd.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
    bd.RenderTarget[0].DestBlend = D3D11_BLEND_SRC1_COLOR;
    pd3dDevice->CreateBlendState(&bd, &g_pDualSourceBlend);

    D3D11_DEPTH_STENCIL_DESC dsd;
    ZeroMemory(&dsd, sizeof(dsd));
    dsd.DepthEnable = TRUE;
    // 0.5f * mask(1) --> output(0.5)
    dsd.DepthWriteMask= D3D11_DEPTH_WRITE_MASK_ALL;
    dsd.DepthFunc= D3D11_COMPARISON_LESS_EQUAL;
   /* BOOL StencilEnable;
    UINT8 StencilReadMask;
    UINT8 StencilWriteMask;
    D3D11_DEPTH_STENCILOP_DESC FrontFace;
    D3D11_DEPTH_STENCILOP_DESC BackFace;*/
    hr=pd3dDevice->CreateDepthStencilState(&dsd, &g_pDefaultDepthStencil);
    dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    hr=pd3dDevice->CreateDepthStencilState(&dsd, &g_pDefaultDepthStencilAndNoWrite);
    dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    
    dsd.DepthFunc = D3D11_COMPARISON_GREATER;
    hr = pd3dDevice->CreateDepthStencilState(&dsd, &g_pGreaterDepthStencil);
    dsd.DepthEnable = FALSE;
    hr = pd3dDevice->CreateDepthStencilState(&dsd, &g_pDisableDepthStencil);
    
	return true;
}
bool TDxState::Release() 
{
    if (g_pDefaultDepthStencilAndNoWrite)g_pDefaultDepthStencilAndNoWrite->Release();
    if (g_pAlphaBlend) g_pAlphaBlend->Release();
    if (g_pDualSourceBlend) g_pDualSourceBlend->Release();
	if (g_pDefaultSSWrap) g_pDefaultSSWrap->Release();
    if (g_pDefaultSSMirror) g_pDefaultSSMirror->Release();
    if (g_pDefaultRSSolid)g_pDefaultRSSolid->Release();
    if (g_pRSSlopeScaledDepthBias)g_pRSSlopeScaledDepthBias->Release();
    if (g_pDefaultRSWireFrame)g_pDefaultRSWireFrame->Release();   
    if (g_pDefaultDepthStencil)g_pDefaultDepthStencil->Release();
    if (g_pGreaterDepthStencil)g_pGreaterDepthStencil->Release();
    if (g_pDisableDepthStencil)g_pDisableDepthStencil->Release();
	return true;
}