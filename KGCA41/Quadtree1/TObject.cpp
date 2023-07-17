#include "TObject.h"
void			TDynamicObject::SetTarget(TPoint& t)
{
	m_Target = t;
	// 방향 = 목적지 위치 - 현재 위치
	m_Direction = t - m_Position;
	// 단위벡터를 얻기 위해서 방향벡터의 크기를 얻는다.
	float fDistance = m_Direction.GetDistance();
	// 정규화 작업
	m_Direction /= fDistance;
}
void			TDynamicObject::Move(float fSecond)
{

	TPoint vDir = m_Target - m_Position;
	float fD = vDir.GetDistance();
	if (fD > 5.0f)
	{
		// 시간의 동기화
		TPoint vVelocity = m_Direction * m_fSpeed * fSecond;
		m_Position = m_Position + vVelocity;
	}
};
TDynamicObject::TDynamicObject()
{
	m_Direction.x = 0.0f;
	m_Direction.y = 0.0f;
}