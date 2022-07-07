#include "TCollision.h"
bool   TCollision::SphereToPoint(TSphere sp, float x, float y, float z)
{
	float fDistance = (sp.vCenter - T::TVector3(x, y, z)).Length();
	if (fDistance <= sp.fRadius)
	{
		return true;
	}
	return false;
}
bool   TCollision::SphereToPoint(TSphere sp, T::TVector3 v)
{
	float fDistance = (sp.vCenter - v).Length();
	if (fDistance <= sp.fRadius)
	{
		return true;
	}
	return false;
}
bool   TCollision::RectToPoint(TRect rt, int x, int y)
{
	if (rt.vMin.x <= x && rt.vMax.x >= x &&
		rt.vMin.y <= y && rt.vMax.y >= y)
	{
		return true;
	}
	return false;
}
bool   TCollision::RectToPoint(TRect rt, T::TVector2 v)
{
	if (rt.vMin.x <= v.x && rt.vMax.x >= v.x &&
		rt.vMin.y <= v.y && rt.vMax.y >= v.y)
	{
		return true;
	}
	return false;
}
TRect   TCollision::UnionRect(TRect rt1, TRect rt2)
{
	TRect rt;
	rt.vMin.x = rt1.vMin.x < rt2.vMin.x ? rt1.vMin.x : rt2.vMin.x;
	rt.vMin.y = rt1.vMin.y < rt2.vMin.y ? rt1.vMin.y : rt2.vMin.y;
	rt.vMax.x = rt1.vMax.x < rt2.vMax.x ? rt2.vMax.x : rt1.vMax.x;
	rt.vMax.y = rt1.vMax.y < rt2.vMax.y ? rt2.vMax.y : rt1.vMax.y;
	rt.size.x = rt.vMax.x - rt.vMin.x;
	rt.size.y = rt.vMax.y - rt.vMin.y;
	rt.vCenter = (rt.vMin + rt.vMax);
	rt.vCenter /= 2.0f;
	return rt;
}
bool   TCollision::IntersectRect(
	TRect rt1, TRect rt2, TRect* pRect)
{
	TRect rt;
	TRect rtUnion = UnionRect(rt1, rt2);
	if (rtUnion.size.x <= (rt1.size.x + rt2.size.x) &&
		rtUnion.size.y <= (rt1.size.y + rt2.size.y))
	{
		if (pRect != nullptr)
		{
			pRect->vMin.x = rt1.vMin.x < rt2.vMin.x ?
				rt2.vMin.x : rt1.vMin.x;
			pRect->vMin.y = rt1.vMin.y < rt2.vMin.y ?
				rt2.vMin.y : rt1.vMin.y;

			pRect->vMax.x = rt1.vMax.x < rt2.vMax.x ?
				rt1.vMax.x : rt2.vMax.x;
			pRect->vMax.y = rt1.vMax.y < rt2.vMax.y ?
				rt1.vMax.y : rt2.vMax.y;

			pRect->size.x = pRect->vMax.x - pRect->vMin.x;
			pRect->size.y = pRect->vMax.y - pRect->vMin.y;
			pRect->vCenter = (pRect->vMax + pRect->vMin);
			pRect->vCenter /= 2.0f;
		}
		return true;
	}
	return false;
}
// 0 :  떨어져 있다.
// 1 :  안에 있다.
// 2 :  걸쳐 있다.
TCollisionResult   TCollision::RectToRect(TRect rt1, 
	TRect rt2)
{	
	TRect rtInterction;
	int iRet = IntersectRect(rt1, rt2, &rtInterction);
	if (iRet <= 0)
	{
		return RECT_OUT;
	}
	if (rtInterction == rt2) return RECT_OVERLAP;
	return RECT_IN;
}

