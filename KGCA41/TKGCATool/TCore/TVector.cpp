#include "pch.h"
#include "TVector.h"
#include "TMatrix.h"
namespace T_TEST
{
	TVector4::TVector4()
	{
		x = y = z = w = 0.0f;
	}
	TVector4::TVector4(float x, float y, float z, float w)
	{
		v[0] = x;
		v[1] = y;
		v[2] = z;
		v[3] = w;
	}
	TVector4::TVector4(const TVector4& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
	}
	TVector4 TVector4::operator + (TVector4& v)
	{
		return TVector4(x + v.x, y + v.y, z + v.z, w + v.w);
	}
	TVector4 TVector4::operator - (TVector4& v)
	{
		return TVector4(x - v.x, y - v.y, z - v.z, w - v.w);
	}
	TVector4 TVector4::operator * (float scala)
	{
		return TVector4(x * scala, y * scala, z * scala, w * scala);
	}
	TVector4 TVector4::operator / (float scala)
	{
		return TVector4(x / scala, y / scala, z / scala, w / scala);
	}
	TVector4 TVector4::operator *= (float scala)
	{
		x *= scala;
		y *= scala;
		z *= scala;
		w *= scala;
		return *this;
	}
	TVector4 TVector4::operator += (TVector4& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
		return *this;
	}
	TVector4 TVector4::operator -= (TVector4& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;
		return *this;
	}
	bool      TVector4::operator == (TVector4& v)
	{
		if (fabs(x - v.x) < T_Epsilon)
		{
			if (fabs(y - v.y) < T_Epsilon)
			{
				if (fabs(z - v.z) < T_Epsilon)
				{
					if (fabs(w - v.w) < T_Epsilon)
					{
						return true;
					}
				}
				return true;
			}
		}
		return false;
	}
	bool      TVector4::operator <= (TVector4& b)
	{
		if (x <= b.x)
		{
			if (y <= b.y)
			{
				if (z <= b.z)
				{
					if (w <= b.w)
					{
						return true;
					}
				}
				return true;
			}
		}
		return false;
	}
	bool      TVector4::operator >= (TVector4& b)
	{
		if (x >= b.x)
		{
			if (y >= b.y)
			{
				if (z >= b.z)
				{
					if (w >= b.w)
					{
						return true;
					}
				}
				return true;
			}
		}
		return false;
	}
	float TVector4::LengthSquared()
	{
		return (x * x + y * y, z * z + w * w);
	}

	float TVector4::Length()
	{
		return sqrt(LengthSquared());
	}
	// 정규화 -> 단위벡터(자신을) 계산
	void TVector4::Normalized()
	{
		float fInvertLength = 1.0f / Length();
		x = x * fInvertLength;
		y = y * fInvertLength;
		z = z * fInvertLength;
		w = w * fInvertLength;
	}
	// 단위벡터 반환
	TVector4 TVector4::Identity()
	{
		TVector4 ret = *this;
		float fInvertLength = 1.0f / Length();
		ret.x = ret.x * fInvertLength;
		ret.y = ret.y * fInvertLength;
		ret.z = ret.z * fInvertLength;
		ret.w = ret.w * fInvertLength;
		return ret;
	}
	float     TVector4::Angle(TVector4& v)
	{
		float fCos = x * v.x + y * v.y + z * v.z + w * v.w;//내적
		fCos = fCos / (Length() * v.Length());
		float fRadian = acos(fCos);
		float fDegree = RadianToDegree(fRadian);
		return fDegree;
	}
	TVector4  TVector4::operator* (TMatrix& m)
	{
		TVector4 v;
		v.x = x * m._11 + y * m._21 + z * m._31 + 1.0f * m._41;
		v.y = x * m._12 + y * m._22 + z * m._32 + 1.0f * m._42;
		v.z = x * m._13 + y * m._23 + z * m._33 + 1.0f * m._43;
		v.w = x * m._14 + y * m._24 + z * m._34 + 1.0f * m._44;
		return v;
	}

	TVector2::TVector2()
	{
		x = y = 0.0f;
	}
	TVector2::TVector2(float x, float y)
	{
		v[0] = x;
		v[1] = y;
	}
	TVector2::TVector2(const TVector2& v)

	{
		x = v.x;
		y = v.y;
	}
	TVector2 TVector2::operator + (TVector2& v)
	{
		return TVector2(x + v.x, y + v.y);
	}
	TVector2 TVector2::operator - (TVector2& v)
	{
		return TVector2(x - v.x, y - v.y);
	}
	TVector2 TVector2::operator * (float scala)
	{
		return TVector2(x * scala, y * scala);
	}
	TVector2 TVector2::operator / (float scala)
	{
		return TVector2(x / scala, y / scala);
	}
	TVector2 TVector2::operator *= (float scala)
	{
		x *= scala;
		y *= scala;
		return *this;
	}
	TVector2 TVector2::operator += (TVector2& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}
	TVector2 TVector2::operator -= (TVector2& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}
	bool      TVector2::operator == (TVector2& v)
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
	bool      TVector2::operator <= (TVector2& b)
	{
		if (x <= b.x)
		{
			if (y <= b.y)
			{
				return true;
			}
		}
		return false;
	}
	bool      TVector2::operator >= (TVector2& b)
	{
		if (x >= b.x)
		{
			if (y >= b.y)
			{
				return true;
			}
		}
		return false;
	}
	float TVector2::LengthSquared()
	{
		return (x * x + y * y);
	}

