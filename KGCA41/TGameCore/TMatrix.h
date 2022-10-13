#pragma once
#include "TVector.h"
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
	TMatrix Transpose();
	TMatrix RotationX(float fRadian);
	TMatrix RotationY(float fRadian);
	TMatrix RotationZ(float fRadian);
	TMatrix Scale(float x, float y, float z);
	TMatrix Translation(float x, float y, float z);
public:
	TMatrix operator* (TMatrix& m);
};

