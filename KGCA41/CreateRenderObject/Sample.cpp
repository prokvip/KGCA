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

    m_texMgr.Set(m_pDevice, m_pImmediateContext);
    m_shaderMgr.Set(m_pDevice, m_pImmediateContext);

    std::wstring texname[] = { 
        //L"../../res/ground.png", // path=../../res/, name = kgcabk.bmp
        L"../../res/ade4.dds" ,
        L"../../res/mapcontrol.png",
        L"../../res/103.tga"
    };

    srand(time(NULL));
    m_pMapObj = new TPlaneObj;
    m_pMapObj->Set(m_pDevice, m_pImmediateContext);
    m_pMapObj->SetScale(TVector3(1000.0f, 1000.0f, 1.0f));
    m_pMapObj->Create(m_texMgr, L"../../res/ground.png", m_shaderMgr, L"Plane.hlsl");

    for (int iObj = 0; iObj < 10; iObj++)
    {
        TObject* pObj = new TNpc;
        pObj->Set(m_pDevice, m_pImmediateContext);
        pObj->SetPos(TVector3(randstep(-1000.0f, +1000.0f),
                     randstep(-1000.0f, +1000.0f), 0));
        pObj->SetScale(TVector3(50.0f, 50.0f, 1.0f));
        pObj->Create( m_texMgr, L"../../res/air.png",
                      m_shaderMgr, L"Plane.hlsl");
        m_NpcList.push_back(pObj);
    }
    return true; 
}
bool  Sample::Frame() 
{ 
    if (m_GameInput.m_dwKeyState[VK_LBUTTON] == KEY_PUSH)
    {
        m_vCameraPos.x -= 500.0f * g_fSecondPerFrame;
    }

    if (m_GameInput.m_dwKeyState['A'] > KEY_UP)
    {
        m_vCameraPos.x -= 500.0f*g_fSecondPerFrame;
    }
    if (m_GameInput.m_dwKeyState['D'] > KEY_UP)
    {
        m_vCameraPos.x += 500.0f * g_fSecondPerFrame;
    }
    if (m_GameInput.m_dwKeyState['W'] > KEY_UP)
    {
        m_vCameraPos.y += 500.0f * g_fSecondPerFrame;
    }
    if (m_GameInput.m_dwKeyState['S'] > KEY_UP)
    {
        m_vCameraPos.y -= 500.0f * g_fSecondPerFrame;
    }

    m_pMapObj->Frame();

    for (auto obj : m_NpcList)
    {       
        obj->Move(g_fSecondPerFrame);
        obj->Frame();
    }
    return true; 
}
bool  Sample::Render() 
{    
    m_pImmediateContext->OMSetBlendState(m_AlphaBlend, 0, -1);
    m_matView._41 = -m_vCameraPos.x;
    m_matView._42 = -m_vCameraPos.y;
    m_matView._43 = -m_vCameraPos.z;
    m_matOrthoProjection._11 = 2.0f / ((float)m_dwWindowWidth);
    m_matOrthoProjection._22 = 2.0f / ((float)m_dwWindowHeight);
    // 월드좌표 범위(-10 ~ +10)  camera (0,0)
    // -10 ~ +10 camera (-5,0)가 원점이 된다.
    // 뷰 좌표 -> -5 ~ 15
    // 투영좌표 -> 9 ~ 10 ~ 11
    // 투영좌표 -> -1 ~ 0 ~ +1
    m_pMapObj->SetMatrix(nullptr, &m_matView, &m_matOrthoProjection);
    m_pMapObj->Render();
    for (auto obj : m_NpcList)
    {   
        obj->SetMatrix(nullptr, &m_matView, &m_matOrthoProjection);
        obj->Render();
    }
    return true; 
}
bool  Sample::Release()
{   
    m_pMapObj->Release();
    delete m_pMapObj;
    m_pMapObj = nullptr;

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