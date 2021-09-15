#pragma once
#include "TStd.h"
class TObject
{
public:
	int			m_iNodeIndex;	
	float		m_fMass;
	TVector3	m_vPos;
	TVector3	m_vVelocity; //속도
	TVector3	m_vAcceleration; //가속도
	TVector3	m_vForces; //모든 힘 합산
	TRect		m_tRect;
public:
	TObject();
	TObject(TVector3 vPos, TVector3 rect);
};
