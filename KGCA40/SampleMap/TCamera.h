#pragma once
#include "TStd.h"
class TCamera
{
public:
	float				m_pSpeed;
	XVector3			m_vCameraPos;
	XVector3			m_vCameraTarget;
	XVector3			m_vLook;
	XVector3			m_vSide;
	XVector3			m_vUp;
public:
	XMatrix m_matWorld;
	XMatrix m_matView;
	XMatrix m_matProj;
public:
	virtual XMatrix     CreateViewMatrix(XVector3 vPos, XVector3 vTarget, XVector3 vUp=XVector3(0,1,0));
	virtual XMatrix  	CreateProjMatrix(float fNear, float fFar, float fFov, float fAspect);
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

