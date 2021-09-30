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
	TVector2  operator / (float fValue)
	{
		TVector2 ret;
		ret.x = x / fValue;
		ret.y = y / fValue;
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
class TVector3
{
public:
	float x;
	float y;
	float z;
	TVector3  operator += (TVector3 p)
	{
		x += p.x;
		y += p.y;
		z += p.z;
		return *this;
	}
	TVector3  operator * (float s)
	{
		TVector3 ret;
		ret.x = x * s;
		ret.y = y * s;
		ret.z = z * s;
		return ret;
	}
	TVector3  operator + (TVector3 p)
	{
		TVector3 ret;
		ret.x = x + p.x;
		ret.y = y + p.y;
		ret.z = z + p.z;
		return ret;
	}
	TVector3  operator - (TVector3 p)
	{
		TVector3 ret;
		ret.x = x - p.x;
		ret.y = y - p.y;
		ret.z = z - p.z;
		return ret;
	}
	TVector3  operator / (float fValue)
	{
		TVector3 ret;
		ret.x = x / fValue;
		ret.y = y / fValue;
		ret.z = z / fValue;
		return ret;
	}
public:
	TVector3() {}
	TVector3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

class TVector4
{
public:
	float x;
	float y;
	float z;
	float w;
	TVector4() {}
	TVector4(float x, float y, float z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}
};