#pragma once
#include "TStd.h"
class TObject
{
public:
	int			m_iNodeIndex;	
	float		m_fMass;
	TVector3	m_vPos;
	TVector3	m_vVelocity; //�ӵ�
	TVector3	m_vAcceleration; //���ӵ�
	TVector3	m_vForces; //��� �� �ջ�
	TRect		m_tRect;
public:
	TObject();
	TObject(TVector3 vPos, TVector3 rect);
};
