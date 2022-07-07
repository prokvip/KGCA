#pragma once
#include <iostream>
#include <math.h>

namespace TMath
{
	struct TFloat2
	{
		union
		{
			struct { float x, y; };
			float v[2];
		};
	};
	struct TFloat4
	{
		union
		{
			struct { float x, y, z, w; };
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
		TVector2 operator + (const TVector2& v);
		TVector2 operator - (const TVector2& v);
		TVector2 operator * (float fValue);
		TVector2 operator / (float fValue);
		TVector2 operator += (const TVector2& v);
		TVector2 operator -= (const TVector2& v);
		bool operator == (const TVector2& v);
		bool operator != (const TVector2& v);
		// 정규화
		TVector2 Normalize();
		TVector2 Normal();
		friend TVector2 Normalize(TVector2& v);
		// 크기
		float Length();
		float Distance(TVector2& v);
	};

	class TVector4 : public TFloat4
	{
	public:
		TVector4() {
			v[0] = 0.0f;
			v[1] = 0.0f;
			v[2] = 0.0f;
			v[3] = 0.0f;
		};
		TVector4(float x, float y, float z, float w) {
			v[0] = x;
			v[1] = y;
			v[2] = z;
			v[3] = w;
		}
		TVector4(const TVector4& v) {
			x = v.x;
			y = v.y;
			z = v.z;
			w = v.w;
		}
	};
}