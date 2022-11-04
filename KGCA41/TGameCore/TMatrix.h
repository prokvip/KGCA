#pragma once
#include "TVector.h"
namespace T_TEST
{
	struct float3x3
{
	union
	{
		struct {
			float _11, _12, _13;
			float _21, _22, _23;
			float _31, _32, _33;
		};
		float m[3][3];
	};
};
struct float4x4
{
	union
	{
		struct {
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float m[4][4];
	};
};
class TMatrix3x3 : public float3x3
{
public:
	TMatrix3x3();
	void Identity();
	TMatrix3x3 Transpose();
	TMatrix3x3 Rotation(float fRadian);
	TMatrix3x3 Scale(float x, float y);
	TMatrix3x3 Translation(float x, float y);
public:
	TMatrix3x3 operator* (TMatrix3x3& m);
};

class TMatrix : public float4x4
{
public:
	TMatrix();
	void Identity();
	void RotationX(float fRadian);
	void RotationY(float fRadian);
	void RotationZ(float fRadian);
	void Scale(float x, float y, float z);
	void Translation(float x, float y, float z);
	TMatrix Transpose();

	void ObjectLookAt(TVector3& vPosition,TVector3& vTarget,TVector3& vUp);
	TMatrix ViewLookAt(TVector3& vPosition,TVector3& vTarget,
						TVector3& vUp);
	TMatrix PerspectiveFovLH(float fNearPlane,
								float fFarPlane,
								float fovy,
								float Aspect);
	TMatrix OrthoLH(float w, float h, float n, float f);
	TMatrix OrthoOffCenterLH(float l, float r, float b, float t, float n, float f);
public:
	friend TMatrix OrthoLH(TMatrix& mat, float w, float h, float n, float f);
	friend TMatrix OrthoOffCenterLH(TMatrix& mat, float l, float r, float b, float t, float n, float f);
	friend TMatrix PerspectiveFovLH(TMatrix& mat, float fNearPlane,float fFarPlane,float fovy,float Aspect);
public:
	TMatrix operator* (TMatrix& m);
};


namespace TMath
{
	static TMatrix RotationX(float fRadian)
	{
		float fCosTheta = cos(fRadian);
		float fSinTheta = sin(fRadian);
		TMatrix m;
		m._22 = fCosTheta; m._23 = fSinTheta;
		m._32 = -fSinTheta; m._33 = fCosTheta;
		return m;
	}
	static TMatrix RotationY(float fRadian)
	{
		float fCosTheta = cos(fRadian);
		float fSinTheta = sin(fRadian);
		TMatrix m;
		m._11 = fCosTheta; m._13 = -fSinTheta;
		m._31 = fSinTheta; m._33 = fCosTheta;
		return m;
	}
	static TMatrix RotationZ(float fRadian)
	{
		float fCosTheta = cos(fRadian);
		float fSinTheta = sin(fRadian);
		TMatrix m;
		m._11 = fCosTheta; m._12 = fSinTheta;
		m._21 = -fSinTheta; m._22 = fCosTheta;
		return m;
	}
	static TMatrix Scale(float x, float y, float z)
	{
		TMatrix m;
		m._11 = x;
		m._22 = y;
		m._33 = z;
		return m;
	}
	static TMatrix Translation(float x, float y, float z)
	{
		TMatrix m;
		m._41 = x;
		m._42 = y;
		m._43 = z;
		return m;
	}
};
};