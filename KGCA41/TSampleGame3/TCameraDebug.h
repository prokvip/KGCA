#pragma once
#include "TCamera.h"
class TCameraDebug : public TCamera
{
public:
	virtual void  CreateViewMatrix(TVector vEye, TVector vAt, TVector vUp);
	virtual void  CreateProjMatrix(float fNear, float fFar, float fFovY, float fAspectRatio);
	virtual bool  Frame();
};

