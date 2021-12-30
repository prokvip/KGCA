#pragma once
#include <iostream>

#include "TCollision.h"

class TObject
{
public:
	TVector2    m_vPos;
	TRect		m_rt;
public:
	TVector2    m_vForces;
	TVector2    m_vAcceleration;
	float		m_fMass;
	TVector2    m_vVelocity;
	//  ������ġ = ������ġ + (���⺤��* �Ÿ�)
	//  F = MA // �� = ����*���ӵ�
	//  A = F/M
	// m_vAcceleration =  m_vForces / m_fMass;	
	// m_vVelocity +=  m_vAcceleration * time;
	// m_vPos += m_vVelocity * time;
	// max(0, m_vVelocity--);
public:
	void   SetPos(TVector2    pos);
	void   SetPos(float x, float y);
	void   SetRect(TRect rt);
	TObject(int x, int y)
	{
		m_vPos.x = x;
		m_vPos.y = y;
	}
	TObject()
	{
		m_vPos.x = 0;
		m_vPos.y = 0;
	}
};

