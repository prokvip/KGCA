#include "TUIObject.h"
#include "TWorld.h"
#include "TSoundMgr.h"
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
	if (m_dwSelectState == TSelectState::T_DEFAULT)
	{
		m_pColorTex = m_pStatePlayList[0].pTex;
		m_dwPreSelectState = m_dwSelectState;
	}
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
	std::string state;
	if (m_dwPreSelectState == m_dwSelectState)
	{
		return;
	}
	switch (m_dwSelectState)
	{
		case TSelectState::T_SELECTED: 
		{
			m_pColorTex = m_pStatePlayList[3].pTex;
			m_pStatePlayList[3].pSound->PlayEffect();
			TWorld::m_pWorld->m_bLoadZone = true;
			state += "T_SELECTED\n";
		}break;
		case TSelectState::T_HOVER: 
		{
			m_pColorTex = m_pStatePlayList[1].pTex;
			m_pStatePlayList[1].pSound->PlayEffect();

			//state += "T_HOVER\n";
		}break;
		case TSelectState::T_ACTIVE:
		{
			m_pColorTex = m_pStatePlayList[2].pTex;
			m_pStatePlayList[2].pSound->PlayEffect();
			state += "T_ACTIVE\n";
		}break;
		case TSelectState::T_FOCUS: 
		{
			m_pColorTex = m_pStatePlayList[2].pTex;
			m_pStatePlayList[2].pSound->PlayEffect();
			//state += "T_FOCUS\n";
		}break;
		default:
		{
			state += std::to_string(m_dwSelectState);
		}
	}	
	m_dwPreSelectState = m_dwSelectState;
	DisplayText(state.c_str());
}