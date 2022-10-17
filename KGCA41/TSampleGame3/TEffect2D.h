#pragma once
#include "TSprite.h"
class TEffect2D
{
public:
	TVector2   m_vPos;
	TVector2   m_vDir = { 0,-1 };
	float		m_fLifeTime = 1.33f;
	float		m_fSpeed = 300.0f;
	UINT		m_iIndex = 0;
	UINT		m_iMaxIndex = 1;
	float		m_fEffectTimer = 0.0f;
	TRect		m_tRect = { 0,0,0,0 };
	float		m_fStep = 1.0f;
	TRect		m_rtCollision;
	TSprite*	m_pSprite = nullptr;//¸®¼Ò½º
public:
	TRect convert(RECT rt);
	virtual bool Update();
};

