#include "TUIObject.h"
#include "TWorld.h"
bool	TUIObject::Frame()
{
	TObject2D::Frame();
	return true;
}
bool	TUIObject::Render()
{
	TObject2D::Render();
	return true;
}
bool	TImageObject::Init()
{
	m_bFadeIn = true;
	return true;
}
bool	TImageObject::Frame()
{
	m_vColor.x = m_fAlpha;
	m_vColor.y = m_fAlpha;
	m_vColor.z = m_fAlpha;
	m_vColor.w = 1.0f;
	TObject2D::Frame();	
	return true;
}
bool	TImageObject::Render()
{
	TObject2D::Render();
	return true;
}
bool	TButtonObject::Init()
{
	//m_bFadeIn = true;
	return true;
}
bool	TButtonObject::Frame()
{
	//m_vColor.x = m_fAlpha;
	//m_vColor.y = m_fAlpha;
	//m_vColor.z = m_fAlpha;
	//m_vColor.w = 1.0f;
	TObject2D::Frame();
	return true;
}
bool	TButtonObject::Render()
{
	TObject2D::Render();
	return true;
}
void TButtonObject::HitSelect(TBaseObject* pObj, DWORD dwState)
{
	if (m_dwSelectState == TSelectState::T_SELECTED)
	{		
		TWorld::m_pWorld->m_bLoadZone = true;
	}
}