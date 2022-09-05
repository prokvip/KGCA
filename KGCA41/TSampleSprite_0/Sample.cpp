#include "Sample.h"
#include "TTextureManager.h"
bool TObject2D::Frame()
{   
    return true;
}
void  TObject2D::UpdateVertexBuffer()
{
    m_VertexList[0].p = { m_vDrawPos.x, m_vDrawPos.y, 0.0f };
    m_VertexList[0].t = { m_rtUV.x1, m_rtUV.y1 };

    m_VertexList[1].p = { m_vDrawPos.x+m_vDrawSize.x, m_vDrawPos.y,  0.0f };
    m_VertexList[1].t = { m_rtUV.x1+ m_rtUV.w, m_rtUV.y1 };

    m_VertexList[2].p = { m_vDrawPos.x, m_vDrawPos.y-m_vDrawSize.y, 0.0f };
    m_VertexList[2].t = { m_rtUV.x1, m_rtUV.y1 + m_rtUV.h };

    m_VertexList[3].p = m_VertexList[2].p;
    m_VertexList[3].t = m_VertexList[2].t;

    m_VertexList[4].p = m_VertexList[1].p;
    m_VertexList[4].t = m_VertexList[1].t;

    m_VertexList[5].p = { m_vDrawPos.x + m_vDrawSize.x, m_vDrawPos.y - m_vDrawSize.y, 0.0f };
    m_VertexList[5].t = { m_rtUV.x1 + m_rtUV.w , m_rtUV.y1 + m_rtUV.h };

    m_pImmediateContext->UpdateSubresource(
        m_pVertexBuffer, NULL,NULL, &m_VertexList.at(0), 0, 0);
}
void  TObject2D::SetRect(TRect rt)
{
    m_rtInit = rt;
    m_ptImageSize.x = m_pTexture->m_Desc.Width;
    m_ptImageSize.y = m_pTexture->m_Desc.Height;
    // 90  -> 0 ~ 1
    m_rtUV.x1 = rt.x1 / m_ptImageSize.x; // u
    // 1
    m_rtUV.y1 = rt.y1 / m_ptImageSize.y; // v
    // 40
    m_rtUV.w = rt.w / m_ptImageSize.x;
    // 60
    m_rtUV.h = rt.h / m_ptImageSize.y;
}
// 화면 좌표 -> NDC 좌표 
void  TObject2D::SetPosition(TVector2D vPos)
{
    m_vPos = vPos;
    // 0  ~ 800   -> 0~1 ->  -1 ~ +1
    m_vDrawPos.x = (vPos.x / g_rtClient.right) * 2.0f - 1.0f;
    m_vDrawPos.y = -((vPos.y / g_rtClient.bottom) * 2.0f - 1.0f);
    m_vDrawSize.x = (m_rtInit.w / g_rtClient.right)*2;
    m_vDrawSize.y = (m_rtInit.h / g_rtClient.bottom)*2;

    UpdateVertexBuffer();
}

bool TNpc2D::Frame()
{
    TVector2D vPos = m_vPos;
    //벡터의 직선의 방정식 &  시간의 동기화
    TVector2D m_vVelocity = m_vDir * m_fSpeed * g_fSecondPerFrame;
    vPos = vPos + m_vVelocity;   
    
    if (vPos.x > g_rtClient.right)
    {
        vPos.x = g_rtClient.right;
        m_vDir.x *= -1.0f;
    }
    if (vPos.x < 0.0f)
    {
        vPos.x = 0.0f;
        m_vDir.x *= -1.0f;
    }
    if (vPos.y > g_rtClient.bottom)
    {
        vPos.y = g_rtClient.bottom;
        m_vDir.y *= -1.0f;
    }
    if (vPos.y < 0.0f)
    {
        vPos.y = 0.0f;
        m_vDir.y *= -1.0f;
    }       

    SetPosition(vPos);
    return true;
}
bool TUser2D::Frame()
{
    TVector2D vPos = m_vPos;
    if (I_Input.GetKey('W'))
    {
        vPos.y += -1.0f * g_fSecondPerFrame * m_fSpeed;
    }
    if (I_Input.GetKey('S'))
    {
        vPos.y += 1.0f * g_fSecondPerFrame * m_fSpeed;
    }
    if (I_Input.GetKey('A'))
    {
        vPos.x += -1.0f * g_fSecondPerFrame * m_fSpeed;
    }
    if (I_Input.GetKey('D'))
    {
        vPos.x += 1.0f * g_fSecondPerFrame * m_fSpeed;
    }

    SetPosition(vPos);
    return true;
}
bool	Sample::Init()
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
bool		Sample::Frame()
{
    m_pMap->Frame();
    for (auto obj : m_pNpcList)
    {
        obj->Frame();
    }
    m_pUser->Frame();
    return true;
}
bool		Sample::Render()
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
bool		Sample::Release()
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
