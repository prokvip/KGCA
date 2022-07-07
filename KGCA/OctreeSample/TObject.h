#pragma once
#include <iostream>

#include "TCollision.h"

class TObject
{
public:
	TVector3    m_vPos;
	TBox		m_rt;
	int			m_iNodeIndex;
public:
	TVector3    m_vForces;
	TVector3    m_vAcceleration;
	TVector3    m_vVelocity;
	float		m_fMass;	
	float		m_fSpeed;
	//  ������ġ = ������ġ + (���⺤��* �Ÿ�)
	//  F = MA // �� = ����*���ӵ�
	//  A = F/M
	// m_vAcceleration =  m_vForces / m_fMass;	
	// m_vVelocity +=  m_vAcceleration * time;
	// m_vPos += m_vVelocity * time;
	// max(0, m_vVelocity--);
public:
	void   SetPosition(float x, float y, float z);
	void   SetPos(TVector3    pos);
	void   SetPos(float x, float y, float z);
	void   SetBox(TBox rt);
	bool   Update(TVector3 vVelocity, float time);
	TObject(int x, int y, int z)
	{
		m_vPos.x = x;
		m_vPos.y = y;
		m_vPos.z = z;
		m_fSpeed = 3.0f;
	}
	TObject()
	{
		m_vPos.x = 0;
		m_vPos.y = 0;
		m_vPos.z = 0;
		m_fSpeed = 3.0f;
	}
};

