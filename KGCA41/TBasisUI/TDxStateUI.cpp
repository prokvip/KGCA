#include "TDxStateUI.h"
namespace TUI
{
    ID3D11SamplerState* TDxStateUI::g_pUI_WrapSS = nullptr;
    ID3D11SamplerState* TDxStateUI::g_pUI_MirrorSS = nullptr;
    ID3D11BlendState* TDxStateUI::g_pUI_AlphaBlend = nullptr;

    ID3D11RasterizerState* TDxStateUI::g_pUI_WireFrameRS = nullptr;
    ID3D11RasterizerState* TDxStateUI::g_pUI_SolidRS = nullptr;

    ID3D11DepthStencilState* TDxStateUI::g_pDSSDepthEnable = 0;
    ID3D11DepthStencilState* TDxStateUI::g_pDSSDepthDisable = 0;
    ID3D11DepthStencilState* TDxStateUI::g_pDSSDepthEnableNoWrite = 0;
    ID3D11DepthStencilState* TDxStateUI::g_pDSSDepthDisableNoWrite = 0;
    ID3D11DepthStencilState* TDxStateUI::g_pDSSDepthAlways = 0;

    bool TDxStateUI::SetState(ID3D11Device* pd3dDevice)
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
        hr = pd3dDevice->CreateSamplerState(&sd, &g_pUI_WrapSS);

        sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;// 최근점 필터링
        sd.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
        sd.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
        sd.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
        hr = pd3dDevice->CreateSamplerState(&sd, &g_pUI_MirrorSS);

        D3D11_RASTERIZER_DESC rd;
        ZeroMemory(&rd, sizeof(rd));
        rd.DepthClipEnable = TRUE;
        rd.FillMode = D3D11_FILL_WIREFRAME;
        rd.CullMode = D3D11_CULL_NONE;
        pd3dDevice->CreateRasterizerState(&rd,
            &g_pUI_WireFrameRS);

        rd.FillMode = D3D11_FILL_SOLID;
        pd3dDevice->CreateRasterizerState(&rd,
            &g_pUI_SolidRS);


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
        pd3dDevice->CreateBlendState(&bd, &g_pUI_AlphaBlend);


        // 깊이버퍼 상태값 세팅
        D3D11_DEPTH_STENCIL_DESC dsDescDepth;
        ZeroMemory(&dsDescDepth, sizeof(D3D11_DEPTH_STENCIL_DESC));
        dsDescDepth.DepthEnable = TRUE;
        dsDescDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        dsDescDepth.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
        dsDescDepth.StencilEnable = FALSE;
        dsDescDepth.StencilReadMask = 1;
        dsDescDepth.StencilWriteMask = 1;
        dsDescDepth.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
        dsDescDepth.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
        dsDescDepth.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        dsDescDepth.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

        // 디폴트 값
        dsDescDepth.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
        dsDescDepth.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        dsDescDepth.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        dsDescDepth.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
        if (FAILED(hr = pd3dDevice->CreateDepthStencilState(&dsDescDepth, &g_pDSSDepthEnable)))
        {
            return hr;
        }
        // 깊이 버퍼 비교 비 활성화.
        dsDescDepth.DepthEnable = FALSE;
        if (FAILED(hr = pd3dDevice->CreateDepthStencilState(&dsDescDepth, &g_pDSSDepthDisable)))
        {
            return hr;
        }
        dsDescDepth.DepthEnable = TRUE;
        dsDescDepth.DepthFunc = D3D11_COMPARISON_ALWAYS;//DepthEnable = TRUE 이어야 한다.
        if (FAILED(hr = pd3dDevice->CreateDepthStencilState(&dsDescDepth, &g_pDSSDepthAlways)))
        {
            return hr;
        }
        return true;
    }
    bool TDxStateUI::Release()
    {
        if (g_pUI_AlphaBlend)
        {
            g_pUI_AlphaBlend->Release();
            g_pUI_AlphaBlend = nullptr;
        }
        if (g_pUI_WrapSS)
        {
            g_pUI_WrapSS->Release();
            g_pUI_WrapSS = nullptr;
        }
        if (g_pUI_MirrorSS)
        {
            g_pUI_MirrorSS->Release();
            g_pUI_MirrorSS = nullptr;
        }
        if (g_pUI_SolidRS)
        {
            g_pUI_SolidRS->Release();
            g_pUI_SolidRS = nullptr;
        }
        if (g_pUI_WireFrameRS)
        {
            g_pUI_WireFrameRS->Release();
            g_pUI_WireFrameRS = nullptr;
        }
        if (g_pDSSDepthEnable)
        {
            g_pDSSDepthEnable->Release();
            g_pDSSDepthEnable = nullptr;
        }
        if (g_pDSSDepthDisable)
        {
            g_pDSSDepthDisable->Release();
            g_pDSSDepthDisable = nullptr;
        }
        if (g_pDSSDepthEnableNoWrite)
        {
            g_pDSSDepthEnableNoWrite->Release();
            g_pDSSDepthEnableNoWrite = nullptr;
        }
        if (g_pDSSDepthDisableNoWrite)
        {
            g_pDSSDepthDisableNoWrite->Release();
            g_pDSSDepthDisableNoWrite = nullptr;
        }
        if (g_pDSSDepthAlways)
        {
            g_pDSSDepthAlways->Release();
            g_pDSSDepthAlways = nullptr;
        }

        return true;
    }
};