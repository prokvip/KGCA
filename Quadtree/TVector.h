#pragma once
#include <math.h>
struct TVector2
{
	float x;
	float y;
	TVector2  operator += (TVector2 p)
	{
		x += p.x;
		y += p.y;
		return *this;
	}
	TVector2  operator * (float s)
	{
		TVector2 ret;
		ret.x = x * s;
		ret.y = y * s;
		return ret;
	}
	TVector2  operator + (TVector2 p)
	{
		TVector2 ret;
		ret.x = x + p.x;
		ret.y = y + p.y;
		return ret;
	}
	TVector2  operator - (TVector2 p)
	{
		TVector2 ret;
		ret.x = x - p.x;
		ret.y = y - p.y;
		return ret;
	}
	// ³»Àû
	float   operator | (TVector2 p);
	float   DotProduct(TVector2 a, TVector2 b);
	float   ProjectionPoint(TVector2 q1, TVector2 q2, TVector2 p);
	TVector2 Normalize();
	float    Length();
	//bool     PointToRect(Rect desk) { return true; };
public:
	TVector2() {}
	TVector2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
};
class TVector
{
};

