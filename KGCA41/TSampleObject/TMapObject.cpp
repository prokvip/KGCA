#include "TMapObject.h"
#include "TInput.h"
bool TMapObject::FrameScroll()
{
    static float fStep = 0.0f;
    /* UINT iMapWidth = m_pTexture->m_Desc.Width;
     if (I_Input.GetKey('W') > 0)
     {
         m_vUserPos.y += g_fSecondPerFrame * 10.0f;
         if (iMapWidth > m_vUserPos.x)
         {
             fStep = m_vUserPos.y / iMapWidth;
         }

     }*/
    if (fStep + 0.1f <= 1.0f)
    {
        fStep += g_fSecondPerFrame * 0.01f;
    }

    m_VertexList[0].p = { -1.0f, 1.0f, 0.0f };
    m_VertexList[1].p = { +1.0f, 1.0f,  0.0f };
    m_VertexList[2].p = { -1.0f, -1.0f, 0.0f };
    m_VertexList[3].p = { 1.0f, -1.0f, 0.0f };

    m_VertexList[0].t = { 0.0f, 0.0f + fStep };
    m_VertexList[1].t = { 1.0f, 0.0f + fStep };
    m_VertexList[2].t = { 0.0f, 0.1f + fStep };
    m_VertexList[3].t = { 1.0f, 0.1f + fStep };

    // gpu update
    m_pImmediateContext->UpdateSubresource(
        m_pVertexBuffer, 0, NULL,
        &m_VertexList.at(0), 0, 0);
    return true;
}
bool TMapObject::Frame()
{
    SetPosition(m_vPos, m_vCameraPos);    
	return true;
}
void  TMapObject::UpdateVertexBuffer()
{
    m_VertexList[0].p = { m_vDrawPos.x, m_vDrawPos.y, 0.0f };
    m_VertexList[1].p = { m_vDrawPos.x + m_vDrawSize.x, m_vDrawPos.y,  0.0f };
    m_VertexList[2].p = { m_vDrawPos.x, m_vDrawPos.y - m_vDrawSize.y, 0.0f };
    m_VertexList[3].p = { m_vDrawPos.x + m_vDrawSize.x, m_vDrawPos.y - m_vDrawSize.y, 0.0f };
    m_pImmediateContext->UpdateSubresource(
        m_pVertexBuffer, NULL, NULL, &m_VertexList.at(0), 0, 0);
}