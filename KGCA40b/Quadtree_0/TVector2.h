#pragma once
struct TFloat2
{
	union
	{
		float x, y;
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
	// Á¤±ÔÈ­
	// Å©±â
	// º¤ÅÍ »¬¼À, ½ºÄ®¶ó °ö¼À
};

