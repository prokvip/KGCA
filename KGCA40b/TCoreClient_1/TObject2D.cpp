#include "TObject2D.h"
void		TObject2D::SetPosition(TVector2 vPos)
{
	// 현재위치
	m_vPos += vPos;
	Convert(m_vPos, m_fWidth, m_fHeight, m_VertexList);
	m_pContext->UpdateSubresource(
		m_pVertexBuffer, 0, NULL, &m_VertexList.at(0), 0, 0);
}