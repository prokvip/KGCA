#include "TInterface.h"
#include "TInput.h"

bool  TButton::Init()
{
    return true;
}
bool  TButton::Frame()
{
    POINT ptMouse = I_Input.m_ptPos;
    if (TCollision::RectToPoint( m_rtCollision, ptMouse))
    {
        m_CurrentState = UI_HOVER;
        m_pCurrentTex = m_pStateList[UI_HOVER];
        if (I_Input.GetKey(VK_LBUTTON) == KEY_PUSH ||
            I_Input.GetKey(VK_LBUTTON) == KEY_HOLD)
        {
            m_CurrentState = UI_PUSH;
            m_pCurrentTex = m_pStateList[UI_PUSH];
        }
        if (I_Input.GetKey(VK_LBUTTON) == KEY_UP)
        {
            m_CurrentState = UI_SELECT;
        }
    }
    else
    {
        m_pCurrentTex = m_pStateList[UI_NORMAL];
    }

    /*m_VertexList[0].c = { 1,1,1,cosf(g_fGameTimer) * 0.5f+0.5f };
    m_VertexList[1].c = { 1,1,1,cosf(g_fGameTimer) * 0.5f + 0.5f };
    m_VertexList[2].c = { 1,1,1,cosf(g_fGameTimer) * 0.5f + 0.5f };
    m_VertexList[3].c = { 1,1,1,cosf(g_fGameTimer) * 0.5f + 0.5f };*/
    m_VertexList[0].t = { 0.0f, 0.0f };
    m_VertexList[1].t = { 1.0f, 0.0f };
    m_VertexList[2].t = { 0.0f, 1.0f };
    m_VertexList[3].t = { 1.0f, 1.0f };
    m_pImmediateContext->UpdateSubresource(m_pVertexBuffer, NULL, NULL, &m_VertexList.at(0), 0, 0);
    return true;
}
bool TButton::Render()
{
    TBaseObject::PreRender();
    m_pImmediateContext->PSSetShaderResources(0, 1, &m_pCurrentTex->m_pTextureSRV);
    TBaseObject::PostRender();
    return true;
}
bool TButton::Release()
{
    return true;
}
void  TInterface::Rotation()
{
    float fRadian = DegreeToRadian(m_fAngleDegree);
    for (int iV = 0; iV < 4; iV++)
    {
        m_InitVertexList[iV].p.x = m_VertexList[iV].p.x * cos(fRadian) - m_VertexList[iV].p.y * sin(fRadian);
        m_InitVertexList[iV].p.y = m_VertexList[iV].p.x * sin(fRadian) + m_VertexList[iV].p.y * cos(fRadian);
    }
    m_pImmediateContext->UpdateSubresource(
        m_pVertexBuffer, NULL, NULL, &m_InitVertexList.at(0), 0, 0);
}

bool  TListControl::Init()
{
    return true;
}
bool  TListControl::Frame()
{
    for (auto data : m_btnList)
    {
        data->Frame();
    }
    return true;
}
bool TListControl::Render()
{
    TBaseObject::PreRender();
    m_pImmediateContext->PSSetShaderResources(0, 1, &m_pCurrentTex->m_pTextureSRV);
    TBaseObject::PostRender();

    for (auto data : m_btnList)
    {
        data->Render();
    }
    return true;
}
bool TListControl::Release()
{
    return true;
}
void  TListControl::SetRect(TRect rt)
{
    m_rtInit = rt;
    m_ptImageSize.x = m_pTexture->m_Desc.Width;
    m_ptImageSize.y = m_pTexture->m_Desc.Height;
    float fPixelX = (1.0f / m_pTexture->m_Desc.Width) / 2.0f;
    float fPixelY = (1.0f / m_pTexture->m_Desc.Height) / 2.0f;
    // 90  -> 0 ~ 1
    m_rtUV.x1 = 0.0f; // u
    // 1
    m_rtUV.y1 = 0.0f; // v
    // 40
    m_rtUV.w = 1.0f;
    // 60
    m_rtUV.h = 1.0f;
}

// È­¸é ÁÂÇ¥ -> NDC ÁÂÇ¥ 
void  TListControl::ScreenToNDC()
{
    TVector2D	vDrawSize;
    vDrawSize.x = m_rtInit.w / 2.0f;
    vDrawSize.y = m_rtInit.h / 2.0f;
    m_rtCollision.Set(
        m_vPos.x - vDrawSize.x,
        m_vPos.y - vDrawSize.y,
        m_rtInit.w,
        m_rtInit.h);

    // 0  ~ 800   -> 0~1 ->  -1 ~ +1
    m_vNDCPos.x = (m_rtCollision.x1 / g_rtClient.right) * 2.0f - 1.0f;
    m_vNDCPos.y = -((m_rtCollision.y1 / g_rtClient.bottom) * 2.0f - 1.0f);
    m_vDrawSize.x = (m_rtInit.w / (float)g_rtClient.right) * 2.0f;
    m_vDrawSize.y = (m_rtInit.h / (float)g_rtClient.bottom) * 2.0f;
}
void  TListControl::UpdateVertexBuffer()
{
    m_VertexList[0].p = { m_vNDCPos.x, m_vNDCPos.y, 0.0f };
    m_VertexList[0].t = { m_rtUV.x1, m_rtUV.y1 };

    m_VertexList[1].p = { m_vNDCPos.x + m_vDrawSize.x, m_vNDCPos.y,  0.0f };
    m_VertexList[1].t = { m_rtUV.x1 + m_rtUV.w, m_rtUV.y1 };

    m_VertexList[2].p = { m_vNDCPos.x, m_vNDCPos.y - m_vDrawSize.y, 0.0f };
    m_VertexList[2].t = { m_rtUV.x1, m_rtUV.y1 + m_rtUV.h };

    m_VertexList[3].p = { m_vNDCPos.x + m_vDrawSize.x, m_vNDCPos.y - m_vDrawSize.y, 0.0f };
    m_VertexList[3].t = { m_rtUV.x1 + m_rtUV.w , m_rtUV.y1 + m_rtUV.h };

    m_pImmediateContext->UpdateSubresource(
        m_pVertexBuffer, NULL, NULL, &m_VertexList.at(0), 0, 0);
}