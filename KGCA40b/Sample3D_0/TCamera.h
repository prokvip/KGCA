#pragma once
#include "TStd.h"
class TCamera
{
public:
	TVector3			m_vCamera;
	TMatrix				m_matWorld;
	TMatrix				m_matView;
	TMatrix				m_matProj;
public:
	bool	Frame();
	TCamera()
	{
		m_vCamera.x = 0.0f;
		m_vCamera.y = 0.0f;
		m_vCamera.z = -5.0f;		
	}
};

