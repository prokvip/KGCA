#include "TNpcObj.h"
void			TNpcObj::Move(float fSecond)
{
	TVector2 rt = { m_vPos.x, m_vPos.y };
	SetRect(rt, m_vScale.x * 2.0f, m_vScale.y * 2.0f);
};
bool    TNpcObj::Frame()
{
	TBASIS_EX::TMatrix matScale, matRotation, matTranslate;
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixRotationZ(&matRotation, m_vRotation.z);
	D3DXMatrixTranslation(&matTranslate, m_vPos.x, m_vPos.y, m_vPos.z );
	m_matWorld = matScale * matRotation * matTranslate;
	return true;
}
TNpcObj::TNpcObj()
{
	m_vDirection = { randstep(-1, +1), randstep(-1, +1), 0 };
}
TNpcObj::~TNpcObj()
{}