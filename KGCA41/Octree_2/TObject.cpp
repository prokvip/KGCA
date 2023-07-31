#include "TObject.h"
void   TObject2D::SetRect(TVector2& c, float w, float h)
{
	m_tRT.Set(c, w, h);
}
void	TDynamicObject2D::SetTarget(TVector2& t)
{
	m_Target = t;
	// 방향 = 목적지 위치 - 현재 위치
	m_Direction = t - m_Position;
	// 단위벡터를 얻기 위해서 방향벡터의 크기를 얻는다.
	float fDistance = m_Direction.Length();
	// 정규화 작업
	m_Direction /= fDistance;
}
void			TDynamicObject2D::Move(float fSecond)
{
	TVector2 vDir = m_Target - m_Position;
	float fD = vDir.Length();
	//	// 시간의 동기화
	TVector2 vVelocity = m_Direction * m_fSpeed * fSecond;
	m_Position = m_Position + vVelocity;
	m_tRT.Set(m_Position, m_tRT.m_fWidth, m_tRT.m_fHeight);
	//if (fD > 5.0f)
	//{
	//	// 시간의 동기화
	//	TVector2 vVelocity = m_Direction * m_fSpeed * fSecond;
	//	m_Position = m_Position + vVelocity;
	//	m_Position.x = max(m_Position.x, 0.0f);
	//	m_Position.y = max(m_Position.y, 0.0f);
	//	m_Position.x = min(m_Position.x, 800.0f);
	//	m_Position.y = min(m_Position.y, 600.0f);
	//}
	if (m_Position.x < 0.0f)
	{
		m_Direction.x *= -1.0f;
		m_Position.x = 0.0f;
	}
	if (m_Position.y < 0.0f)
	{
		m_Direction.y *= -1.0f;
		m_Position.y = 0.0f;
	}

	if (m_Position.x > 800.0f)
	{
		m_Direction.x *= -1.0f;
		m_Position.x = 800.0f;
	}
	if (m_Position.y > 600.0f)
	{
		m_Direction.y *= -1.0f;
		m_Position.y = 600.0f;
	}
};
TDynamicObject2D::TDynamicObject2D()
{
	m_Direction.x = 0.0f;
	m_Direction.y = 0.0f;
}


void   TObject3D::SetBox(TVector3& c, float w, float h, float z)
{
	m_tBox.Set(c, w, h, z);
}
void	TDynamicObject3D::SetTarget(TVector3& t)
{
	m_Target = t;
	// 방향 = 목적지 위치 - 현재 위치
	m_Direction = t - m_Position;
	// 단위벡터를 얻기 위해서 방향벡터의 크기를 얻는다.
	float fDistance = m_Direction.Length();
	// 정규화 작업
	m_Direction /= fDistance;
}
void			TDynamicObject3D::Move(float fSecond)
{
	TVector3 vDir = m_Target - m_Position;
	float fD = vDir.Length();
	//	// 시간의 동기화
	TVector3 vVelocity = m_Direction * m_fSpeed * fSecond;
	m_Position = m_Position + vVelocity;
	m_tBox.Set(m_Position, m_tBox.m_fWidth, m_tBox.m_fHeight, m_tBox.m_fDepth);
	//if (fD > 5.0f)
	//{
	//	// 시간의 동기화
	//	TVector2 vVelocity = m_Direction * m_fSpeed * fSecond;
	//	m_Position = m_Position + vVelocity;
	//	m_Position.x = max(m_Position.x, 0.0f);
	//	m_Position.y = max(m_Position.y, 0.0f);
	//	m_Position.x = min(m_Position.x, 800.0f);
	//	m_Position.y = min(m_Position.y, 600.0f);
	//}
	if (m_Position.x < 0.0f)
	{
		m_Direction.x *= -1.0f;
		m_Position.x = 0.0f;
	}
	if (m_Position.y < 0.0f)
	{
		m_Direction.y *= -1.0f;
		m_Position.y = 0.0f;
	}
	if (m_Position.z < 0.0f)
	{
		m_Direction.z *= -1.0f;
		m_Position.z = 0.0f;
	}

	if (m_Position.x > 800.0f)
	{
		m_Direction.x *= -1.0f;
		m_Position.x = 800.0f;
	}
	if (m_Position.y > 600.0f)
	{
		m_Direction.y *= -1.0f;
		m_Position.y = 600.0f;
	}
	if (m_Position.z > 600.0f)
	{
		m_Direction.z *= -1.0f;
		m_Position.z = 600.0f;
	}

};
TDynamicObject3D::TDynamicObject3D()
{
	m_Direction.x = 0.0f;
	m_Direction.y = 0.0f;
	m_Direction.z = 0.0f;
}