#include "TObject2D.h"
#include "TTextureManager.h"
bool TObject2D::Frame()
{
    return true;
}
void  TObject2D::UpdateVertexBuffer()
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
void  TObject2D::SetRect(TRect rt)
{
    m_rtInit = rt;
    m_ptImageSize.x = m_pTexture->m_Desc.Width;
    m_ptImageSize.y = m_pTexture->m_Desc.Height;
    float fPixelX = (1.0f / m_pTexture->m_Desc.Width)/2.0f;
    float fPixelY = (1.0f / m_pTexture->m_Desc.Height)/2.0f;
    // 90  -> 0 ~ 1
    m_rtUV.x1 = rt.x1 / m_ptImageSize.x +fPixelX; // u
    // 1
    m_rtUV.y1 = rt.y1 / m_ptImageSize.y +fPixelY; // v
    // 40
    m_rtUV.w = rt.w / m_ptImageSize.x;
    // 60
    m_rtUV.h = rt.h / m_ptImageSize.y;
}

// È­¸é ÁÂÇ¥ -> NDC ÁÂÇ¥ 
void  TObject2D::ScreenToNDC()
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
void  TObject2D::SetPosition(TVector2D vPos)
{
    m_vPos = vPos;
    ScreenToNDC();
    UpdateVertexBuffer();
}
// ¿ùµåÁÂÇ¥(È­¸éÁÂÇ¥°è) -> ºä ÁÂÇ¥ -> NDC ÁÂÇ¥
void  TObject2D::ScreenToCamera(
    TVector2D vCameraPos, TVector2D vViewPort)
{
    TVector2D   vViewPos = m_vPos;
    TVector2D	vDrawSize;
    vDrawSize.x = m_rtInit.w / 2.0f;
    vDrawSize.y = m_rtInit.h / 2.0f;
    m_rtCollision.Set(
        m_vPos.x - vDrawSize.x,
        m_vPos.y - vDrawSize.y,
        m_rtInit.w,
        m_rtInit.h);

    vViewPos.x = m_rtCollision.x1 - vCameraPos.x;
    vViewPos.y = m_rtCollision.y1 - vCameraPos.y;
    // 0  ~ 800   -> 0~1 ->  -1 ~ +1
    m_vNDCPos.x = vViewPos.x * (2.0f / vViewPort.x);
    m_vNDCPos.y = vViewPos.y * (2.0f / vViewPort.y) * -1.0f;

    m_vDrawSize.x = (m_rtInit.w / vViewPort.x) * 2.0f;
    m_vDrawSize.y = (m_rtInit.h / vViewPort.y) * 2.0f;
}
void  TObject2D::SetPosition(TVector2D vPos, TVector2D vCamera)
{
    m_vPos = vPos;
    ScreenToCamera(vCamera, m_vViewSize);
    UpdateVertexBuffer();
}
void TObject2D::Rotation()
{
    TVector vCenter;
    vCenter.x = (m_VertexList[1].p.x + m_VertexList[0].p.x) / 2.0f;
    vCenter.y = (m_VertexList[2].p.x + m_VertexList[0].p.y) / 2.0f;

    float fDegree = m_fAngleDegree;
    float fRadian = DegreeToRadian(fDegree);
    TVector vRot;
    for (int vertex = 0; vertex < 4; vertex++)
    {
        TVector vCenterMove = m_VertexList[vertex].p - vCenter;
        vRot.x = vCenterMove.x * cos(fRadian) - vCenterMove.y * sin(fRadian);
        vRot.y = vCenterMove.x * sin(fRadian) + vCenterMove.y * cos(fRadian);
        m_VertexList[vertex].p = vRot + vCenter;
    }
}