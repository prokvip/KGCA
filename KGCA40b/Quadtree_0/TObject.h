#pragma once
#include <iostream>
#include "TVector2.h"
class TObject
{
public:
	TVector2    m_Pos;
	TObject(int x, int y)
	{
		m_Pos.x = x;
		m_Pos.y = y;
	}
};

