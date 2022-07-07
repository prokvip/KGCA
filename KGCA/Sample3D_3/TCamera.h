#pragma once
#include "TFrustum.h"
class TCamera : public TFrustum
{
public:
	float				m_fRadius=10.0f;
	T::TQuaternion		m_qRotation;
	T::TVector3			m_vCamera;
	T::TVector3			m_vTarget;
	T::TVector3			m_vDefaultUp;

	T::TVector3			m_vRight; //x	
	T::TVector3			m_vUp; //y 
	T::TVector3			m_vLook; // z	

	T::TMatrix				m_matWorld;
	T::TMatrix				m_matView;
	T::TMatrix				m_matProj;
public:

	void CreateViewMatrix(T::TVector3 p,T::TVector3 t=T::TVector3(0, 0, 0), T::TVector3 u = T::TVector3(0,1,0));
	void CreateProjMatrix(float fovy = XM_PI*0.25f, float Aspect = 1.44f, float zn=0.1f, float zf=5000.0f);
	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	Update(T::TVector4 vValue);
	void MoveLook(float fValue);
	void MoveSide(float fValue);
	void MoveUp(float fValue);
	bool UpdateVector();
	TCamera();
	virtual ~TCamera() {}
};

