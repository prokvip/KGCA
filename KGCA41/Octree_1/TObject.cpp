#include "TObject.h"
void   TObject::SetRect(TPoint& c, float w, float h)
{
	m_rt.Set(c, w, h);
}
void	TDynamicObject::SetTarget(TPoint& t)
{
	m_Target = t;
	// ���� = ������ ��ġ - ���� ��ġ
	m_Direction = t - m_Position;
	// �������͸� ��� ���ؼ� ���⺤���� ũ�⸦ ��´�.
	float fDistance = m_Direction.GetDistance();
	// ����ȭ �۾�
	m_Direction /= fDistance;
}
void			TDynamicObject::Move(float fSecond)
{
	TPoint vDir = m_Target - m_Position;
	float fD = vDir.GetDistance();
	//	// �ð��� ����ȭ
	TPoint vVelocity = m_Direction * m_fSpeed * fSecond;
	m_Position = m_Position + vVelocity;
	m_rt.Set(m_Position, m_rt.m_fWidth, m_rt.m_fHeight);
	//if (fD > 5.0f)
	//{
	//	// �ð��� ����ȭ
	//	TPoint vVelocity = m_Direction * m_fSpeed * fSecond;
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
TDynamicObject::TDynamicObject()
{
	m_Direction.x = 0.0f;
	m_Direction.y = 0.0f;
}