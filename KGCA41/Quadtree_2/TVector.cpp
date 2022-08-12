#include "TVector.h"
TVector2D::TVector2D()
{
	x = y = 0.0f;
}
TVector2D::TVector2D(float x, float y)
{
	v[0] = x;
	v[1] = y;
}
TVector2D::TVector2D(const TVector2D& v)
{
	x = v.x;
	y = v.y;
}
TVector2D TVector2D::operator + (TVector2D& v)
{
	return TVector2D(x + v.x, y + v.y);
}
TVector2D TVector2D::operator - (TVector2D& v)
{
	return TVector2D(x - v.x, y - v.y);
}
TVector2D TVector2D::operator * (float scala)
{
	return TVector2D(x *scala, y * scala);
}
TVector2D TVector2D::operator / (float scala)
{
	return TVector2D(x / scala, y / scala);
}
TVector2D TVector2D::operator *= (float scala)
{
	x *= scala;
	y *= scala;
	return *this;
}
TVector2D TVector2D::operator += (TVector2D& v)
{
	x += v.x;
	y += v.y;
	return *this;
}
TVector2D TVector2D::operator -= (TVector2D& v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}
bool      TVector2D::operator == (TVector2D& v)
{
	if (fabs(x - v.x) < T_Epsilon)
	{
		if (fabs(y - v.y) < T_Epsilon)
		{
			return true;
		}
	}
	return false;
}

float TVector2D::LengthSquared()
{
	return (x*x + y*y);
}

float TVector2D::Length()
{
	return sqrt(LengthSquared());
}
// 정규화 -> 단위벡터(자신을) 계산
void TVector2D::Normalized()
{
	float fInvertLength = 1.0f/ Length();
	x = x * fInvertLength;
	y = y * fInvertLength;
}
// 단위벡터 반환
TVector2D TVector2D::Identity()
{
	TVector2D ret = *this;
	float fInvertLength = 1.0f / Length();
	ret.x = ret.x * fInvertLength;
	ret.y = ret.y * fInvertLength;
	return ret;
}
float     TVector2D::Angle(TVector2D& v)
{
	float fCos = x * v.x + y * v.y;//내적
	fCos = fCos / (Length() * v.Length());
	float fRadian = acos(fCos);
	float fDegree = RadianToDegree(fRadian);
	return fDegree;
}