TCollisionResult   TCollision::ToRect(TRect rt1,TRect rt2)
{
	// 거리 판정
	float fDistanceX;
	float fDistanceY;
	fDistanceX = fabs(rt1.vCenter.x - rt2.vCenter.x);
	fDistanceY = fabs(rt1.vCenter.y - rt2.vCenter.y);
	float fToX = rt1.size.x / 2.0f + rt1.size.x / 2.0f;
	float fToY = rt1.size.y / 2.0f + rt1.size.y / 2.0f;
	if (fDistanceX < fToX && fDistanceY < fToY)
	{
		return RECT_OVERLAP;
	}
	return RECT_OUT;
}
bool   TCollision::BoxToPoint(TBox rt, int x, int y, int z)
{
	if (rt.vMin.x <= x && rt.vMax.x >= x &&
		rt.vMin.y <= y && rt.vMax.y >= y &&
		rt.vMin.z <= z && rt.vMax.z >= z)
	{
		return true;
	}
	return false;
}
bool   TCollision::BoxToPoint(TBox rt, T::TVector3 v)
{
	if (rt.vMin.x <= v.x && rt.vMax.x >= v.x &&
		rt.vMin.y <= v.y && rt.vMax.y >= v.y &&
		rt.vMin.z <= v.z && rt.vMax.z >= v.z)
	{
		return true;
	}
	return false;
}
TBox   TCollision::UnionBox(TBox rt1, TBox rt2)
{
	TBox rt;
	rt.vMin.x = rt1.vMin.x < rt2.vMin.x ? rt1.vMin.x : rt2.vMin.x;
	rt.vMin.y = rt1.vMin.y < rt2.vMin.y ? rt1.vMin.y : rt2.vMin.y;
	rt.vMin.z = rt1.vMin.z < rt2.vMin.z ? rt1.vMin.z : rt2.vMin.z;

	rt.vMax.x = rt1.vMax.x < rt2.vMax.x ? rt2.vMax.x : rt1.vMax.x;
	rt.vMax.y = rt1.vMax.y < rt2.vMax.y ? rt2.vMax.y : rt1.vMax.y;
	rt.vMax.z = rt1.vMax.z < rt2.vMax.z ? rt2.vMax.z : rt1.vMax.z;

	rt.size = rt.vMax - rt.vMin;
	rt.vCenter = (rt.vMin + rt.vMax);
	rt.vCenter /= 2.0f;
	return rt;
}
bool   TCollision::IntersectBox(
	TBox rt1, TBox rt2, TBox* pRect)
{
	TBox rt;
	TBox rtUnion = UnionBox(rt1, rt2);
	if (rtUnion.size.x <= (rt1.size.x + rt2.size.x) &&
		rtUnion.size.y <= (rt1.size.y + rt2.size.y) &&
		rtUnion.size.z <= (rt1.size.z + rt2.size.z))
	{
		if (pRect != nullptr)
		{
			pRect->vMin.x = rt1.vMin.x < rt2.vMin.x ?
				rt2.vMin.x : rt1.vMin.x;
			pRect->vMin.y = rt1.vMin.y < rt2.vMin.y ?
				rt2.vMin.y : rt1.vMin.y;
			pRect->vMin.z = rt1.vMin.z < rt2.vMin.z ?
				rt2.vMin.z : rt1.vMin.z;

			pRect->vMax.x = rt1.vMax.x < rt2.vMax.x ?
				rt1.vMax.x : rt2.vMax.x;
			pRect->vMax.y = rt1.vMax.y < rt2.vMax.y ?
				rt1.vMax.y : rt2.vMax.y;
			pRect->vMax.z = rt1.vMax.z < rt2.vMax.z ?
				rt1.vMax.z : rt2.vMax.z;

			pRect->size = pRect->vMax - pRect->vMin;
			pRect->vCenter = (pRect->vMax + pRect->vMin);
			pRect->vCenter /= 2.0f;
		}
		return true;
	}
	return false;
}
// 0 :  떨어져 있다.
// 1 :  안에 있다.
// 2 :  걸쳐 있다.
TCollisionResult   TCollision::BoxToBox(TBox rt1, TBox rt2)
{
	TBox rtInterction;
	int iRet = IntersectBox(rt1, rt2, &rtInterction);
	if (iRet <= 0)
	{
		return RECT_OUT;
	}
	if (rtInterction == rt2) return RECT_OVERLAP;
	return RECT_IN;
}