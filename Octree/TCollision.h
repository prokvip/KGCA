#pragma once
#include "TStd.h"
class TCollision
{
public:
	//// 2D ->  È­¸éÁÂÇ¥°è
	//static bool  LineToPoint(TRect desk, TRect src) { return true; };
	//static bool  LineToSphere2D(TRect desk, TRect src) { return true; };
	//static bool  RectToRect2D(TRect desk, TRect src) { return true; };
	//static bool  RectToPoint2D(TRect desk, TVector3 pos) { return true; };
	//static TRect UnionRegion(const TRect& r1, const TRect& r2);
	//static bool IntersectRegion(const TRect& r1, const TRect& r2, TRect* rtIntersect);

	// 3D - > Á÷°¢ÁÂÇ¥°è
	static bool  LineToPoint(TRect desk, TRect src) { return true; };
	static bool  LineToSphere(TRect desk, TRect src) { return true; };
	static bool  RectToRect(TRect desk, TRect src) { return true; };
	static bool  RectToPoint(TRect desk, TVector3 pos) { return true; };
	static TRect UnionRegion(const TRect& r1, const TRect& r2);
	static bool IntersectRegion(const TRect& r1, const TRect& r2, TRect* rtIntersect);
};

