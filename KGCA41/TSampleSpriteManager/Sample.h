#pragma once
#include "TGameCore.h"
#include "TUser2D.h"
#include "TMapObject.h"
#include "TSpriteManager.h"
struct TEffect
{
	TVector2D   m_vPos;
	TVector2D   m_vDir = { 0,1 };
	float		m_fLifeTime = 1.33f;
	float		m_fSpeed = 300.0f;
	UINT		m_iIndex = 0;
	UINT		m_iMaxIndex=1;
	float		m_fEffectTimer = 0.0f;
	TRect		m_tRect = { 0,0,0,0 };
	float		m_fStep = 1.0f;
	TRect		m_rtCollision;
	TSprite*	m_pSprite=nullptr;//¸®¼Ò½º

	TRect convert(RECT rt)
	{
		TRect tRt;
		tRt.x1 = rt.left;
		tRt.y1 = rt.top;
		tRt.w = rt.right;
		tRt.h = rt.bottom;
		return tRt;
	}
	bool Update()
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
		TVector2D vAdd = m_vDir * m_fSpeed * g_fSecondPerFrame;
		m_vPos = m_vPos + vAdd;
		m_rtCollision.x1 = m_vPos.x;
		m_rtCollision.y1 = m_vPos.y;
		return true;
	}
};
class Sample : public TGameCore
{
	TVector2D		m_vCamera;
	TMapObject*		m_pMap;
	TUser2D*		m_pUser;
	// 0 : 50, 1: 30, 2: 20
	std::list<TEffect*>		m_pEffectList;
public:
	void AddEffect();
	//bool GameDataLoad(const TCHAR* pszLoad);
	virtual bool		Init() override;
	virtual bool		Frame() override;
	virtual bool		Render() override;
	virtual bool		Release() override;
};

