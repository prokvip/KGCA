#pragma once
#include "TUtils.h"
enum  class CollisionTypeA
{
	CT_NONE,
	CT_OVERLAB,
	CT_STAY,
	CT_EXIT,	
};
enum  CollisionTypeB
{
	A_OUTSIDE = 0,
	A_SPANNING = 1,
	A_DestBig,
	A_SrcBig,
};
enum  CollisionType
{
	CT_OUTSIDE = 0,
	CT_SPANNING = 1,
	//CT_INSIDE, 
	CT_DestBig,
	CT_SrcBig,
	//CT_FRONT,
	//CT_BACK,
	//CT_ONPLANE,		
};
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
	static CollisionType BoxToBox(TBox& box1, TBox& box2)
	{
		CollisionType ret= CT_OUTSIDE;
		TVector3 dir = box1.m_Center - box2.m_Center;
		if (BoxInBox(box1, box2))
		{
			ret = CT_DestBig;
			return ret;
		}
		if (BoxInBox(box2, box1))
		{
			ret = CT_SrcBig;
			return ret;
		}
		if (dir.x <= (box1.m_Half.x + box2.m_Half.x))
		{
			if (dir.y <= (box1.m_Half.y + box2.m_Half.y))
			{
				if (dir.z <= (box1.m_Half.z + box2.m_Half.z))
				{
					ret = CT_SPANNING;
				}
			}
		}		
		return ret;
	}
	static bool BoxInBox(TBox& rt1, TBox& rt2)
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