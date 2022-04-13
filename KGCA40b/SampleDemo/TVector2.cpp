#include "TVector2.h"
namespace TMath
{
	TVector2::TVector2()
	{
		x = y = 0.0f;
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
		TVector2 ret;
		ret.x = this->x + v.x;
		ret.y = this->y + v.y;
		return ret;
	}
	TVector2 TVector2::operator - (const TVector2& v)
	{
		TVector2 ret;
		ret.x = this->x - v.x;
		ret.y = this->y - v.y;
		return ret;
	}

	TVector2 TVector2::operator * (float fValue)
	{
		TVector2 ret;
		ret.x = this->x * fValue;
		ret.y = this->y * fValue;
		return ret;
	}
	TVector2 TVector2::operator / (float fValue)
	{
		TVector2 ret;
		ret.x = this->x / fValue;
		ret.y = this->y / fValue;
		return ret;
	}
	TVector2 TVector2::operator += (const TVector2& v)
	{
		this->x = this->x + v.x;
		this->y = this->y + v.y;
		return *this;
	}
	TVector2 TVector2::operator -= (const TVector2& v)
	{
		this->x = this->x - v.x;
		this->y = this->y - v.y;
		return *this;
	}
	bool TVector2::operator == (const TVector2& v)
	{
		if (fabs(x - v.x) < 0.0001f)
		{
			if (fabs(y - v.y) < 0.0001f)
			{
				return true;
			}
		}
		return false;
	}
	bool TVector2::operator != (const TVector2& v)
	{
		if (fabs(x - v.x) < 0.0001f)
		{
			if (fabs(y - v.y) < 0.0001f)
			{
				return false;
			}
		}
		return true;
	}

	TVector2 TVector2::Normal()
	{
		TVector2 ret;
		float length = 1.0f / Length();
		ret.x = x * length;
		ret.y = y * length;
		return ret;
	}
	TVector2 TVector2::Normalize()
	{
		float length = 1.0f / Length();
		x *= length;
		y *= length;
		return *this;
	}
	TVector2 Normalize(TVector2& v)
	{
		TVector2 ret;
		float length = 1.0f / v.Length();
		ret.x = v.x * length;
		ret.y = v.y * length;
		return ret;
	}
	// 크기
	float TVector2::Length()
	{
		float ret = x * x + y * y;
		return sqrt(ret);
	}
	// 두벡터의 거리 반환
	float TVector2::Distance(TVector2& v)
	{
		float fDist = (*this - v).Length();
		return fDist;
	}
}