#pragma once
#include "TStd.h"
class TCamera
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

