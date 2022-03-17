#pragma once
#include "TVector3.h"
struct float4x4
{
	union {
		struct {
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float m[4][4];
	};
};

class TMatrix : public float4x4
{
public:
	TMatrix operator * (TMatrix const& Matrix)
	{
		TMatrix OutMatrix;
		for (int iColumn = 0; iColumn < 4; iColumn++)
		{
			for (int iRow = 0; iRow < 4; iRow++)
			{
				OutMatrix.m[iRow][iColumn] = m[iRow][0] * Matrix.m[0][iColumn] +
					m[iRow][1] * Matrix.m[1][iColumn] +
					m[iRow][2] * Matrix.m[2][iColumn] +
					m[iRow][3] * Matrix.m[3][iColumn];
			}
		}
		return OutMatrix;
	}
	void    XRotate(float fRadian)
	{
		float fCos = cos(fRadian);
		float fSin = sin(fRadian);
		_22 = fCos;
		_23 = fSin;
		_32 = -fSin;
		_33 = fCos;
	}
	void    YRotate(float fRadian)
	{
		float fCos = cos(fRadian);
		float fSin = sin(fRadian);
		_11 = fCos;
		_13 = -fSin;
		_31 = fSin;
		_33 = fCos;
	}
	void    ZRotate(float fRadian)
	{
		float fCos = cos(fRadian);
		float fSin = sin(fRadian);
		_11 = fCos;
		_12 = fSin;
		_21 = -fSin;
		_22 = fCos;
	}
	void Translation(const TVector3& v)
	{
		_41 = v.x;
		_42 = v.y;
		_43 = v.z;
	}
	void Translation(float x, float y, float z)
	{
		_41 = x;
		_42 = y;
		_43 = z;
	}
	void Scale(const TVector3& vector)
	{
		_11 = vector.x;
		_22 = vector.y;
		_33 = vector.z;
	}
	void Scale(float x, float y, float z)
	{
		_11 = x;
		_22 = y;
		_33 = z;
	}
	void   Identity()
	{
		_11 = _12 = _13   = _14 =0.0f;
		_21 = _22 = _23 = _24 = 0.0f;
		_31 = _32 = _33 = _34 = 0.0f;
		_41 = _42 = _43 = _44 = 0.0f;
		_11 = _22 = _33 = _44 = 1.0f;
	}
	TMatrix Transpose()
	{
		TMatrix matrix;
		matrix._11 = _11; matrix._21 = _21; matrix._13 = _31; matrix._14 = _41;
		matrix._21 = _12; matrix._22 = _22; matrix._23 = _32; matrix._24 = _42;
		matrix._31 = _13; matrix._32 = _23; matrix._33 = _33; matrix._34 = _43;
		matrix._41 = _14; matrix._42 = _24; matrix._43 = _34; matrix._44 = _44;
		return matrix;
	}
public:
	TMatrix()
	{
		Identity();
	};
};

