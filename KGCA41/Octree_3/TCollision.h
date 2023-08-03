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
	static bool RectToPoint(TRect& rt, TVector2& p)
	{
		if (rt.m_Min.x <= p.x && rt.m_Max.x >= p.x
			&&
			rt.m_Min.y <= p.y && rt.m_Max.y >= p.y)
		{
			return true;
		}
		return false;
	}
	static bool BoxToBox(TBox& rt1, TBox& rt2)
	{
		if (rt1.m_Min.x <= rt2.m_Min.x && rt1.m_Max.x >= rt2.m_Max.x)
		{
			if (rt1.m_Min.y <= rt2.m_Min.y && rt1.m_Max.y >= rt2.m_Max.y)
			{
				if (rt1.m_Min.z <= rt2.m_Min.z && rt1.m_Max.z >= rt2.m_Max.z)
				{
					return true;
				}
			}
		}
		return false;
	}
	static bool BoxToPoint(TBox& rt, TVector3& p)
	{
		if (rt.m_Min.x <= p.x && rt.m_Max.x >= p.x
			&&
			rt.m_Min.y <= p.y && rt.m_Max.y >= p.y
			&&
			rt.m_Min.z <= p.z && rt.m_Max.z >= p.z)
		{
			return true;
		}
		return false;
	}
};