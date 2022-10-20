#pragma once
#include "TBaseObject.h"
class TCamera : public TBaseObject
{
public:
	TVector   m_vPos;
	TVector   m_vLook;//z axis
	TVector   m_vUp = { 0,1,0 };;// y
	TVector   m_vRight; //z
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
	void Update();
};

