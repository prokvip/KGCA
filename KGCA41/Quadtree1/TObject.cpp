#include "TObject.h"
void			TDynamicObject::SetTarget(TPoint& t)
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
	if (fD > 5.0f)
	{
		// �ð��� ����ȭ
		TPoint vVelocity = m_Direction * m_fSpeed * fSecond;
		m_Position = m_Position + vVelocity;
	}
};
TDynamicObject::TDynamicObject()
{
	m_Direction.x = 0.0f;
	m_Direction.y = 0.0f;
}