#pragma once
#include "TBaseObject.h"
class TCamera : public TBaseObject
{
public:
	TVector   m_vPos;
	TVector   m_vLook;//z axis
	TVector   m_vUp = { 0,1,0 };;// y
	TVector   m_vRight; //z
	float     m_fYaw = 0.0f;
	float     m_fPitch = 0.0f;
	float     m_fRoll = 0.0f;
	float     m_fDistance = 0.0f;
	float     m_fSpeed =10.0f;
public:
	TVector			m_vTarget = { 0,0,0 };
	float			m_fNear;
	float			m_fFar;
	float			m_fFovY;
	float			m_fAspectRatio;
public:
	virtual void  CreateViewMatrix(TVector vEye, TVector vAt, TVector vUp);
	virtual void  CreateProjMatrix(float fNear, float fFar, float fFovY, float fAspectRatio);
	virtual bool  Frame();
	virtual void Update();
};
