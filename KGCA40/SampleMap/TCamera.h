#pragma once
#include "TStd.h"
class TCamera
{
public:
	float				m_pSpeed;
	TVector3			m_vCameraPos;
	TVector3			m_vCameraTarget;
	TVector3			m_vLook;
	TVector3			m_vSide;
	TVector3			m_vUp;
public:
	TMatrix m_matWorld;
	TMatrix m_matView;
	TMatrix m_matProj;
public:
	virtual TMatrix     CreateViewMatrix(TVector3 vPos, TVector3 vTarget, TVector3 vUp=TVector3(0,1,0));
	virtual TMatrix  	CreateProjMatrix(float fNear, float fFar, float fFov, float fAspect);
public:
	virtual bool		Init();
	virtual bool		Frame();
	virtual bool		Render();
	virtual bool		Release();
public:
	TCamera();
	virtual ~TCamera();
};

class TDebugCamera : public TCamera
{
public:
	virtual bool		Frame() override;
};

