#pragma once
#include <iostream>

#include "TCollision.h"

class TObject
{
public:
	TVector2    m_vPos;
	TRect		m_rt;
public:
	void   SetPos(TVector2    pos);
	void   SetPos(float x, float y);
	void   SetRect(TRect rt);
	TObject(int x, int y)
	{
		m_vPos.x = x;
		m_vPos.y = y;
	}
	TObject()
	{
		m_vPos.x = 0;
		m_vPos.y = 0;
	}
};

