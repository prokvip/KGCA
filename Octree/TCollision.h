#pragma once
#include "TStd.h"
// 2D ->  È­¸éÁÂÇ¥°è
class TCollision2D
{
public:	
	/*static bool  PointToPoint(TVector2 desk, TVector2 src);	
	static bool  LineToLine(TLine desk, TLine src);
	static bool  LineToPoint(TLine desk, TVector2 src);
	static bool  LineToCircle(TLine desk, TCircle src);
	static bool  LineToRect(TLine desk, TCircle src);
	static bool  RectToRect(TRect desk, TRect src);
	static bool  RectToPoint(TRect desk, TVector2 pos);
	static bool  CircleToCircle(TLine desk, TCircle src);
	static bool  CircleToPoint(TLine desk, TCircle src);
	static TRect UnionRegion(const TRect& r1, const TRect& r2);
	static bool  IntersectRegion(const TRect& r1, const TRect& r2, TRect* rtIntersect);*/
};
class TCollision
{
public:
	// 3D - > Á÷°¢ÁÂÇ¥°è
	static bool  LineToPoint(TLine desk, TVector3 src) { return true; };
	static bool  LineToSphere(TLine desk, TSphere src) { return true; };
	static bool  RectToRect(TBox desk, TBox src) { return true; };
	static bool  RectToPoint(TBox desk, TLine pos) { return true; };
	static TBox  UnionRegion(const TBox& r1, const TBox& r2);
	static bool  IntersectRegion(const TBox& r1, const TBox& r2, TBox* rtIntersect);
};

