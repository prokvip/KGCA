#include "TCollision.h"
//bool  TCollision2D::PointToPoint(XVector2 desk, XVector2 src)
//{
//	if (fabs(desk.x - src.x) < T_EPSILON)
//	{
//		if (fabs(desk.y - src.y) < T_EPSILON)
//		{
//			return true;
//		}
//	}
//	return false;
//}
//bool  TCollision2D::LineToPoint(TLine desk, XVector2 p) 
//{ 
//	return true; 
//};
//bool  TCollision2D::LineToCircle(TLine desk, TCircle p)
//{ 
//	return true; 
//};
//bool  TCollision2D::RectToRect(TRect desk, TRect src) 
//{ 
//	return true; 
//};
//bool  TCollision2D::RectToPoint(TRect desk, XVector2 pos) 
//{
//	return true; 
//};
//TRect TCollision2D::UnionRegion(const TRect& r1, const TRect& r2)
//{
//	TRect rt;
//	return rt;
//}
//bool  TCollision2D::IntersectRegion(const TRect& r1, const TRect& r2, TRect* rtIntersect)
//{
//	return true;
//}

TBox TCollision::UnionRegion(const TBox& r1, const TBox& r2)
{
	TBox rt;
	rt.vMin.x = r1.vMin.x < r2.vMin.x ? r1.vMin.x : r2.vMin.x;
	rt.vMin.y = r1.vMin.y < r2.vMin.y ? r1.vMin.y : r2.vMin.y;
	rt.vMin.z = r1.vMin.z < r2.vMin.z ? r1.vMin.z : r2.vMin.z;

	rt.vMax.x = r1.vMax.x > r2.vMax.x ? r1.vMax.x : r2.vMax.x;
	rt.vMax.y = r1.vMax.y > r2.vMax.y ? r1.vMax.y : r2.vMax.y;
	rt.vMax.z = r1.vMax.z > r2.vMax.z ? r1.vMax.z : r2.vMax.z;

	rt.vSize.x = rt.vMax.x - rt.vMin.x;
	rt.vSize.y = rt.vMax.y - rt.vMin.y;
	rt.vSize.z = rt.vMax.z - rt.vMin.z;

	rt.vPivot.x = rt.vMin.x;
	rt.vPivot.y = rt.vMin.y;
	rt.vPivot.z = rt.vMax.z;
	return rt;
}
bool TCollision::IntersectRegion(const TBox& r1, const TBox& r2, TBox* rtIntersect)
{
	TBox rtUnion = UnionRegion(r1, r2);
	
	if (rtUnion.vSize.x > (r1.vSize.x + r2.vSize.x) &&
		rtUnion.vSize.y > (r1.vSize.y + r2.vSize.y) &&
		rtUnion.vSize.z > (r1.vSize.z + r2.vSize.z))
	{
		if (rtIntersect != nullptr)
		{
			rtIntersect->vMin.x = r1.vMin.x > r2.vMin.x ? r1.vMin.x : r2.vMin.x;
			rtIntersect->vMin.y = r1.vMin.y > r2.vMin.y ? r1.vMin.y : r2.vMin.y;
			rtIntersect->vMin.z = r1.vMin.z > r2.vMin.z ? r1.vMin.z : r2.vMin.z;
			rtIntersect->vMax.x = r1.vMax.x > r2.vMax.x ? r2.vMax.x : r1.vMax.x;
			rtIntersect->vMax.y = r1.vMax.y > r2.vMax.y ? r2.vMax.y : r1.vMax.y;
			rtIntersect->vMax.z = r1.vMax.z > r2.vMax.z ? r2.vMax.z : r1.vMax.z;	

			rtIntersect->vSize		= rtIntersect->vMax - rtIntersect->vMin;
			rtIntersect->vCenter	= (rtIntersect->vMax + rtIntersect->vMin) / 2.0f;

			rtIntersect->vPivot.x = rtIntersect->vMin.x;
			rtIntersect->vPivot.y = rtIntersect->vMin.y;
			rtIntersect->vPivot.z = rtIntersect->vMax.z;

		}
		return true;
	}	
	return false;
}