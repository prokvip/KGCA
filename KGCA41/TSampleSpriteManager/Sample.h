#pragma once
#include "TGameCore.h"
#include "TUser2D.h"
typedef std::vector<RECT>  RECT_ARRAY;
class TSprite : public TObject2D
{

	TRect    m_rtCollision;
public:
	// È­¸é ÁÂÇ¥ -> NDC ÁÂÇ¥ 
	void  SetPosition(TVector2D vPos)
	{
		m_vPos = vPos;
		TVector2D	vDrawSize;
		vDrawSize.x = m_rtInit.w / 2.0f;
		vDrawSize.y = m_rtInit.h / 2.0f;	
		m_rtCollision.Set(
			vPos.x - vDrawSize.x,
			vPos.y - vDrawSize.y,
			m_rtInit.w,
			m_rtInit.h);
		// 0  ~ 800   -> 0~1 ->  -1 ~ +1
		m_vDrawPos.x = (m_rtCollision.x1 / g_rtClient.right) * 2.0f - 1.0f;
		m_vDrawPos.y = -((m_rtCollision.y1 / g_rtClient.bottom) * 2.0f - 1.0f);
		m_vDrawSize.x = (m_rtInit.w / g_rtClient.right) * 2;
		m_vDrawSize.y = (m_rtInit.h / g_rtClient.bottom) * 2;
		UpdateVertexBuffer();
	}
};
class Sample : public TGameCore
{
	TVector2D		m_vCamera;
	TUser2D*		m_pUser;
	TSprite*	   m_pObject;
	std::vector<RECT_ARRAY> m_rtSpriteList;
public:
	bool GameDataLoad(const TCHAR* pszLoad);
	virtual bool		Init() override;
	virtual bool		Frame() override;
	virtual bool		Render() override;
	virtual bool		Release() override;
};

