#include "TEffectObj.h"

bool TEffectObj::Init()
{
	return true;
}
bool TEffectObj::Frame()
{
	static int iIndex = 0;
	static float fChangeTime = m_pSprite->m_fAnimTime / m_pSprite->m_rtArray.size();
	static float fTimer = 0.0f;
	fTimer += g_fSecPerFrame;
	if (fTimer >= fChangeTime)
	{
		iIndex++;
		if (iIndex >= m_pSprite->m_rtArray.size())
		{
			iIndex = 0;
		}
		fTimer -= fChangeTime;
		SetRectSouce(m_pSprite->m_rtArray[iIndex]);
		SetRectDraw({ 0,0,
			m_pSprite->m_rtArray[iIndex].right,
			m_pSprite->m_rtArray[iIndex].bottom });
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