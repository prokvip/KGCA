#pragma once
#include "TStd.h"
class TCamera
{
public:
	float				m_pSpeed;
	TVector3			m_vCameraPos;
	TVector3			m_vCameraTarget;
public:
	TMatrix m_matWorld;
	TMatrix m_matView;
	TMatrix m_matProj;
public:
	TMatrix     CreateViewMatrix(TVector3 vPos, TVector3 vTarget, TVector3 vUp=TVector3(0,1,0));
	TMatrix  	CreateProjMatrix(float fNear, float fFar, float fFov, float fAspect);
public:
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
public:
	TCamera();
	virtual ~TCamera();
};

