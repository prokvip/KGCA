#include "TVector.h"

TVector3::TVector3()
{
	x = y = z = 0.0f;
}

TVector3::TVector3(const TVector3& v0)
{
	x = v0.x;
	y = v0.y;
	z = v0.z;
}

TVector3::TVector3(float fX, float fY, float fZ)
{
	x = fX;
	y = fY;
	z = fZ;
}

// 연산자 재정의 
TVector3 TVector3::operator + (TVector3 const& v0)
{
	return TVector3(v0.x + x, v0.y + y, v0.z + z);
}

TVector3 TVector3::operator - (TVector3 const& v0)
{
	return TVector3(x - v0.x, y - v0.y, z - v0.z);
}

TVector3 TVector3::operator + (float const& fScala)
{
	return TVector3(fScala + x, fScala + y, fScala + z);
}
TVector3 TVector3::operator * (float const& fScala)
{
	return TVector3(fScala * x, fScala * y, fScala * z);
}
TVector3 TVector3::operator / (float const& fScala)
{
	float fInvert = 1.0f / fScala;
	return TVector3(x * fInvert, y * fInvert, z * fInvert);
}

// Dot Product
float TVector3::operator | (TVector3 const& v0)
{
	return x * v0.x + y * v0.y + z * v0.z;
}

// Cross Product
TVector3 TVector3::operator ^ (TVector3 const& v0)
{
	return TVector3((y * v0.z - z * v0.y), (z * v0.x - x * v0.z), (x * v0.y - y * v0.x));
}

bool	TVector3::operator == (TVector3 const& v0)
{
	if (fabs(x - v0.x) < TBASIS_EPSILON)
	{
		if (fabs(y - v0.y) < TBASIS_EPSILON)
		{
			if (fabs(z - v0.z) < TBASIS_EPSILON)
			{
				return true;
			}
		}
	}
	return false;
}

// 제곱
float TVector3::LengthSquared()
{
	return (x * x + y * y + z * z);
}

// 원점으로 부터의 거리
float TVector3::Length()
{
	return (float)sqrt(LengthSquared());
}

TVector3 TVector3::Normal()
{
	float invertLength = 1.0f / Length();
	return TVector3(x * invertLength, y * invertLength, z * invertLength);
}

float TVector3::Angle(TVector3& v0)
{
	float fLength1 = Length();
	float fLength2 = v0.Length();
	float fCosAngle = ((*this) | v0) / (fLength1 * fLength2);
	return (float)RadianToDegree(acos(fCosAngle));
}


TVector4::TVector4()
{
	x = y = z = w = 0.0f;
}

TVector4::TVector4(const TVector4& v0)
{
	x = v0.x;
	y = v0.y;
	z = v0.z;
	w = v0.w;
}

TVector4::TVector4(float fX, float fY, float fZ, float fW)
{
	x = fX;
	y = fY;
	z = fZ;
	w = fW;
}


TVector2::TVector2()
{
	x = y = 0.0f;
}

TVector2::TVector2(const TVector2& v0)
{
	x = v0.x;
	y = v0.y;
}

TVector2::TVector2(float fX, float fY)
{
	x = fX;
	y = fY;
}