#include "TObject.h"
void   TObject::SetPos(TVector2    pos)
{
	m_vPos = pos;
}
void   TObject::SetPos(float x, float y)
{
	m_vPos.x = x;
	m_vPos.y = y;
	TVector2 vMin;
	vMin.x = m_vPos.x - (m_rt.size.x/2);
	vMin.y = m_vPos.y - (m_rt.size.y / 2);;
	TRect rt(vMin, (m_rt.size.x / 2) * 2.0f, 
					(m_rt.size.y / 2) * 2.0f);
	SetRect(rt);
}
void   TObject::SetRect(TRect rt)
{
	m_rt = rt;
}