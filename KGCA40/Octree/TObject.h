#pragma once
#include "TStd.h"
class TObject
{
public:
	int			m_iNodeIndex;	
	float		m_fMass;
	XVector3	m_vPos;
	XVector3	m_vVelocity; //속도
	XVector3	m_vAcceleration; //가속도
	XVector3	m_vForces; //모든 힘 합산
	TBox		m_tRect;
public:
	TObject();
	TObject(XVector3 vPos, XVector3 rect);
};
