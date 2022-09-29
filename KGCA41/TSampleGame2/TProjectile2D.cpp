#include "TProjectile2D.h"
bool TProjectile2D::Update()
{
	m_fEffectTimer += g_fSecondPerFrame;
	if (m_fLifeTime <= m_fEffectTimer)
	{
		return false;
	}
	if (m_fStep <= m_fEffectTimer)
	{
		m_fEffectTimer -= m_fStep;
		m_iIndex++;
	}
	if (m_iIndex >= m_iMaxIndex)
	{
		m_iIndex = 0;
	}
	RECT rt = m_pSprite->m_uvArray[m_iIndex];
	m_tRect = convert(rt);
	TVector2D vAdd = m_vDir * m_fSpeed * g_fSecondPerFrame;
	m_vPos = m_vPos + vAdd;

	TVector2D	vDrawSize;
	vDrawSize.x = m_pSprite->m_rtInit.w / 2.0f;
	vDrawSize.y = m_pSprite->m_rtInit.h / 2.0f;
	m_rtCollision.Set(
		m_vPos.x - vDrawSize.x,
		m_vPos.y - vDrawSize.y,
		m_pSprite->m_rtInit.w,
		m_pSprite->m_rtInit.h);
	return true;
}
TProjectile2D::TProjectile2D()
{
	m_fLifeTime = 3.33f;
	m_fSpeed = 600.0f;
}