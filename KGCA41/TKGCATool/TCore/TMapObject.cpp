#include "pch.h"
#include "TMapObject.h"
#include "TInput.h"
bool TMapObject::Frame()
{
    SetPosition(m_vPos, m_vCameraPos);    
	return true;
}
void  TMapObject::UpdateVertexBuffer()
{
    m_VertexList[0].p = { m_vNDCPos.x, m_vNDCPos.y, 0.0f };
    m_VertexList[1].p = { m_vNDCPos.x + m_vDrawSize.x, m_vNDCPos.y,  0.0f };
    m_VertexList[2].p = { m_vNDCPos.x, m_vNDCPos.y - m_vDrawSize.y, 0.0f };
    m_VertexList[3].p = { m_vNDCPos.x + m_vDrawSize.x, m_vNDCPos.y - m_vDrawSize.y, 0.0f };
    m_pImmediateContext->UpdateSubresource(
        m_pVertexBuffer, NULL, NULL, &m_VertexList.at(0), 0, 0);
}