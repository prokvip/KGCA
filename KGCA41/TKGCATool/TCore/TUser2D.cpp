#include "pch.h"
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

bool TUser2D::Frame()
{
    TVector2 vPos = m_vPos;

    if (I_Input.GetKey('P'))
    {
        m_fAngleDegree += 180.0f * g_fSecondPerFrame;
    }    
    if (I_Input.GetKey('O'))
    {
        m_fAngleDegree -= 180.0f * g_fSecondPerFrame;
    } 

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