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
	XVector2  operator / (float fValue)
	{
		XVector2 ret;
		ret.x = x / fValue;
		ret.y = y / fValue;
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
class XVector3
{
public:
	float x;
	float y;
	float z;
	XVector3  operator += (XVector3 p)
	{
		x += p.x;
		y += p.y;
		z += p.z;
		return *this;
	}
	XVector3  operator * (float s)
	{
		XVector3 ret;
		ret.x = x * s;
		ret.y = y * s;
		ret.z = z * s;
		return ret;
	}
	XVector3  operator + (XVector3 p)
	{
		XVector3 ret;
		ret.x = x + p.x;
		ret.y = y + p.y;
		ret.z = z + p.z;
		return ret;
	}
	XVector3  operator - (XVector3 p)
	{
		XVector3 ret;
		ret.x = x - p.x;
		ret.y = y - p.y;
		ret.z = z - p.z;
		return ret;
	}
	XVector3  operator / (float fValue)
	{
		XVector3 ret;
		ret.x = x / fValue;
		ret.y = y / fValue;
		ret.z = z / fValue;
		return ret;
	}
public:
	XVector3() {}
	XVector3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

