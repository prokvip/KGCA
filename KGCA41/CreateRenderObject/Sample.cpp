#include "Sample.h"
#include "TShaderMgr.h"
#include "TTextureMgr.h"

void  Sample::CreateBlendState()
{
    // alpha blending
    D3D11_BLEND_DESC bsd;
    ZeroMemory(&bsd, sizeof(bsd));
    bsd.RenderTarget[0].BlendEnable = true;
    bsd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    bsd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    bsd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    // 알파블랜딩 공식 - 소스(float4(0,1,0,0.5f)), 대상(1,0,0,1)
    //finalcolor(RGB) = SrcColor* SrcBlend   + DestColor*DestBlend
    //           = Scrcolor* alphaValue + DestColor * (1.0f-alphaValue)
    //           = 0,1,0 * 0.5f + 1,0,0 * (1.0f-0.5f)
    // //        만약 t= 0.0f; 결과 => 배경만 나온다.
    // //        만약 t= 1.0f; 결과 => 소스만 나온다.
    //           만약 t= 0.5f; 결과 - >소스컬러*0.5 + 배경컬러*0.5f
    //           = 0,1,0 * t + 1,0,0 * (1.0f-t)
    //           =  r*t,b*t,b*t + r*(1.0f-t),b*(1.0f-t),b*(1.0f-t)

    // (A)
    bsd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    bsd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    bsd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

    bsd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    m_pDevice->CreateBlendState(&bsd, &m_AlphaBlend);
}
bool  Sample::Init()
{
    CreateBlendState();  

    std::wstring texname[] = { 
        //L"../../res/ground.png", // path=../../res/, name = kgcabk.bmp
        L"../../res/ade4.dds" ,
        L"../../res/mapcontrol.png",
        L"../../res/103.tga"
    };
   
    srand(time(NULL));
    m_pMapObj = new TPlaneObj;
    m_pMapObj->Set(m_pDevice, m_pImmediateContext);
    m_pMapObj->SetPos({ 0.0f,0.0f ,0.0f });
    m_pMapObj->SetScale(TVector3(g_fMapSizeX, g_fMapSizeY, 1.0f));
    m_pMapObj->Create(L"../../res/topdownmap.jpg", L"Plane.hlsl");
    

    m_pPlayer = new TPlayer;
    m_pPlayer->Set(m_pDevice, m_pImmediateContext);
    m_pPlayer->SetPos({-g_fMapSizeX,0.0f ,0.0f });
    m_pPlayer->SetScale(TVector3(50.0f, 50.0f, 1.0f));
    TVector2 rt = { m_pPlayer->m_vPos.x, m_pPlayer->m_vPos.y };
    m_pPlayer->SetRect(rt,
                       m_pPlayer->m_vScale.x*2.0f, m_pPlayer->m_vScale.y*2.0f);
    m_pPlayer->Create(  L"../../res/blackhole2.png", L"Plane.hlsl");

    m_MainCamera.Create(m_pPlayer->m_vPos,
                       { (float)m_dwWindowWidth, (float)m_dwWindowHeight });


    for (int iObj = 0; iObj < 10; iObj++)
    {
        TObject* pObj = new TNpcObj;
        pObj->Set(m_pDevice, m_pImmediateContext);
        pObj->SetPos(TVector3(randstep(-g_fMapSizeX, +g_fMapSizeX),
                     randstep(-g_fMapSizeY, +g_fMapSizeY), 0));
        pObj->SetScale(TVector3(50.0f, 50.0f, 1.0f));
        TVector2 rt = { pObj->m_vPos.x, pObj->m_vPos.y };
        pObj->SetRect(rt,
            pObj->m_vScale.x * 2.0f, pObj->m_vScale.y * 2.0f);

        pObj->Create( L"../../res/air.png",L"Plane.hlsl");
        m_NpcList.push_back(pObj);
    }
    return true; 
}
bool  Sample::Frame() 
{ 
    m_pPlayer->Frame();
    m_pMapObj->Frame();

    for (auto obj : m_NpcList)
    {       
        if (obj->m_bDead == false)
        {
            obj->Move(g_fSecondPerFrame);
            obj->Frame();
        }
    }

    for (auto obj : m_NpcList)
    {
        if (m_pPlayer->m_tRT.ToRect(obj->m_tRT))
        {
           obj->m_bDead = true;
        }
    }
    return true; 
}
bool  Sample::Render() 
{    
    m_pImmediateContext->OMSetBlendState(m_AlphaBlend, 0, -1); 
    
    m_MainCamera.m_vCameraPos = m_pPlayer->m_vPos;
    m_pMapObj->SetMatrix(nullptr, &m_MainCamera.m_matView, 
                                  &m_MainCamera.m_matOrthoProjection);
    m_pMapObj->Render();

    bool gamefinish = true;
    for (auto obj : m_NpcList)
    {   
        if (obj->m_bDead == false)
        {
            obj->SetMatrix(nullptr, &m_MainCamera.m_matView,
                &m_MainCamera.m_matOrthoProjection);
            obj->Render();
            gamefinish = false;
        }
    }

    m_pPlayer->SetMatrix(nullptr, &m_MainCamera.m_matView,
                                  &m_MainCamera.m_matOrthoProjection);
    m_pPlayer->Render();

    if (gamefinish)
    {
        return false;
    }
    return true; 
}
bool  Sample::Release()
{   
    m_pMapObj->Release();
    delete m_pMapObj;
    m_pMapObj = nullptr;

    m_pPlayer->Release();
    delete m_pPlayer;
    m_pPlayer = nullptr;

    for (auto obj : m_NpcList)
    {
        obj->Release();
        delete obj;
    }
    m_NpcList.clear();  
    m_AlphaBlend->Release();
    return true; 
}

TGAME(L"kgca", 800, 600)