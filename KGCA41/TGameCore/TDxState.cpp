#include "TDxState.h"
ID3D11SamplerState* TDxState::g_pDefaultSSWrap = nullptr;
ID3D11SamplerState* TDxState::g_pDefaultSSMirror = nullptr;
ID3D11BlendState* TDxState::g_pAlphaBlend = nullptr;
ID3D11BlendState* TDxState::g_pDualSourceBlend = nullptr;
ID3D11RasterizerState* TDxState::g_pDefaultRSWireFrame = nullptr;
ID3D11RasterizerState* TDxState::g_pDefaultRSSolid = nullptr;
ID3D11RasterizerState* TDxState::g_pRSSlopeScaledDepthBias=nullptr;

ID3D11DepthStencilState* TDxState::g_pDefaultDepthStencil = nullptr;
ID3D11DepthStencilState* TDxState::g_pDefaultDepthStencilAndNoWrite = nullptr;
ID3D11DepthStencilState* TDxState::g_pGreaterDepthStencil = nullptr;
ID3D11DepthStencilState* TDxState::g_pDisableDepthStencil = nullptr;
bool TDxState::SetState(ID3D11Device* pd3dDevice)
{
    HRESULT hr;
	// SS
	D3D11_SAMPLER_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;// �ֱ��� ���͸�
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

    sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;// �ֱ��� ���͸�
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
    pd3dDevice->CreateRasterizerState(&rd,
        &g_pDefaultRSSolid);

    rd.DepthBias = 20000;
    rd.DepthBiasClamp = 0.0f;
    rd.SlopeScaledDepthBias = 1.0f;
    if (FAILED(hr = pd3dDevice->CreateRasterizerState(&rd,
        &g_pRSSlopeScaledDepthBias)))
        return hr;

    D3D11_BLEND_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    //bd.IndependentBlendEnable = TRUE;
    // ȥ�� : ���´�. 
    // ���(������) d-RGBA : ����ۿ� �̹� ������ �Ǿ� �ִ� ���,
    // ���� �ҽ� :  s-RGBA : ���� ������ �Ϸ��� ��ü, �ȼ����̴�
    //  RGB ������ ȥ���ϴ� ���
    // ���ĺ��� ����
    // finalColor = SrcColor*SrcAlpha+DestColor*(1.0f-SrcApha);
    // apha= 1.0f (������), alpha=0.0f (����), alpha 0~1 (������)
    // finalColor = SrcColor*1.0f+DestColor*(1.0f-1.0f);
    // finalColor = SrcColor*0.0f+DestColor*(1.0f-0.0f);
    // finalColor = SrcColor*0.5f+DestColor*(1.0f-0.5f);
    bd.RenderTarget[0].BlendEnable = TRUE;
    bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    //  A ������ ȥ���ϴ� ���
    // finalAlpha = SrcAlpha*1.0f+DestAlpha*0.0f;
    bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

    bd.RenderTarget[0].RenderTargetWriteMask =
        D3D11_COLOR_WRITE_ENABLE_ALL;
    pd3dDevice->CreateBlendState(&bd, &g_pAlphaBlend);


    bd.AlphaToCoverageEnable = FALSE; // ���� (0.5) ���� �����׽��� Ȱ��ȭ ����
    bd.IndependentBlendEnable = TRUE; // TRUE: ��� ����Ÿ�� ���� ����, FALSE:0���� �����.
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
    if (g_pDefaultRSWireFrame)g_pDefaultRSWireFrame->Release();
    if (g_pRSSlopeScaledDepthBias)g_pRSSlopeScaledDepthBias->Release();
    if (g_pDefaultDepthStencil)g_pDefaultDepthStencil->Release();
    if (g_pGreaterDepthStencil)g_pGreaterDepthStencil->Release();
    if (g_pDisableDepthStencil)g_pDisableDepthStencil->Release();
	return true;
}