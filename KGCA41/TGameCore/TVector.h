#pragma once
#include <windows.h>
#include <iostream>
#include <math.h>
#define T_PI 3.141592f
#define DegreeToRadian(x)  (x *(T_PI / 180.0f))
#define RadianToDegree(x) (x *(180.0f / T_PI))
#define T_Epsilon 0.001f

class TMatrix;

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
struct TFloat4
{
	union
	{
		struct {
			float x;
			float y;
			float z;
			float w;
		};
		float v[4];
	};
};
class TVector2 : public TFloat2
{
public:
	TVector2();
	TVector2(float x, float y);
	TVector2(const TVector2& v);
public:
	TVector2 operator + (TVector2& v);
	TVector2 operator - (TVector2& v);
	TVector2 operator * (float scala);
	TVector2 operator / (float scala);
	TVector2 operator *= (float scala);
	TVector2 operator += (TVector2& v);
	TVector2 operator -= (TVector2& v);
	bool      operator == (TVector2& v);
	bool      operator <= (TVector2& v);
	bool      operator >= (TVector2& v);
public:
	float     LengthSquared();
	float     Length();
	void      Normalized();
	TVector2 Identity();
	float     Angle(TVector2& v);
};
class TVector4 : public TFloat4
{
public:
	TVector4();
	TVector4(float x, float y, float z, float w);
	TVector4(const TVector4& v);
public:
	TVector4 operator + (TVector4& v);
	TVector4 operator - (TVector4& v);
	TVector4 operator * (float scala);
	TVector4 operator / (float scala);
	TVector4 operator *= (float scala);
	TVector4 operator += (TVector4& v);
	TVector4 operator -= (TVector4& v);
	bool      operator == (TVector4& v);
	bool      operator <= (TVector4& v);
	bool      operator >= (TVector4& v);
	TVector4 operator* (TMatrix& m);
public:
	float     LengthSquared();
	float     Length();
	void      Normalized();
	TVector4 Identity();
	float     Angle(TVector4& v);
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
	// Dot Product
	float operator | (TVector const& v0);
	// Cross Product
	TVector operator ^ (TVector const& v0);
public:
	float     LengthSquared();
	float     Length();
	void      Normalized();
	TVector	  Normal();
	float     Angle(TVector& v);
	TVector  operator* (TMatrix& m);
};

