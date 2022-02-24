#include "TEffectObj.h"

bool TEffectObj::Init()
{
	return true;
}
bool TEffectObj::Frame()
{
	m_fChangeTime = m_pSprite->m_fAnimTime / m_pSprite->m_rtArray.size();
	m_fTimer += g_fSecPerFrame;
	if (m_fTimer >= m_fChangeTime)
	{
		m_iCurrentIndex++;
		if (m_iCurrentIndex >= m_pSprite->m_rtArray.size())
		{
			m_iCurrentIndex = 0;
		}
		m_fTimer -= m_fChangeTime;
		SetRectSouce(m_pSprite->m_rtArray[m_iCurrentIndex]);
		SetRectDraw({ 0,0,
			m_pSprite->m_rtArray[m_iCurrentIndex].right,
			m_pSprite->m_rtArray[m_iCurrentIndex].bottom });
	}
	AddPosition({ 0,0 });
	return true;
}
bool TEffectObj::Render()
{
	TDxObject::Render();
	return true;
}
bool TEffectObj::Release()
{
	return true;
}