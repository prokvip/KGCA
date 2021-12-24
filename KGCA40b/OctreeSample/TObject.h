#pragma once
#include <iostream>

#include "TCollision.h"

class TObject
{
public:
	TVector3    m_vPos;
	TBox		m_rt;
public:
	void   SetPos(TVector3    pos);
	void   SetPos(float x, float y, float z);
	void   SetBox(TBox rt);
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

