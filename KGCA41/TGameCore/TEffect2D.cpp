#include "TEffect2D.h"
TRect TEffect2D::convert(RECT rt)
{
	TRect tRt;
	tRt.x1 = rt.left;
	tRt.y1 = rt.top;
	tRt.w = rt.right;
	tRt.h = rt.bottom;
	return tRt;
}
bool TEffect2D::Update()
{
	m_fEffectTimer += g_fSecondPerFrame;
	if (m_fStep <= m_fEffectTimer)
	{
		m_fEffectTimer -= m_fStep;
		m_iIndex++;
	}
	if (m_iIndex >= m_iMaxIndex)
	{
		return false;
	}
	RECT rt = m_pSprite->m_uvArray[m_iIndex];
	m_tRect = convert(rt);

	TVector2	vDrawSize;
	vDrawSize.x = m_pSprite->m_rtInit.w / 2.0f;
	vDrawSize.y = m_pSprite->m_rtInit.h / 2.0f;
	m_rtCollision.Set(
		m_vPos.x - vDrawSize.x,
		m_vPos.y - vDrawSize.y,
		m_pSprite->m_rtInit.w,
		m_pSprite->m_rtInit.h);
	return true;
}