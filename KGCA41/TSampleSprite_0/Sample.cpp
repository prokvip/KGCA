#include "Sample.h"
void  TUser2D::UpdateVertexBuffer()
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
void  TUser2D::SetRect(TRect rt)
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
// È­¸é ÁÂÇ¥ -> NDC ÁÂÇ¥ 
void  TUser2D::SetPosition(TVector2D vPos)
{
    m_vPos = vPos;
    // 0  ~ 800   -> 0~1 ->  -1 ~ +1
    m_vDrawPos.x = (vPos.x / g_rtClient.right) * 2.0f - 1.0f;
    m_vDrawPos.y = -((vPos.y / g_rtClient.bottom) * 2.0f - 1.0f);
    m_vDrawSize.x = (m_rtInit.w / g_rtClient.right)*2;
    m_vDrawSize.y = (m_rtInit.h / g_rtClient.bottom)*2;

    UpdateVertexBuffer();
}

bool TUser2D::Frame()
{
    TVector2D vPos = m_vPos;
    if (I_Input.GetKey('W'))
    {
        vPos.y += -1.0f *  g_fSecondPerFrame * 100.0f;
    }
    if (I_Input.GetKey('S'))
    {
        vPos.y += 1.0f * g_fSecondPerFrame * 100.0f;
    }
    if (I_Input.GetKey('A'))
    {
        vPos.x += -1.0f * g_fSecondPerFrame * 100.0f;
    }
    if (I_Input.GetKey('D'))
    {
        vPos.x += 1.0f * g_fSecondPerFrame * 100.0f;
    }

    SetPosition(vPos);
    return true;
}
bool	Sample::Init()
{     
    // map
    TBaseObject* pObjectMap = new TBaseObject;
    pObjectMap->Create(m_pd3dDevice, m_pImmediateContext,
        L"../../data/kgcabk.bmp",
        L"../../data/shader/DefaultShape.txt");
    m_pObjectList.push_back(pObjectMap);

    // user character
    // { 90, 1, 40, 60 } , { 400,300 }
    // -1 ~ +1
    m_pUser = new TUser2D;
    m_pUser->Create(m_pd3dDevice, m_pImmediateContext,
        L"../../data/bitmap1.bmp",
        L"../../data/shader/DefaultShape.txt");    
    m_pUser->SetRect({ 90, 2, 40, 59 });
    m_pUser->SetPosition({ g_rtClient.right/2.0f,
                           g_rtClient.bottom-100.0f });
    return true;
}
bool		Sample::Frame()
{
    for (auto obj : m_pObjectList)
    {
        obj->Frame();
    }
    m_pUser->Frame();
    return true;
}
bool		Sample::Render()
{
    for (auto obj : m_pObjectList)
    {
        obj->Render();
    }

    m_pUser->Render();
    return true;
}
bool		Sample::Release()
{
    m_pUser->Release();
    delete m_pUser;
    m_pUser = nullptr;
    for (auto obj : m_pObjectList)
    {
        obj->Release();
        delete obj;
    }
    return true;
}

GAME_RUN(TSampleSprite_0, 800, 600)
