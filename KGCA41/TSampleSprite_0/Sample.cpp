#include "Sample.h"
#include "TTextureManager.h"
bool Sample::Init()
{     
    TTexture* pMaskTex = I_Tex.Load(L"../../data/bitmap2.bmp");
    // map
    m_pMap = new TBaseObject;
    m_pMap->Create(m_pd3dDevice, m_pImmediateContext,
        L"../../data/kgcabk.bmp",
        L"../../data/shader/DefaultShape.txt");
    
    for (int iNpc = 0; iNpc < 37; iNpc++)
    {
        TNpc2D* npc = new TNpc2D;
        npc->Create(m_pd3dDevice, m_pImmediateContext,
            L"../../data/bitmap1.bmp",
            L"DefaultShapeMask.txt");
        if (iNpc % 2 == 0)
        {
            npc->SetRect({ 46, 62, 68, 79 });
        }
        else
        {
            npc->SetRect({ 115, 62, 37, 35 });
        }
        npc->SetDirection({ randstep(-1.0f, 1.0f),
            randstep(-1.0f, 1.0f) });
        npc->SetPosition({ 100.0f+iNpc* 100.0f, 100.0f });
        npc->SetMask(pMaskTex);
        m_pNpcList.push_back(npc);
    }

    // user character
    // { 90, 1, 40, 60 } , { 400,300 }
    // -1 ~ +1
    m_pUser = new TUser2D;
    m_pUser->Create(m_pd3dDevice, m_pImmediateContext,
        L"../../data/bitmap1.bmp",
        L"DefaultShapeMask.txt");    
    m_pUser->SetMask(pMaskTex);
    m_pUser->m_fSpeed = 300.0f;
    m_pUser->SetRect({ 91, 2, 39, 59 });
    m_pUser->SetPosition({ g_rtClient.right/2.0f,
                           g_rtClient.bottom-100.0f });
    return true;
}
bool Sample::Frame()
{
    m_pMap->Frame();
    for (auto obj : m_pNpcList)
    {
        obj->Frame();
    }
    m_pUser->Frame();
    return true;
}
bool Sample::Render()
{
    m_pMap->Render();

    for (auto obj : m_pNpcList)
    {
        obj->Render();
    }

    m_pUser->PreRender();
    m_pImmediateContext->PSSetShaderResources(1, 1, 
        &m_pUser->m_pMaskTex->m_pTextureSRV);
    m_pUser->PostRender();
    return true;
}
bool Sample::Release()
{
    m_pMap->Release();
    m_pUser->Release();
    delete m_pUser;
    m_pUser = nullptr;
    for (auto obj : m_pNpcList)
    {
        obj->Release();
        delete obj;
    }
    return true;
}

GAME_RUN(TSampleSprite_0, 800, 600)