#pragma once
#include <iostream>

#include "TCollision.h"

class TObject
{
public:
	TVector2    m_vPos;
	TRect		m_rt;
	int			m_iNodeIndex;
public:
	TVector2    m_vForces;
	TVector2    m_vAcceleration;
	float		m_fMass;
	TVector2    m_vVelocity;
	float		m_fSpeed; 
	//  최종위치 = 현재위치 + (방향벡터* 거리)
	//  F = MA // 힘 = 질량*가속도
	//  A = F/M
	// m_vAcceleration =  m_vForces / m_fMass;	
	// m_vVelocity +=  m_vAcceleration * time;
	// m_vPos += m_vVelocity * time;
	// max(0, m_vVelocity--);
public:
	void   SetPosition(float x, float y);
	void   SetPos(TVector2    pos);
	void   SetPos(float x, float y);
	void   SetRect(TRect rt);
	bool   Update(TVector2 vVelocity, float time);
	TObject(int x, int y)
	{
		m_vPos.x = x;
		m_vPos.y = y;
		m_fSpeed = 3.0f;
	}
	TObject()
	{
		m_vPos.x = 0;
		m_vPos.y = 0;
		m_fSpeed = 3.0f;
	}
};

