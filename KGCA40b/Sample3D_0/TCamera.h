#pragma once
#include "TStd.h"
class TCamera
{
public:
	TMath::TVector3			m_vCamera;
	TMath::TVector3			m_vTarget;
	TMath::TVector3			m_vDefaultUp;

	TMath::TVector3			m_vLight; //x	
	TMath::TVector3			m_vUp; //y 
	TMath::TVector3			m_vLook; // z	

	TMath::TMatrix				m_matWorld;
	TMath::TMatrix				m_matView;
	TMath::TMatrix				m_matProj;
public:
	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	Update(TMath::TVector4 vValue);
	TCamera();
	virtual ~TCamera() {}
};

