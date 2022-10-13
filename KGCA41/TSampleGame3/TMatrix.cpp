#include "TMatrix.h"
TMatrix3x3::TMatrix3x3()
{
	Identity();
}

void TMatrix3x3::Identity()
{
	_12 = _13 = 0.0f;
	_21 = _23 = 0.0f;
	_31 = _32 = 0.0f;
	_11 = _22 = _33 = 1.0f;
}

TMatrix3x3 TMatrix3x3::Transpose()
{
	TMatrix3x3 m;
	m._11 = _11; m._12 = _21; m._13 = _31;
	m._21 = _12; m._22 = _22; m._23 = _32;
	m._31 = _13; m._32 = _23; m._33 = _33;
	return m;
}
TMatrix3x3 TMatrix3x3::Rotation(float fRadian)
{
	float fCosTheta = cos(fRadian);
	float fSinTheta = sin(fRadian);
	TMatrix3x3 m;
	m._11 = fCosTheta; m._12 = fSinTheta;
	m._21 = -fSinTheta; m._22 = fCosTheta;
	return m;
}
TMatrix3x3 TMatrix3x3::Scale(float x, float y)
{
	TMatrix3x3 m;
	m._11 = x;
	m._22 = y;
	return m;
}
TMatrix3x3 TMatrix3x3::Translation(float x, float y)
{
	TMatrix3x3 m;
	m._31 = x;
	m._32 = y;
	return m;
}
TMatrix3x3  TMatrix3x3::operator* (TMatrix3x3& matrix)
{
	TMatrix3x3 mat;
	for ( int iColumn =0; iColumn < 3; iColumn++)
	{
		for (int iRow = 0; iRow < 3; iRow++)
		{
			mat.m[iRow][iColumn] = 
				m[iRow][0] * matrix.m[0][iColumn] +
				m[iRow][1] * matrix.m[1][iColumn] +
				m[iRow][2] * matrix.m[2][iColumn];
		}
	}
	return mat;
}


TMatrix::TMatrix()
{
	Identity();
}
void TMatrix::Identity()
{
	_12 = _13 = _14 = 0.0f;
	_21 = _23 = _24 = 0.0f;
	_31 = _32 = _34 = 0.0f;
	_41 = _42 = _43 = 0.0f;
	_11 = _22 = _33 = _44 = 1.0f;
}
TMatrix TMatrix::Transpose()
{
	TMatrix m;
	m._11 = _11; m._12 = _21; m._13 = _31; m._14 = _41;
	m._21 = _12; m._22 = _22; m._23 = _32; m._24 = _42;
	m._31 = _13; m._32 = _23; m._33 = _33; m._34 = _43;
	m._41 = _14; m._42 = _24; m._43 = _34; m._44 = _44;
	return m;
}
TMatrix TMatrix::RotationX(float fRadian)
{
	float fCosTheta = cos(fRadian);
	float fSinTheta = sin(fRadian);
	TMatrix m;
	m._22 = fCosTheta; m._23 = fSinTheta;
	m._32 = -fSinTheta; m._33 = fCosTheta;
	return m;
}
TMatrix TMatrix::RotationY(float fRadian)
{
	float fCosTheta = cos(fRadian);
	float fSinTheta = sin(fRadian);
	TMatrix m;
	m._11 = fCosTheta; m._13 = -fSinTheta;
	m._31 = fSinTheta; m._33 = fCosTheta;
	return m;
}
TMatrix TMatrix::RotationZ(float fRadian)
{
	float fCosTheta = cos(fRadian);
	float fSinTheta = sin(fRadian);
	TMatrix m;
	m._11 = fCosTheta; m._12 = fSinTheta;
	m._21 = -fSinTheta; m._22 = fCosTheta;
	return m;
}
TMatrix TMatrix::Scale(float x, float y, float z)
{
	TMatrix m;
	m._11 = x;
	m._22 = y;
	m._33 = z;
	return m;
}
TMatrix TMatrix::Translation(float x, float y, float z)
{
	TMatrix m;
	m._41 = x;
	m._42 = y;
	m._43 = z;
	return m;
}
TMatrix  TMatrix::operator* (TMatrix& matrix)
{
	TMatrix mat;
	for (int iColumn = 0; iColumn < 4; iColumn++)
	{
		for (int iRow = 0; iRow < 4; iRow++)
		{
			mat.m[iRow][iColumn] =
				m[iRow][0] * matrix.m[0][iColumn] +
				m[iRow][1] * matrix.m[1][iColumn] +
				m[iRow][2] * matrix.m[2][iColumn];
		}
	}
	return mat;
}