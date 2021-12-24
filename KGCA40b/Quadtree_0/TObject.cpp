#include "TObject.h"
void   TObject::SetPos(TVector2    pos)
{
	m_vPos = pos;
}
void   TObject::SetPos(float x, float y)
{
	m_vPos.x = x;
	m_vPos.y = y;
}
void   TObject::SetRect(TRect rt)
{
	m_rt = rt;
}