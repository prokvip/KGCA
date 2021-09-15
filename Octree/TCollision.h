#pragma once
#include "TStd.h"
class TCollision
{
public:
	static bool  LineToPoint(TRect desk, TRect src) { return true; };
	static bool  LineToSphere(TRect desk, TRect src) { return true; };
	static bool  RectToRect(TRect desk, TRect src) { return true; };
	static bool  RectToPoint(TRect desk, TVector3 pos) { return true; };
	static TRect UnionRegion(const TRect& r1, const TRect& r2);
	static bool IntersectRegion(const TRect& r1, const TRect& r2, TRect* rtIntersect);
};

