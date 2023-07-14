#pragma once
#include "TCollision.h"
class TObject
{
public:
	std::wstring    m_csName;
	TPoint			m_Position;
	virtual void	Move() {};
	virtual void	SetTarget(TPoint& t) {};
};
class TStaticObject : public TObject
{
public:	
	void			Move() {};
};
class TDynamicObject : public TObject
{
public:
	float			m_fSpeed = 30.0f;
	TPoint			m_Direction;
	TPoint			m_Target;
	void			SetTarget(TPoint& t)
	{
		m_Target = t;
		// ���� = ������ ��ġ - ���� ��ġ
		m_Direction = t - m_Position;
		// �������͸� ��� ���ؼ� ���⺤���� ũ�⸦ ��´�.
		float fDistance = m_Direction.GetDistance();
		// ����ȭ �۾�
		m_Direction /= fDistance;		
	}
	void			Move()
	{		 

		TPoint vDir = m_Target - m_Position;
		float fD = vDir.GetDistance();
		if (fD > 50.0f)
		{
			// NewPosition = Position + Direction* (t=1.0f)
			TPoint vVelocity = m_Direction * m_fSpeed;
			m_Position = m_Position + vVelocity;
		}
	};
	TDynamicObject()
	{
		m_Direction.x = 0.0f;
		m_Direction.y = 0.0f;
	}
};


