#pragma once
#include <math.h>
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
	class TVector3 : public float3
	{
	public:
		TVector3();
		TVector3(const TVector3& v0);
		TVector3(float fX, float fY, float fZ);
		// 연산자 재정의 
		TVector3 operator + (TVector3 const& v0);
		TVector3 operator - (TVector3 const& v0);
		TVector3 operator * (float const& fScala);
		TVector3 operator + (float const& fScala);
		TVector3 operator / (float const& fScala);
		// Dot Product
		float operator | (TVector3 const& v0);
		// Cross Product
		TVector3 operator ^ (TVector3 const& v0);
		bool	operator == (TVector3 const& v0);
		// 제곱
		float LengthSquared();
		// 원점으로 부터의 거리
		float Length();
		TVector3 Normal();
		float Angle(TVector3& v0);
	};
	class TVector4 : public float4
	{
	public:
		TVector4();
		TVector4(const TVector4& v0);
		TVector4(float fX, float fY, float fZ, float fW);
	};

	class TVector2 : public float2
	{
	public:
		TVector2();
		TVector2(const TVector2& v0);
		TVector2(float fX, float fY);
	};
}
using namespace TBASIS_VECTOR;