#include "XVector.h"

XVector3::XVector3()
{
	x = y = z = 0.0f;
}

XVector3::XVector3(const XVector3& v0)
{
	x = v0.x;
	y = v0.y;
	z = v0.z;
}

XVector3::XVector3(float fX, float fY, float fZ)
{
	x = fX;
	y = fY;
	z = fZ;
}

// 연산자 재정의 
XVector3 XVector3::operator + (XVector3 const& v0)
{
	return XVector3(v0.x + x, v0.y + y, v0.z + z);
}

XVector3 XVector3::operator - (XVector3 const& v0)
{
	return XVector3(x - v0.x, y - v0.y, z - v0.z);
}

XVector3 XVector3::operator + (float const& fScala)
{
	return XVector3(fScala + x, fScala + y, fScala + z);
}
XVector3 XVector3::operator * (float const& fScala)
{
	return XVector3(fScala * x, fScala * y, fScala * z);
}
XVector3 XVector3::operator / (float const& fScala)
{
	float fInvert = 1.0f / fScala;
	return XVector3(x * fInvert, y * fInvert, z * fInvert);
}

// Dot Product
float XVector3::operator | (XVector3 const& v0)
{
	return x * v0.x + y * v0.y + z * v0.z;
}

// Cross Product
XVector3 XVector3::operator ^ (XVector3 const& v0)
{
	return XVector3((y * v0.z - z * v0.y), (z * v0.x - x * v0.z), (x * v0.y - y * v0.x));
}

bool	XVector3::operator == (XVector3 const& v0)
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
float XVector3::LengthSquared()
{
	return (x * x + y * y + z * z);
}

// 원점으로 부터의 거리
float XVector3::Length()
{
	return (float)sqrt(LengthSquared());
}

XVector3 XVector3::Normal()
{
	float invertLength = 1.0f / Length();
	return XVector3(x * invertLength, y * invertLength, z * invertLength);
}

float XVector3::Angle(XVector3& v0)
{
	float fLength1 = Length();
	float fLength2 = v0.Length();
	float fCosAngle = ((*this) | v0) / (fLength1 * fLength2);
	return (float)RadianToDegree(acos(fCosAngle));
}


XVector4::XVector4()
{
	x = y = z = w = 0.0f;
}

XVector4::XVector4(const XVector4& v0)
{
	x = v0.x;
	y = v0.y;
	z = v0.z;
	w = v0.w;
}

XVector4::XVector4(float fX, float fY, float fZ, float fW)
{
	x = fX;
	y = fY;
	z = fZ;
	w = fW;
}


XVector2::XVector2()
{
	x = y = 0.0f;
}

XVector2::XVector2(const XVector2& v0)
{
	x = v0.x;
	y = v0.y;
}

XVector2::XVector2(float fX, float fY)
{
	x = fX;
	y = fY;
}