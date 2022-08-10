#include "TObject.h"

void TObject::Create(TRect rt)
{
	m_rt = rt;
}

void TObject::SetPos(float x, float y)
{
	m_rt.left = x;
	m_rt.top  = y;
}

TObject::TObject()
{

}
TObject::TObject(TRect rt)
{
	m_rt = rt;
}