	float TVector2::Length()
	{
		return sqrt(LengthSquared());
	}
	// 정규화 -> 단위벡터(자신을) 계산
	void TVector2::Normalized()
	{
		float fInvertLength = 1.0f / Length();
		x = x * fInvertLength;
		y = y * fInvertLength;
	}
	// 단위벡터 반환
	TVector2 TVector2::Identity()
	{
		TVector2 ret = *this;
		float fInvertLength = 1.0f / Length();
		ret.x = ret.x * fInvertLength;
		ret.y = ret.y * fInvertLength;
		return ret;
	}
	float     TVector2::Angle(TVector2& v)
	{
		float fCos = x * v.x + y * v.y;//내적
		fCos = fCos / (Length() * v.Length());
		float fRadian = acos(fCos);
		float fDegree = RadianToDegree(fRadian);
		return fDegree;
	}


	///// 3d
	TVector3::TVector3()
	{
		x = y = z = 0.0f;
	}
	TVector3::TVector3(float x, float y, float z)
	{
		v[0] = x;
		v[1] = y;
		v[2] = z;
	}
	TVector3::TVector3(const TVector3& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}
	TVector3 TVector3::operator + (TVector3& v)
	{
		return TVector3(x + v.x, y + v.y, z + v.z);
	}
	TVector3 TVector3::operator - (TVector3& v)
	{
		return TVector3(x - v.x, y - v.y, z - v.z);
	}
	TVector3 TVector3::operator * (float scala)
	{
		return TVector3(x * scala, y * scala, z * scala);
	}
	TVector3 TVector3::operator / (float scala)
	{
		return TVector3(x / scala, y / scala, z / scala);
	}
	TVector3 TVector3::operator *= (float scala)
	{
		x *= scala;
		y *= scala;
		z *= scala;
		return *this;
	}
	TVector3 TVector3::operator += (TVector3& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	TVector3 TVector3::operator -= (TVector3& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}
	bool      TVector3::operator <= (TVector3& b)
	{
		if (fabs(x - b.x) <= T_Epsilon)
		{
			if (fabs(y - b.y) <= T_Epsilon)
			{
				if (fabs(z - b.z) <= T_Epsilon)
				{
					return true;
				}
			}
		}
		return false;
	}
	bool      TVector3::operator >= (TVector3& b)
	{
		if (fabs(x - b.x) >= T_Epsilon)
		{
			if (fabs(y - b.y) >= T_Epsilon)
			{
				if (fabs(z - b.z) >= T_Epsilon)
				{
					return true;
				}
			}
		}
		return false;
	}
	bool      TVector3::operator == (TVector3& v)
	{
		if (fabs(x - v.x) < T_Epsilon)
		{
			if (fabs(y - v.y) < T_Epsilon)
			{
				if (fabs(z - v.z) < T_Epsilon)
				{
					return true;
				}
			}
		}
		return false;
	}

	float TVector3::LengthSquared()
	{
		return (x * x + y * y + z * z);
	}

	float TVector3::Length()
	{
		return sqrt(LengthSquared());
	}
	// 정규화 -> 단위벡터(자신을) 계산
	void TVector3::Normalized()
	{
		float fInvertLength = 1.0f / Length();
		x = x * fInvertLength;
		y = y * fInvertLength;
		z = z * fInvertLength;
	}
	// 단위벡터 반환
	TVector3 TVector3::Normal()
	{
		TVector3 ret = *this;
		float fInvertLength = 1.0f / Length();
		ret.x = ret.x * fInvertLength;
		ret.y = ret.y * fInvertLength;
		ret.z = ret.z * fInvertLength;
		return ret;
	}
	float     TVector3::Angle(TVector3& v)
	{
		float fCos = x * v.x + y * v.y + z * v.z;//내적
		fCos = fCos / (Length() * v.Length());
		float fRadian = acos(fCos);
		float fDegree = RadianToDegree(fRadian);
		return fDegree;
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
	// TVector3 a =   a * m; [1x4] * [4x4]
	TVector3  TVector3::operator* (TMatrix& m)
	{
		TVector3 v;
		v.x = x * m._11 + y * m._21 + z * m._31 + 1.0f * m._41;
		v.y = x * m._12 + y * m._22 + z * m._32 + 1.0f * m._42;
		v.z = x * m._13 + y * m._23 + z * m._33 + 1.0f * m._43;
		return v;
	}
};