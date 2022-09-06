#include "TObject2D.h"
#include "TTextureManager.h"
bool TObject2D::Frame()
{
    return true;
}
void  TObject2D::UpdateVertexBuffer()
{
    m_VertexList[0].p = { m_vDrawPos.x, m_vDrawPos.y, 0.0f };
    m_VertexList[0].t = { m_rtUV.x1, m_rtUV.y1 };

    m_VertexList[1].p = { m_vDrawPos.x + m_vDrawSize.x, m_vDrawPos.y,  0.0f };
    m_VertexList[1].t = { m_rtUV.x1 + m_rtUV.w, m_rtUV.y1 };

    m_VertexList[2].p = { m_vDrawPos.x, m_vDrawPos.y - m_vDrawSize.y, 0.0f };
    m_VertexList[2].t = { m_rtUV.x1, m_rtUV.y1 + m_rtUV.h };

    m_VertexList[3].p = m_VertexList[2].p;
    m_VertexList[3].t = m_VertexList[2].t;

    m_VertexList[4].p = m_VertexList[1].p;
    m_VertexList[4].t = m_VertexList[1].t;

    m_VertexList[5].p = { m_vDrawPos.x + m_vDrawSize.x, m_vDrawPos.y - m_vDrawSize.y, 0.0f };
    m_VertexList[5].t = { m_rtUV.x1 + m_rtUV.w , m_rtUV.y1 + m_rtUV.h };

    m_pImmediateContext->UpdateSubresource(
        m_pVertexBuffer, NULL, NULL, &m_VertexList.at(0), 0, 0);
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
// È­¸é ÁÂÇ¥ -> NDC ÁÂÇ¥ 
void  TObject2D::SetPosition(TVector2D vPos)
{
    m_vPos = vPos;
    // 0  ~ 800   -> 0~1 ->  -1 ~ +1
    m_vDrawPos.x = (vPos.x / g_rtClient.right) * 2.0f - 1.0f;
    m_vDrawPos.y = -((vPos.y / g_rtClient.bottom) * 2.0f - 1.0f);
    m_vDrawSize.x = (m_rtInit.w / g_rtClient.right) * 2;
    m_vDrawSize.y = (m_rtInit.h / g_rtClient.bottom) * 2;

    UpdateVertexBuffer();
}