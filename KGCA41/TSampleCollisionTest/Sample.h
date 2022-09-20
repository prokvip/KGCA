#pragma once
#include "TGameCore.h"
#include "TUser2D.h"
#include "TMapObject.h"
#include "TSpriteManager.h"
#include "TNpc2D.h"
struct TEffect
{
	TVector2D   m_vPos;
	TVector2D   m_vDir = { 0,-1 };
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
	virtual bool Update()
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
	
};
struct TProjectile : TEffect
{
	virtual bool Update()
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
	TProjectile()
	{
		m_fLifeTime = 3.33f;
		m_fSpeed = 600.0f;
	}
};
class Sample : public TGameCore
{
	TVector2D		m_vCamera;
	TMapObject*		m_pMap;
	TUser2D*		m_pUser;
	// 0 : 50, 1: 30, 2: 20
	std::list<TEffect*>		m_pEffectList;
	std::list<TNpc2D*>		m_pNpcList;
	std::list<TProjectile*>  m_Projectile;
public:
	void AddProjectile(TVector2D pos);
	void AddEffect();
	void AddEffect(float x, float y);
	virtual bool		Init() override;
	virtual bool		Frame() override;
	virtual bool		Render() override;
	virtual bool		Release() override;
};

