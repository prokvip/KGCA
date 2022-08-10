#pragma once
#include "TShape.h"
class TObject
{
public:
	TRect			m_rt;
	std::wstring    m_csName;
	int				m_iIndex;
public:
	void			Create(TRect rt);
	void			SetPos(float x, float y);
public:
	TObject();
	TObject(TRect rt);
};
