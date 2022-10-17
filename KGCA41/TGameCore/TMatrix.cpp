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

void TMatrix::Transpose()
{
	Identity();
	_11 = _11; _12 = _21; _13 = _31; _14 = _41;
	_21 = _12; _22 = _22; _23 = _32; _24 = _42;
	_31 = _13; _32 = _23; _33 = _33; _34 = _43;
	_41 = _14; _42 = _24; _43 = _34; _44 = _44;
}

void TMatrix::RotationX(float fRadian)
{
	Identity();
	float fCosTheta = cos(fRadian);
	float fSinTheta = sin(fRadian);
	_22 = fCosTheta; _23 = fSinTheta;
	_32 = -fSinTheta; _33 = fCosTheta;
}
void TMatrix::RotationY(float fRadian)
{
	Identity();
	float fCosTheta = cos(fRadian);
	float fSinTheta = sin(fRadian);
	_11 = fCosTheta; _13 = -fSinTheta;
	_31 = fSinTheta; _33 = fCosTheta;
}
void TMatrix::RotationZ(float fRadian)
{
	Identity();
	float fCosTheta = cos(fRadian);
	float fSinTheta = sin(fRadian);
	_11 = fCosTheta; _12 = fSinTheta;
	_21 = -fSinTheta; _22 = fCosTheta;	
}
void TMatrix::Scale(float x, float y, float z)
{
	Identity();
	_11 = x;
	_22 = y;
	_33 = z;
}
void TMatrix::Translation(float x, float y, float z)
{
	Identity();
	_41 = x;
	_42 = y;
	_43 = z;
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
				m[iRow][2] * matrix.m[2][iColumn] +
				m[iRow][3] * matrix.m[3][iColumn];
		}
	}
	return mat;
}
void TMatrix::ObjectLookAt(TVector& vPosition,
	TVector& vTarget,
	TVector& vUp)
{
	TVector vDirection = vTarget - vPosition;
	vDirection = vDirection.Normal();
	float fDot = vUp | vDirection;
	TVector vC = vDirection * fDot;
	TVector vD = (vDirection * fDot);
	TVector vUpVector = vUp - vD;
	vUpVector = vUpVector.Normal();
	TVector vRightVector = vUpVector ^ vDirection;

	_11 = vRightVector.x;	_12 = vRightVector.y;	_13 = vRightVector.z;
	_21 = vUpVector.x;		_22 = vUpVector.y;		_23 = vUpVector.z;
	_31 = vDirection.x;		_32 = vDirection.y;		_33 = vDirection.z;
	_41 = vPosition.x;
	_42 = vPosition.y;
	_43 = vPosition.z;
}
TMatrix TMatrix::ViewLookAt(TVector& vPosition,
	TVector& vTarget,
	TVector& vUp)
{
	TMatrix matrix;
	TVector vDirection = (vTarget - vPosition).Normal();
	TVector vRightVector = (vUp ^ vDirection).Normal();
	TVector vUpVector = (vDirection ^ vRightVector).Normal();

	_11 = vRightVector.x;	_12 = vUpVector.x;	_13 = vDirection.x;
	_21 = vRightVector.y;	_22 = vUpVector.y;	_23 = vDirection.y;
	_31 = vRightVector.z;	_32 = vUpVector.z;	_33 = vDirection.z;

	_41 = -(vPosition.x * _11 + vPosition.y * _21 + vPosition.z * _31);
	_42 = -(vPosition.x * _12 + vPosition.y * _22 + vPosition.z * _32);
	_43 = -(vPosition.x * _13 + vPosition.y * _23 + vPosition.z * _33);
	memcpy(&matrix, this, 16 * sizeof(float));
	return matrix;
}

TMatrix TMatrix::PerspectiveFovLH(float fNearPlane,
	float fFarPlane,
	float fovy,
	float Aspect) // width / heght
{
	float    h, w, Q;

	h = 1 / tan(fovy * 0.5f);  // 1/tans(x) = cot(x)
	w = h / Aspect;

	Q = fFarPlane / (fFarPlane - fNearPlane);

	TMatrix ret;
	ZeroMemory(this, sizeof(TMatrix));

	_11 = w;
	_22 = h;
	_33 = Q;
	_43 = -Q * fNearPlane;
	_34 = 1;

	memcpy((void*)&ret, this, 16 * sizeof(float));
	return ret;
}

TMatrix OrthoLH(float w, float h, float n, float f)
{
	TMatrix mat;
	return mat;
}
TMatrix OrthoOffCenterLH(float l, float r, float b, float t, float n, float f)
{
	TMatrix mat;
	return mat;
}
TMatrix TMatrix::OrthoLH(float w, float h, float n, float f)
{	
	Identity();
	_11 = 2.0f / w;
	_22 = 2.0f / h;
	_33 = 1.0f / (f - n);
	_43 = -n / (f - n);
	return *this;
}
TMatrix TMatrix::OrthoOffCenterLH(float l, float r, float b, float t, float n, float f)
{
	return* this;
}

