#pragma once
#include "TUtils.h"

class TCollision
{
public:
	static bool RectToPoint(TRect& rt, TPoint& p)
	{
		if (rt.m_Min.x <= p.x && rt.m_Max.x >= p.x
			&&
			rt.m_Min.y <= p.y && rt.m_Max.y >= p.y)
		{
			return true;
		}
		return false;
	}
};