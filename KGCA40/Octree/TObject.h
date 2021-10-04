#pragma once
#include "TStd.h"
class TObject
{
public:
	int			m_iNodeIndex;	
	float		m_fMass;
	XVector3	m_vPos;
	XVector3	m_vVelocity; //�ӵ�
	XVector3	m_vAcceleration; //���ӵ�
	XVector3	m_vForces; //��� �� �ջ�
	TBox		m_tRect;
public:
	TObject();
	TObject(XVector3 vPos, XVector3 rect);
};
