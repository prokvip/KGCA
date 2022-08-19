#pragma once
#include <windows.h>
#include <iostream>
#include <math.h>
#define T_PI 3.141592f
#define DegreeToRadian(x)  (x *(T_PI / 180.0f))
#define RadianToDegree(x) (x *(180.0f / T_PI))
#define T_Epsilon 0.001f
struct TFloat2
{
	union
	{
		struct {
			float x;
			float y;
		};
		float v[2];
	};
};
struct TFloat3
{
	union
	{
		struct {
			float x;
			float y;
			float z;
		};
		float v[3];
	};
};
class TVector2D : public TFloat2
{
public:
	TVector2D();
	TVector2D(float x, float y);
	TVector2D(const TVector2D& v);
public:
	TVector2D operator + (TVector2D& v);
	TVector2D operator - (TVector2D& v);
	TVector2D operator * (float scala);
	TVector2D operator / (float scala);
	TVector2D operator *= (float scala);
	TVector2D operator += (TVector2D& v);
	TVector2D operator -= (TVector2D& v);
	bool      operator == (TVector2D& v);
	bool      operator <= (TVector2D& v);
	bool      operator >= (TVector2D& v);
public:
	float     LengthSquared();
	float     Length();
	void      Normalized();
	TVector2D Identity();
	float     Angle(TVector2D& v);
};

class TVector : public TFloat3
{
public:
	TVector();
	TVector(float x, float y, float z);
	TVector(const TVector& v);
public:
	TVector operator + (TVector& v);
	TVector operator - (TVector& v);
	TVector operator * (float scala);
	TVector operator / (float scala);
	TVector operator *= (float scala);
	TVector operator += (TVector& v);
	TVector operator -= (TVector& v);
	bool      operator == (TVector& v);
	bool      operator <= (TVector& v);
	bool      operator >= (TVector& v);
public:
	float     LengthSquared();
	float     Length();
	void      Normalized();
	TVector Identity();
	float     Angle(TVector& v);
};

