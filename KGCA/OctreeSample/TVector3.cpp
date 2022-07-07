#include "TVector3.h"
TVector3::TVector3()
{
	x = y = z=0.0f;
}
TVector3::TVector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}
TVector3::TVector3(const TVector3& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
}
TVector3 TVector3::operator + (const TVector3& v)
{
	TVector3 ret;
	ret.x = this->x + v.x;
	ret.y = this->y + v.y;
	ret.z = this->z + v.z;
	return ret;
}
TVector3 TVector3::operator - (const TVector3& v)
{
	TVector3 ret;
	ret.x = this->x - v.x;
	ret.y = this->y - v.y;
	ret.z = this->z - v.z;
	return ret;
}
TVector3 TVector3::operator * (float fValue)
{
	this->x *= fValue;
	this->y *= fValue;
	this->z *= fValue;
	return *this;
}
TVector3 TVector3::operator / (float fValue)
{
	this->x /= fValue;
	this->y /= fValue;
	this->z /= fValue;
	return *this;
}
bool TVector3::operator == (const TVector3& v)
{
	if (fabs(x - v.x) < 0.0001f)
	{
		if (fabs(y - v.y) < 0.0001f)
		{
			if (fabs(z - v.z) < 0.0001f)
			{
				return true;
			}
		}
	}
	return false;
}
bool TVector3::operator != (const TVector3& v)
{
	if (fabs(x - v.x) < 0.0001f)
	{
		if (fabs(y - v.y) < 0.0001f)
		{
			if (fabs(z - v.z) < 0.0001f)
			{
				return false;
			}
		}
	}
	return true;
}

TVector3 TVector3::Normal()
{
	TVector3 ret;
	float length = 1.0f / Length();
	ret.x = x * length;
	ret.y = y * length;
	ret.z = z * length;
	return ret;
}
TVector3 TVector3::Normalize()
{
	float length = 1.0f / Length();
	x *= length;
	y *= length;
	z *= length;
	return *this;
}
TVector3 Normalize(TVector3& v)
{
	TVector3 ret;
	float length = 1.0f / v.Length();
	ret.x = v.x * length;
	ret.y = v.y * length;
	ret.z = v.z * length;
	return ret;
}
// Å©±â
float TVector3::Length()
{
	float ret = x * x + y * y + z*z;
	return sqrt(ret);
}