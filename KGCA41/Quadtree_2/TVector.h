#pragma once
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
	TVector2D operator *= (float scala);
	bool      operator == (TVector2D& v);
public:
	float     LengthSquared();
	float     Length();
	void      Normalized();
	TVector2D Identity();
	float     Angle(TVector2D& v);
};

