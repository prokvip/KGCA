#include "TObject.h"
bool	TObject::Update(TVector3 vVelocity, float time)
{
	TVector3 pos = m_vPos;
	pos = m_vPos + vVelocity * time * m_fSpeed;
	SetPosition(pos.x, pos.y, pos.z);
	return true;
}
void   TObject::SetPos(TVector3    pos)
{
	m_vPos = pos;
}
void   TObject::SetPos(float x, float y, float z)
{
	m_vPos.x = x;
	m_vPos.y = y;
	m_vPos.z = z;
}
void   TObject::SetPosition(float x, float y, float z)
{
	m_vPos.x = x;
	m_vPos.y = y;
	m_vPos.z = z;
	TVector3 vMin;
	vMin.x = m_vPos.x - (m_rt.size.x / 2);
	vMin.y = m_vPos.y - (m_rt.size.y / 2);
	vMin.z = m_vPos.z - (m_rt.size.z / 2);
	TBox rt(vMin, (m_rt.size.x / 2) * 2.0f,	(m_rt.size.y / 2) * 2.0f,
					(m_rt.size.z / 2) * 2.0f);
	SetBox(rt);
}
void   TObject::SetBox(TBox rt)
{
	m_rt = rt;
}