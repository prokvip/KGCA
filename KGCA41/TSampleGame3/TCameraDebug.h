#pragma once
#include "TCamera.h"
#include "TFrustum.h"
class TCameraDebug : public TCamera
{
public:
	TFrustum   m_vFrustum;
public:
	virtual void  CreateViewMatrix(TVector vEye, TVector vAt, TVector vUp);
	virtual void  CreateProjMatrix(float fNear, float fFar, float fFovY, float fAspectRatio);
	virtual bool  Frame();
	virtual void  Update() override;
};

