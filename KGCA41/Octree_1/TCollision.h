#pragma once
#include "TUtils.h"

class TCollision
{
public:
	static bool RectToRect(TRect& rt1, TRect& rt2)
	{
		TRect sum = rt1 + rt2;
		float fX = rt1.m_fWidth + rt2.m_fWidth;
		float fY = rt1.m_fHeight + rt2.m_fHeight;
		if (sum.m_fWidth <= fX)
		{
			if (sum.m_fHeight <= fY)
			{
				return true;
			}
		}
		return false;
	}
	static bool RectToPoint(TRect& rt, TPoint2& p)
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