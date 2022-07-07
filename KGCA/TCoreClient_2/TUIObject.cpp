#include "TUIObject.h"
bool	TUIObject::Frame()
{
	TDxObject::Frame();
	return true;
}
bool	TUIObject::Render()
{
	TDxObject::Render();
	return true;
}
bool	TImageObject::Init()
{
	m_bFadeIn = true;
	return true;
}
bool	TImageObject::Frame()
{
	TDxObject::Frame();
	if(m_bFadeIn)	FadeIn();
	if(m_bFadeOut)	FadeOut();
	m_ConstantList.Color = TVector4(
		m_fAlpha,
		m_fAlpha,
		m_fAlpha, 1.0f);
	m_ConstantList.Timer = TVector4(
		g_fGameTimer,
		0,
		0,
		1.0f);
	m_pContext->UpdateSubresource(
		m_pConstantBuffer, 0, NULL, &m_ConstantList, 0, 0);
	return true;
}
bool	TImageObject::Render()
{
	TDxObject::Render();
	return true;
}
void    TImageObject::FadeIn()
{
	m_fAlpha += g_fSecPerFrame*0.5f;
	m_fAlpha = min(m_fAlpha, 1.0f);
	if (m_fAlpha >= 1.0f)
	{
		m_bFadeIn = false;
	}

}
void    TImageObject::FadeOut()
{
	m_fAlpha = m_fAlpha - g_fSecPerFrame * 0.5f;
	m_fAlpha = max(m_fAlpha, 0.0f);
	if (m_fAlpha <= 0.0f)
	{
		m_bFadeOut = false;
	}
}
bool	TButtonObject::Frame()
{
	TDxObject::Frame();
	return true;
}
bool	TButtonObject::Render()
{
	TDxObject::Render();
	return true;
}