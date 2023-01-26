#include "TDxStateUI.h"
namespace TUI
{
    ID3D11SamplerState* TDxStateUI::g_pUI_WrapSS = nullptr;
    ID3D11SamplerState* TDxStateUI::g_pUI_MirrorSS = nullptr;
    ID3D11BlendState* TDxStateUI::g_pUI_AlphaBlend = nullptr;

    ID3D11RasterizerState* TDxStateUI::g_pUI_WireFrameRS = nullptr;
    ID3D11RasterizerState* TDxStateUI::g_pUI_SolidRS = nullptr;
    bool TDxStateUI::SetState(ID3D11Device* pd3dDevice)
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
        hr = pd3dDevice->CreateSamplerState(&sd, &g_pUI_WrapSS);

        sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;// �ֱ��� ���͸�
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
        pd3dDevice->CreateBlendState(&bd, &g_pUI_AlphaBlend);

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
        return true;
    }
};