#include "TObject2D.h"
#include "TTextureUIManager.h"
namespace TUI
{
    bool TObject2D::Frame()
    {
        return true;
    }
    void  TObject2D::UpdateVertexBuffer()
    {
        if (m_VertexList.size() == 0) return;
        m_VertexList[0].p = { m_vNDCPos.x, m_vNDCPos.y, 0.5f };
        m_VertexList[0].t = { m_rtUV.x1, m_rtUV.y1 };

        m_VertexList[1].p = { m_vNDCPos.x + m_vDrawSize.x, m_vNDCPos.y,  0.5f };
        m_VertexList[1].t = { m_rtUV.x1 + m_rtUV.w, m_rtUV.y1 };

        m_VertexList[2].p = { m_vNDCPos.x, m_vNDCPos.y - m_vDrawSize.y, 0.5f };
        m_VertexList[2].t = { m_rtUV.x1, m_rtUV.y1 + m_rtUV.h };

        m_VertexList[3].p = { m_vNDCPos.x + m_vDrawSize.x, m_vNDCPos.y - m_vDrawSize.y, 0.5f };
        m_VertexList[3].t = { m_rtUV.x1 + m_rtUV.w , m_rtUV.y1 + m_rtUV.h };

        m_pImmediateContext->UpdateSubresource(  m_pVertexBuffer, NULL, NULL, &m_VertexList.at(0), 0, 0);
    }
    void  TObject2D::SetRect(TRect rt)
    {
        m_rtInitPos = rt;
        SetRectUV(rt);
    }
    void  TObject2D::SetRectUV(TRect rt)
    {
        if (m_pTexture == nullptr) return;
        m_rtInitUV = rt;
        m_ptImageSize.x = m_pTexture->m_Desc.Width;
        m_ptImageSize.y = m_pTexture->m_Desc.Height;
        float fPixelX = (1.0f / m_pTexture->m_Desc.Width) / 2.0f;
        float fPixelY = (1.0f / m_pTexture->m_Desc.Height) / 2.0f;
        m_rtUV.x1 = rt.x1 / m_ptImageSize.x + fPixelX; // u  
        m_rtUV.y1 = rt.y1 / m_ptImageSize.y + fPixelY; // v  
        m_rtUV.w = rt.w / m_ptImageSize.x;
        m_rtUV.h = rt.h / m_ptImageSize.y;
    }
    // È­¸é ÁÂÇ¥ -> NDC ÁÂÇ¥ 
    void  TObject2D::ScreenToNDC()
    {
        TVector2D	vDrawSize;
        vDrawSize.x = m_rtInitPos.w / 2.0f;
        vDrawSize.y = m_rtInitPos.h / 2.0f;
        m_rtCollision.Set(
            m_vPos.x - vDrawSize.x,
            m_vPos.y - vDrawSize.y,
            m_rtInitPos.w,
            m_rtInitPos.h);

        // 0  ~ 800   -> 0~1 ->  -1 ~ +1
        m_vNDCPos.x = (m_rtCollision.x1 / g_rtClient.right) * 2.0f - 1.0f;
        m_vNDCPos.y = -((m_rtCollision.y1 / g_rtClient.bottom) * 2.0f - 1.0f);
        m_vDrawSize.x = (m_rtInitPos.w / (float)g_rtClient.right) * 2.0f;
        m_vDrawSize.y = (m_rtInitPos.h / (float)g_rtClient.bottom) * 2.0f;
    }
    void  TObject2D::SetPosition(TVector2D vPos)
    {
        //m_vBeforePos = m_vPos;
        m_vPos = vPos;
        ScreenToNDC();
        UpdateVertexBuffer();
        //m_vOffsetPos = m_vPos - m_vBeforePos;
    }
    // ¿ùµåÁÂÇ¥(È­¸éÁÂÇ¥°è) -> ºä ÁÂÇ¥ -> NDC ÁÂÇ¥
    void  TObject2D::ScreenToCamera(
        TVector2D vCameraPos, TVector2D vViewPort)
    {
        TVector2D   vViewPos = m_vPos;
        TVector2D	vDrawSize;
        vDrawSize.x = m_rtInitPos.w / 2.0f;
        vDrawSize.y = m_rtInitPos.h / 2.0f;
        m_rtCollision.Set(
            m_vPos.x - vDrawSize.x,
            m_vPos.y - vDrawSize.y,
            m_rtInitPos.w,
            m_rtInitPos.h);

        vViewPos.x = m_rtCollision.x1 - vCameraPos.x;
        vViewPos.y = m_rtCollision.y1 - vCameraPos.y;
        // 0  ~ 800   -> 0~1 ->  -1 ~ +1
        m_vNDCPos.x = vViewPos.x * (2.0f / vViewPort.x);
        m_vNDCPos.y = vViewPos.y * (2.0f / vViewPort.y) * -1.0f;

        m_vDrawSize.x = (m_rtInitPos.w / vViewPort.x) * 2.0f;
        m_vDrawSize.y = (m_rtInitPos.h / vViewPort.y) * 2.0f;
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

    TObject2D::TObject2D()
    {
        m_szName = L"none";
        m_vBeforePos = { 0.0f, 0.0f };
        m_vOffsetPos = { 0.0f, 0.0f };
    }
    TObject2D::~TObject2D()
    {
        m_szName = L"none";
    }
};