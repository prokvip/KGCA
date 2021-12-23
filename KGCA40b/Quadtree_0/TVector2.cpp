#include "TVector2.h"
TVector2::TVector2()
{
	x = y = 0;
}
TVector2::TVector2(float x, float y)
{
	this->x = x;
	this->y = y;
}
TVector2::TVector2(const TVector2& v)
{
	x = v.x;
	y = v.y;
}
TVector2 TVector2::operator + (const TVector2& v)
{
	this->x += v.x;
	this->y += v.y;
	return *this;
}