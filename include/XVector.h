#pragma once
#include <Windows.h>
#include <math.h>

#define TBASIS_EPSILON		((FLOAT)  0.001f)
#define TBASIS_PI			((FLOAT)  3.141592654f)
#define DegreeToRadian( degree ) ((degree) * (TBASIS_PI / 180.0f))
#define RadianToDegree( radian ) ((radian) * (180.0f / TBASIS_PI))
#define MAKECOLOR_ARGB(a, r, g, b)			(((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)
#define IS_IN_RANGE(value,r0,r1) (( ((r0) <= (value)) && ((value) <= (r1)) ) ? 1 : 0)

namespace TBASIS_VECTOR {

	struct float2
	{
		union
		{
			struct { float x, y; };
			float v[2];
		};
	};

	struct float3
	{
		union
		{
			struct { float x, y, z; };
			float v[3];
		};
	};
	struct float4
	{
		union
		{
			struct { float x, y, z, w; };
			float v[4];
		};
	};
	class XVector3 : public float3
	{
	public:
		XVector3();
		XVector3(const XVector3& v0);
		XVector3(float fX, float fY, float fZ);
		// 연산자 재정의 
		XVector3 operator + (XVector3 const& v0);
		XVector3 operator - (XVector3 const& v0);
		XVector3 operator * (float const& fScala);
		XVector3 operator + (float const& fScala);
		XVector3 operator / (float const& fScala);
		// Dot Product
		float operator | (XVector3 const& v0);
		// Cross Product
		XVector3 operator ^ (XVector3 const& v0);
		bool	operator == (XVector3 const& v0);
		// 제곱
		float LengthSquared();
		// 원점으로 부터의 거리
		float Length();
		XVector3 Normal();
		float Angle(XVector3& v0);
	};
	class XVector4 : public float4
	{
	public:
		XVector4();
		XVector4(const XVector4& v0);
		XVector4(float fX, float fY, float fZ, float fW);
	};

	class XVector2 : public float2
	{
	public:
		XVector2();
		XVector2(const XVector2& v0);
		XVector2(float fX, float fY);
	};
}
using namespace TBASIS_VECTOR;