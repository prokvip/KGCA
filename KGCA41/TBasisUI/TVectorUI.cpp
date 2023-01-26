#include "TVectorUI.h"
namespace TUI
{
	TVector4D::TVector4D()
	{
		x = y = z = w = 0.0f;
	}
	TVector4D::TVector4D(float x, float y, float z, float w)
	{
		v[0] = x;
		v[1] = y;
		v[2] = z;
		v[3] = w;
	}
	TVector4D::TVector4D(const TVector4D& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
	}
	TVector4D TVector4D::operator + (TVector4D& v)
	{
		return TVector4D(x + v.x, y + v.y, z + v.z, w + v.w);
	}
	TVector4D TVector4D::operator - (TVector4D& v)
	{
		return TVector4D(x - v.x, y - v.y, z - v.z, w - v.w);
	}
	TVector4D TVector4D::operator * (float scala)
	{
		return TVector4D(x * scala, y * scala, z * scala, w * scala);
	}
	TVector4D TVector4D::operator / (float scala)
	{
		return TVector4D(x / scala, y / scala, z / scala, w / scala);
	}
	TVector4D TVector4D::operator *= (float scala)
	{
		x *= scala;
		y *= scala;
		z *= scala;
		w *= scala;
		return *this;
	}
	TVector4D TVector4D::operator += (TVector4D& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
		return *this;
	}
	TVector4D TVector4D::operator -= (TVector4D& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;
		return *this;
	}
	bool      TVector4D::operator == (TVector4D& v)
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
	bool      TVector4D::operator <= (TVector4D& b)
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
	bool      TVector4D::operator >= (TVector4D& b)
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
	float TVector4D::LengthSquared()
	{
		return (x * x + y * y, z * z + w * w);
	}

	float TVector4D::Length()
	{
		return sqrt(LengthSquared());
	}
	// 정규화 -> 단위벡터(자신을) 계산
	void TVector4D::Normalized()
	{
		float fInvertLength = 1.0f / Length();
		x = x * fInvertLength;
		y = y * fInvertLength;
		z = z * fInvertLength;
		w = w * fInvertLength;
	}
	// 단위벡터 반환
	TVector4D TVector4D::Identity()
	{
		TVector4D ret = *this;
		float fInvertLength = 1.0f / Length();
		ret.x = ret.x * fInvertLength;
		ret.y = ret.y * fInvertLength;
		ret.z = ret.z * fInvertLength;
		ret.w = ret.w * fInvertLength;
		return ret;
	}
	float     TVector4D::Angle(TVector4D& v)
	{
		float fCos = x * v.x + y * v.y + z * v.z + w * v.w;//내적
		fCos = fCos / (Length() * v.Length());
		float fRadian = acos(fCos);
		float fDegree = RadianToDegree(fRadian);
		return fDegree;
	}


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
		return TVector2D(x * scala, y * scala);
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
	bool      TVector2D::operator <= (TVector2D& b)
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
	bool      TVector2D::operator >= (TVector2D& b)
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
	float TVector2D::LengthSquared()
	{
		return (x * x + y * y);
	}

	float TVector2D::Length()
	{
		return sqrt(LengthSquared());
	}
	// 정규화 -> 단위벡터(자신을) 계산
	void TVector2D::Normalized()
	{
		float fInvertLength = 1.0f / Length();
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


	///// 3d
	TVector::TVector()
	{
		x = y = z = 0.0f;
	}
	TVector::TVector(float x, float y, float z)
	{
		v[0] = x;
		v[1] = y;
		v[2] = z;
	}
	TVector::TVector(const TVector& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}
	TVector TVector::operator + (TVector& v)
	{
		return TVector(x + v.x, y + v.y, z + v.z);
	}
	TVector TVector::operator - (TVector& v)
	{
		return TVector(x - v.x, y - v.y, z - v.z);
	}
	TVector TVector::operator * (float scala)
	{
		return TVector(x * scala, y * scala, z * scala);
	}
	TVector TVector::operator / (float scala)
	{
		return TVector(x / scala, y / scala, z / scala);
	}
	TVector TVector::operator *= (float scala)
	{
		x *= scala;
		y *= scala;
		z *= scala;
		return *this;
	}
	TVector TVector::operator += (TVector& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	TVector TVector::operator -= (TVector& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}
	bool      TVector::operator <= (TVector& b)
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
	bool      TVector::operator >= (TVector& b)
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
	bool      TVector::operator == (TVector& v)
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

	float TVector::LengthSquared()
	{
		return (x * x + y * y + z * z);
	}

	float TVector::Length()
	{
		return sqrt(LengthSquared());
	}
	// 정규화 -> 단위벡터(자신을) 계산
	void TVector::Normalized()
	{
		float fInvertLength = 1.0f / Length();
		x = x * fInvertLength;
		y = y * fInvertLength;
		z = z * fInvertLength;
	}
	// 단위벡터 반환
	TVector TVector::Identity()
	{
		TVector ret = *this;
		float fInvertLength = 1.0f / Length();
		ret.x = ret.x * fInvertLength;
		ret.y = ret.y * fInvertLength;
		ret.z = ret.z * fInvertLength;
		return ret;
	}
	float     TVector::Angle(TVector& v)
	{
		float fCos = x * v.x + y * v.y + z * v.z;//내적
		fCos = fCos / (Length() * v.Length());
		float fRadian = acos(fCos);
		float fDegree = RadianToDegree(fRadian);
		return fDegree;
	}

};