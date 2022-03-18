#pragma once
#include "TStd.h"
class TCamera
{
public:
	TVector3			m_vCamera;
	TVector3			m_vTarget;
	TVector3			m_vUp;

	TMatrix				m_matWorld;
	TMatrix				m_matView;
	TMatrix				m_matProj;
public:
	virtual bool	Init();
	virtual bool	Frame();
	TCamera();
	virtual ~TCamera() {}
};

