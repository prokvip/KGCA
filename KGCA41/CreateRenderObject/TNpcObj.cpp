#include "TNpcObj.h"
void			TNpcObj::Move(float fSecond)
{
	TVector3 vVelocity = m_vDirection * 500.0f * fSecond;
	m_vPos = m_vPos + vVelocity;

	if (m_vPos.x < -g_fMapSizeX)
	{
		m_vDirection.x *= -1.0f;
		m_vPos.x = -g_fMapSizeX;
	}
	if (m_vPos.y < -g_fMapSizeY)
	{
		m_vDirection.y *= -1.0f;
		m_vPos.y = -g_fMapSizeY;
	}
	if (m_vPos.x > g_fMapSizeX)
	{
		m_vDirection.x *= -1.0f;
		m_vPos.x = g_fMapSizeX;
	}
	if (m_vPos.y > g_fMapSizeY)
	{
		m_vDirection.y *= -1.0f;
		m_vPos.y = g_fMapSizeY;
	}

	TVector2 rt = { m_vPos.x, m_vPos.y };
	SetRect(rt, m_vScale.x * 2.0f, m_vScale.y * 2.0f);
};
bool    TNpcObj::Frame()
{
	TMatrix matScale, matRotation, matTranslate;
	matScale.Scale(m_vScale);
	matRotation.ZRotate(m_vRotation.z);
	matTranslate.Translation(m_vPos);
	m_matWorld = matScale * matRotation * matTranslate;
	return true;
}
TNpcObj::TNpcObj()
{
	m_vDirection = { randstep(-1, +1), randstep(-1, +1), 0 };
}
TNpcObj::~TNpcObj()
{}