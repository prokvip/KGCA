#include "TObjectNpc2D.h"
void TObjectNpc2D::HitOverlap(TBaseObject* pObj, DWORD dwState)
{
	if (dwState == TCollisionType::Overlap)
	{
		//pObj->m_bAlphaBlend = !pObj->m_bAlphaBlend;
	}
}

void	TObjectNpc2D::HitSelect(TBaseObject* pObj, DWORD dwState)
{
	if (m_dwSelectState & T_HOVER)
	{
		INT K = 0;
	}
	if (m_dwSelectState & T_FOCUS)
	{
		INT K = 0;
	}
	if (m_dwSelectState & T_ACTIVE)
	{
		INT K = 0;
	}
	if (m_dwSelectState & T_SELECTED)
	{
		m_bSelect = true;
		m_bDead = true;
	}
}
bool	TObjectNpc2D::Frame()
{
	TVector2 p;
	if (m_vPos.x > g_rtClient.right - 50.0f)
	{
		m_vDirection.x *= -1.0f;
		m_vPos.x = g_rtClient.right - 50.0f;
	}
	if (m_vPos.x < g_rtClient.left+ 50.0f)
	{
		m_vDirection.x *= -1.0f;
		m_vPos.x = g_rtClient.left + 50.0f;
	}
	if (m_vPos.y > g_rtClient.bottom - 50.0f)
	{
		m_vDirection.y *= -1.0f;
		m_vPos.y = g_rtClient.bottom - 50.0f;
	}
	if (m_vPos.y < g_rtClient.top + 50.0f)
	{
		m_vDirection.y *= -1.0f;
		m_vPos.y = g_rtClient.top + 50.0f;
	}
	p = m_vDirection * (m_fSpeed * g_fSecPerFrame);
	AddPosition(p);
	return true;
}
TObjectNpc2D::TObjectNpc2D()
{
	m_fSpeed = 20.0f + (rand() % 300); // ¼Ó·Â
	m_vDirection.x = (rand()%2 == 0) ? 1.0f : -1.0f;
	m_vDirection.y = (rand() % 2 == 0) ? 1.0f : -1.0f;
	m_dwCollisonType = TCollisionType::Overlap;
	m_dwSelectType = TSelectType::Select_Overlap;
}
TObjectNpc2D::~TObjectNpc2D()
{
	
}