#pragma once
#include <iostream>
#include <math.h>

struct TFloat2
{
	union
	{
		struct { float x, y; };
		float v[2];
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
	bool operator == (const TVector2& v);
	bool operator != (const TVector2& v);
	// 정규화
	TVector2 Normalize();
	TVector2 Normal();
	friend TVector2 Normalize(TVector2& v);
	// 크기
	float Length();
};

