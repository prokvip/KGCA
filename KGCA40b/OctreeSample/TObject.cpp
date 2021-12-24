#include "TObject.h"
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
void   TObject::SetBox(TBox rt)
{
	m_rt = rt;
}