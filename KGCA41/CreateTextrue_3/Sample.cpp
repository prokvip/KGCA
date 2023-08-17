#include "Sample.h"
bool  Sample::Init() 
{   
    // alpha blending
    D3D11_BLEND_DESC bsd;
    ZeroMemory(&bsd, sizeof(bsd));
    bsd.RenderTarget[0].BlendEnable = true;
    bsd.RenderTarget[0].SrcBlend =  D3D11_BLEND_SRC_ALPHA;
    bsd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    bsd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    // 알파블랜딩 공식 - 소스(float4(0,1,0,0.5f)), 대상(1,0,0,1)
    //finalcolor = SrcColor* SrcBlend BlendOP(+) DestColor*DestBlend
    //           = Scrcolor* alphaValue + DestColor * (1.0f-alphaValue)
    //           = 0,1,0 * 0.5f + 1,0,0 * (1.0f-0.5f)
    // //        만약 t= 0.0f; 결과 => 배경만 나온다.
    // //        만약 t= 1.0f; 결과 => 소스만 나온다.
    //           만약 t= 0.5f; 결과 - >소스컬러*0.5 + 배경컬러*0.5f
    //           = 0,1,0 * t + 1,0,0 * (1.0f-t)
    //           =  r*t,b*t,b*t + r*(1.0f-t),b*(1.0f-t),b*(1.0f-t)

    bsd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    bsd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    bsd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

    bsd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    m_pDevice->CreateBlendState(&bsd, &m_AlphaBlend);

    m_texMgr.Set(m_pDevice, m_pImmediateContext);
    m_shaderMgr.Set(m_pDevice, m_pImmediateContext);

    std::wstring texname[] = { 
        L"../../res/kgcabk.bmp", // path=../../res/, name = kgcabk.bmp
        L"../../res/ade4.dds" ,
        L"../../res/mapcontrol.png",
        L"../../res/103.tga"
    };
    for (int iObj = 0; iObj < 10; iObj++)
    {
        TObject* pObj = new TObject;
        pObj->Set(m_pDevice, m_pImmediateContext);
        pObj->Create( m_texMgr, texname[iObj % 4],
                      m_shaderMgr, L"Plane.hlsl");
        m_ObjList.push_back(pObj);
    }
    return true; 
}
bool  Sample::Frame() 
{ 
    for (auto obj : m_ObjList)
    {
        obj->Frame();
    }
    return true; 
}
bool  Sample::Render() 
{    
    m_pImmediateContext->OMSetBlendState(m_AlphaBlend, 0, -1);
    for (auto obj : m_ObjList)
    {
        obj->Render();
    }
    return true; 
}
bool  Sample::Release()
{   
    for (auto obj : m_ObjList)
    {
        obj->Release();
        delete obj;
    }
    m_ObjList.clear();  
    m_AlphaBlend->Release();
    return true; 
}

TGAME(L"kgca", 800, 600)