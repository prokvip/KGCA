#pragma once
#include <math.h>
struct XVector2
{
	float x;
	float y;
	XVector2  operator += (XVector2 p)
	{
		x += p.x;
		y += p.y;
		return *this;
	}
	XVector2  operator * (float s)
	{
		XVector2 ret;
		ret.x = x * s;
		ret.y = y * s;
		return ret;
	}
	XVector2  operator + (XVector2 p)
	{
		XVector2 ret;
		ret.x = x + p.x;
		ret.y = y + p.y;
		return ret;
	}
	XVector2  operator - (XVector2 p)
	{
		XVector2 ret;
		ret.x = x - p.x;
		ret.y = y - p.y;
		return ret;
	}
	// ³»Àû
	float   operator | (XVector2 p);
	float   DotProduct(XVector2 a, XVector2 b);
	float   ProjectionPoint(XVector2 q1, XVector2 q2, XVector2 p);
	XVector2 Normalize();
	float    Length();
	//bool     PointToRect(Rect desk) { return true; };
public:
	XVector2() {}
	XVector2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
};
class TVector
{
};

