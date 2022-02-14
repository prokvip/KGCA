#include "TPlayerObj.h"
#include "TInput.h"
bool	TPlayerObj::Frame()
{
	if (TInput::Get().GetKey(VK_F1) == KEY_PUSH)
	{
		int kk = 0;
	}
	if (TInput::Get().GetKey('1') == KEY_UP)
	{
		int kk = 0;
	}
	if (TInput::Get().GetKey('W') == KEY_HOLD)
	{
		m_VertexList[0].v.y += m_fSpeed;
		m_VertexList[1].v.y += m_fSpeed;
		m_VertexList[2].v.y += m_fSpeed;
		m_VertexList[3].v.y += m_fSpeed;
		m_VertexList[4].v.y += m_fSpeed;
		m_VertexList[5].v.y += m_fSpeed;
	}
	if (TInput::Get().GetKey('A') == KEY_HOLD)
	{
		m_VertexList[0].v.x -= m_fSpeed;
		m_VertexList[1].v.x -= m_fSpeed;
		m_VertexList[2].v.x -= m_fSpeed;
		m_VertexList[3].v.x -= m_fSpeed;
		m_VertexList[4].v.x -= m_fSpeed;
		m_VertexList[5].v.x -= m_fSpeed;
	}
	if (TInput::Get().GetKey('S') == KEY_HOLD)
	{
		m_VertexList[0].v.y -= m_fSpeed;
		m_VertexList[1].v.y -= m_fSpeed;
		m_VertexList[2].v.y -= m_fSpeed;
		m_VertexList[3].v.y -= m_fSpeed;
		m_VertexList[4].v.y -= m_fSpeed;
		m_VertexList[5].v.y -= m_fSpeed;
	}
	if (TInput::Get().GetKey('D') == KEY_HOLD)
	{
		m_VertexList[0].v.x += m_fSpeed;
		m_VertexList[1].v.x += m_fSpeed;
		m_VertexList[2].v.x += m_fSpeed;
		m_VertexList[3].v.x += m_fSpeed;
		m_VertexList[4].v.x += m_fSpeed;
		m_VertexList[5].v.x += m_fSpeed;
	}

	
	m_pContext->UpdateSubresource(
		m_pVertexBuffer,0, NULL, &m_VertexList.at(0), 0,0);
	return true;
}