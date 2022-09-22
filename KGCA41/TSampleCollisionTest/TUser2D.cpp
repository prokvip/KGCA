#include "TUser2D.h"
#include "TInput.h"
void  TUser2D::UpdateVertexBuffer()
{
    m_VertexList[0].p = { m_vNDCPos.x, m_vNDCPos.y, 0.0f };
    m_VertexList[0].t = { m_rtUV.x1, m_rtUV.y1 };

    m_VertexList[1].p = { m_vNDCPos.x + m_vDrawSize.x, m_vNDCPos.y,  0.0f };
    m_VertexList[1].t = { m_rtUV.x1 + m_rtUV.w, m_rtUV.y1 };

    m_VertexList[2].p = { m_vNDCPos.x, m_vNDCPos.y - m_vDrawSize.y, 0.0f };
    m_VertexList[2].t = { m_rtUV.x1, m_rtUV.y1 + m_rtUV.h };

    m_VertexList[3].p = { m_vNDCPos.x + m_vDrawSize.x, m_vNDCPos.y - m_vDrawSize.y, 0.0f };
    m_VertexList[3].t = { m_rtUV.x1 + m_rtUV.w , m_rtUV.y1 + m_rtUV.h };

    Rotation();

    m_pImmediateContext->UpdateSubresource(m_pVertexBuffer, NULL, NULL, &m_VertexList.at(0), 0, 0);
}
void TUser2D::Rotation()
{
    m_InitVertexList = m_VertexList;
    TRect center;
    center.Set(m_VertexList[0].p.x, m_VertexList[0].p.y,
        m_VertexList[1].p.x - m_VertexList[0].p.x,
        m_VertexList[2].p.y - m_VertexList[0].p.y);

    TVector v[4];
    v[0].x = m_VertexList[0].p.x - center.cx; v[0].y = m_VertexList[0].p.y - center.cy;
    v[1].x = m_VertexList[1].p.x - center.cx; v[1].y = m_VertexList[1].p.y - center.cy;
    v[2].x = m_VertexList[2].p.x - center.cx; v[2].y = m_VertexList[2].p.y - center.cy;
    v[3].x = m_VertexList[3].p.x - center.cx; v[3].y = m_VertexList[3].p.y - center.cy;

    float fDegree = 45.0f;
    float fRadian = DegreeToRadian(fDegree);

    TVector vRet[4];
    for (int iV = 0; iV < 4; iV++)
    {
        vRet[iV].x = v[iV].x * cos(g_fGameTimer) - v[iV].y * sin(g_fGameTimer);
        vRet[iV].y = v[iV].x * sin(g_fGameTimer) + v[iV].y * cos(g_fGameTimer);
    }
    m_VertexList[0].p.x = vRet[0].x + center.cx; m_VertexList[0].p.y = vRet[0].y + center.cy;
    m_VertexList[1].p.x = vRet[1].x + center.cx; m_VertexList[1].p.y = vRet[1].y + center.cy;
    m_VertexList[2].p.x = vRet[2].x + center.cx; m_VertexList[2].p.y = vRet[2].y + center.cy;
    m_VertexList[3].p.x = vRet[3].x + center.cx; m_VertexList[3].p.y = vRet[3].y + center.cy;
}
bool TUser2D::Frame()
{
    TVector2D vPos = m_vPos;
    
    m_vDir = { 0,0 };
    if (I_Input.GetKey('W'))
    {
        m_vDir.y = -1.0f;
        vPos.y += -1.0f * g_fSecondPerFrame * m_fSpeed;
    }
    if (I_Input.GetKey('S'))
    {
        m_vDir.y = 1.0f;
        vPos.y += 1.0f * g_fSecondPerFrame * m_fSpeed;
    }
    if (I_Input.GetKey('A'))
    {
        m_vDir.x = -1.0f;
        vPos.x += -1.0f * g_fSecondPerFrame * m_fSpeed;
    }
    if (I_Input.GetKey('D'))
    {
        m_vDir.x = 1.0f;
        vPos.x += 1.0f * g_fSecondPerFrame * m_fSpeed;
    }
    m_vPos = vPos;   
    return true;
}