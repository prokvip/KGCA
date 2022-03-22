#pragma once
#include "TStd.h"
class TCamera
{
public:
	TVector3			m_vCamera;
	TVector3			m_vTarget;
	TVector3			m_vDefaultUp;

	TVector3			m_vLight; //x	
	TVector3			m_vUp; //y 
	TVector3			m_vLook; // z	

	TMatrix				m_matWorld;
	TMatrix				m_matView;
	TMatrix				m_matProj;
public:
	virtual bool	Init();
	virtual bool	Frame();
	TCamera();
	virtual ~TCamera() {}
};

