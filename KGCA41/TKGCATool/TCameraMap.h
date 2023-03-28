#pragma once
#include "TCamera.h"
class TCameraMap :  public TCamera
{
public:
	TVector3 m_vBeforePos;
public:
	virtual void  CreateViewMatrix(TVector3 vEye, TVector3 vAt, TVector3 vUp);
	virtual void  CreateProjMatrix(float fNear, float fFar, float fFovY, float fAspectRatio);
	virtual bool  Frame()override;
	virtual void  Update() override;
	virtual void  SetPos(TVector3 vTargetPos, TVector3 vMapHeight)override;
};

