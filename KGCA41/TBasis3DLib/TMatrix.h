#pragma once

#include "TVector.h"

struct float4x4
{
	union{
		struct {
			float _11,_12,_13,_14;
			float _21,_22,_23,_24;
			float _31,_32,_33,_34;
			float _41,_42,_43,_44;
		};
		float m[4][4];		
	};
};

class Matrix4x4 : public float4x4
{	
public:
	static  float g_identity[16];

public:
	Matrix4x4() 
	{
		Identity();
	};
	void	Identity()
	{
		_12 = _13 = _14 = 0.0f;
		_21 = _23 = _24 = 0.0f;
		_31 = _32 = _34 = 0.0f;
		_41 = _42 = _43 = 0.0f;
		_11 = _22 = _33 = _44 = 1.0f;
	}
	Matrix4x4( const Matrix4x4& matrix)
	{
		_11 = matrix._11; _12 = matrix._12; _13 = matrix._13; _14 = matrix._14;
		_21 = matrix._21; _22 = matrix._22; _23 = matrix._23; _24 = matrix._24;
		_31 = matrix._31; _32 = matrix._32; _33 = matrix._33; _34 = matrix._34;
		_41 = matrix._41; _42 = matrix._42; _43 = matrix._43; _44 = matrix._44;
	}

	Matrix4x4 operator * ( Matrix4x4 const &Matrix )
	{
		Matrix4x4 OutMatrix;
		for( int iColumn = 0; iColumn < 4; iColumn++ )
		{
			for( int iRow = 0; iRow < 4; iRow++ )
			{
				OutMatrix.m[iRow][iColumn] =	m[iRow][0] * Matrix.m[0][iColumn] + 
												m[iRow][1] * Matrix.m[1][iColumn] +
												m[iRow][2] * Matrix.m[2][iColumn] +
												m[iRow][3] * Matrix.m[3][iColumn];
			}
		}
		return OutMatrix;
	}
	Vector3 operator * ( Vector3 const &vector )
	{
		return Vector3(
			vector.x * _11 + vector.y*_21 + vector.z*_31 + _41,
			vector.x * _12 + vector.y*_22 + vector.z*_32 + _42,
			vector.x * _13 + vector.y*_23 + vector.z*_33 + _43	);
	}
	// access grants
	float& operator () ( UINT iRow, UINT iCol )
	{
		return m[iRow][iCol];
	}
	float operator () ( UINT iRow, UINT iCol ) const
	{
		return m[iRow][iCol];
	}
	// casting operators
	operator float* ()
	{
		return (float *) &_11;
	}
	operator const float* () const
	{
		return (const float *) &_11;
	}
	void Translation( const Vector3 &vector )
	{
		_41 = vector.x;
		_42 = vector.y;
		_43 = vector.z;
	}
	void Translation( float x, float y, float z )
	{
		_41 = x;
		_42 = y;
		_43 = z; 
	}
	void Scale( const Vector3 &vector )
	{
		_11 = vector.x;
		_22 = vector.y;
		_33 = vector.z; 
	}
	void Scale( float x, float y, float z )
	{
		_11 = x;
		_22 = y;
		_33 = z; 
	}
	void XRotate( float fRadian )
	{
		float fCosTheta = (float)cos(fRadian);
		float fSinTheta = (float)sin(fRadian);
		_22 = fCosTheta;
		_23 = fSinTheta;
		_32 = -fSinTheta;
		_33 = fCosTheta;
	}
	void YRotate( float fRadian )
	{
		float fCosTheta = (float)cos(fRadian);
		float fSinTheta = (float)sin(fRadian);
		_11 = fCosTheta;
		_13 = -fSinTheta;
		_31 = fSinTheta;
		_33 = fCosTheta;
	}
	void ZRotate( float fRadian )
	{
		float fCosTheta = (float)cos(fRadian);
		float fSinTheta = (float)sin(fRadian);
		_11 = fCosTheta;
		_12 = fSinTheta;
		_21 = -fSinTheta;
		_22 = fCosTheta;
	}
	//        ^
	//        \
	//        |  \
	//        |    ---------------->
	// 내적의 결과는 직각이 된다.
	void ObjectLookAt(	 Vector3& vPosition,
						 Vector3& vTarget,
						 Vector3& vUp )
	{
		Vector3 vDirection  = vTarget - vPosition;
		vDirection = vDirection.Normal();
		float fDot = vUp | vDirection;
		Vector3 vC = vDirection * fDot;
		Vector3 vUpVector = vUp - ( vDirection * fDot );
		vUpVector = vUpVector.Normal();
		Vector3 vRightVector = vUpVector ^ vDirection;
		
		_11 = vRightVector.x;	_12 = vRightVector.y;	_13 = vRightVector.z; 
		_21 = vUpVector.x;		_22 = vUpVector.y;		_23 = vUpVector.z;
		_31 = vDirection.x;		_32 = vDirection.y;		_33 = vDirection.z;
		_41 = vPosition.x;
		_42 = vPosition.y;
		_43 = vPosition.z;		
	}
	Matrix4x4 ViewLookAt(	 Vector3& vPosition,
						 Vector3& vTarget,
						 Vector3& vUp )
	{
		Matrix4x4 matrix;
		Vector3 vDirection  = vTarget - vPosition;
		vDirection = vDirection.Normal();
		float fDot = vUp | vDirection;
		Vector3 vC = vDirection * fDot;
		Vector3 vUpVector = vUp - ( vDirection * fDot );
		vUpVector = vUpVector.Normal();
		Vector3 vRightVector = vUpVector ^ vDirection;
		
		_11 = vRightVector.x;	_12 = vUpVector.x;		_13 = vDirection.x; 
		_21 = vRightVector.y;	_22 = vUpVector.y;		_23 = vDirection.y;
		_31 = vRightVector.z;	_32 = vUpVector.z;		_33 = vDirection.z;		
		
		_41 = -( vPosition.x*_11 + vPosition.y*_21 + vPosition.z*_31 );
		_42 = -( vPosition.x*_12 + vPosition.y*_22 + vPosition.z*_32 );
		_43 = -( vPosition.x*_13 + vPosition.y*_23 + vPosition.z*_33 );

		memcpy( matrix, this, 16*sizeof(float));
		return matrix;
	}
	// 전치 행렬 계산
	Matrix4x4 Transpose()
	{
		Matrix4x4 matrix;
		matrix._11 = _11; matrix._12 = _21;matrix._13 = _31;matrix._14 = _41;
		matrix._21 = _12; matrix._22 = _22;matrix._23 = _32;matrix._24 = _42;
		matrix._31 = _13; matrix._32 = _23;matrix._33 = _33;matrix._34 = _43;
		matrix._41 = _14; matrix._42 = _24;matrix._43 = _34;matrix._44 = _44;
		return matrix;
	}
	// 역행렬 계산
	void	InverseGaussJordan( float* m, float *out );
	Matrix4x4	InverseGeneral( float *out = NULL );
	Matrix4x4	Inverse( float *out = NULL );

	// 뷰 행렬 계산
	Matrix4x4 ComputeViewMatrix(	Vector3& vPosition,
								Vector3& vTarget,
								Vector3& vUp)
	{
		ObjectLookAt( vPosition, vTarget, vUp );
		return Inverse();
	}

	// 원근 투영 행렬 계산
	Matrix4x4 PerspectiveLH(	float fNearPlane,
							float fFarPlane,
							float fWidth,
							float fHeight )
	{
		float    h, w, Q;

		w = ( 2.0f*fNearPlane ) / ((float)fWidth);
		h = ( 2.0f*fNearPlane ) / ((float)fHeight);
		Q = fFarPlane/(fFarPlane - fNearPlane);

		Matrix4x4 ret;
		ZeroMemory( this, sizeof(Matrix4x4));

		_11 = w;
		_22 = h;
		_33 = Q;
		_43 = fNearPlane*fFarPlane/(fNearPlane-fFarPlane);
		//_43 = -Q*fNearPlane;
		_34 = 1;

		memcpy( ret, this, 16*sizeof(float));
		return ret;
	}
	// 원근 투영 행렬 계산
	Matrix4x4 PerspectiveFovLH(	float fNearPlane,
								float fFarPlane,
								float fovy,
								float Aspect) // width / heght
	{
		float    h, w, Q;

		h = 1/tan(fovy*0.5f);  
		w = h/Aspect;  
		
		Q = fFarPlane/(fFarPlane - fNearPlane);

		Matrix4x4 ret;
		ZeroMemory( this, sizeof(Matrix4x4));

		_11 = w;
		_22 = h;
		_33 = Q;
		_43 = -Q*fNearPlane;
		_34 = 1;

		memcpy( ret, this, 16*sizeof(float));
		return ret;
	}

	void CreateAxisAngle( Vector3& vAxis, float fRadian )
	{
		Vector3 vAxisVector = vAxis.Normal(); 
		float fCosTheta = (float)cos(fRadian);
		float fSinTheta = (float)sin(fRadian);
		_11 = vAxis.x * vAxis.x*( 1- fCosTheta ) + fCosTheta;
		_21 = vAxis.x * vAxis.y*( 1- fCosTheta ) - ( vAxis.z*fSinTheta);
		_31 = vAxis.x * vAxis.z*( 1- fCosTheta ) + ( vAxis.y*fSinTheta);

		_12 = vAxis.y * vAxis.x*( 1- fCosTheta ) + ( vAxis.z*fSinTheta);
		_22 = vAxis.y * vAxis.y*( 1- fCosTheta ) + fCosTheta;
		_32 = vAxis.y * vAxis.z*( 1- fCosTheta ) - ( vAxis.x*fSinTheta);

		_13 = vAxis.z * vAxis.x*( 1- fCosTheta ) - ( vAxis.y*fSinTheta);
		_23 = vAxis.z * vAxis.y*( 1- fCosTheta ) + ( vAxis.x*fSinTheta);
		_33 = vAxis.z * vAxis.z*( 1- fCosTheta ) + fCosTheta;

		_41 = _42 = _43 = _14 = _24 = _34 = 0.0f;
		_44 = 1.0f;		
	}
	Matrix4x4 AxisAngle( Vector3& vAxis, float fRadian )
	{
		Matrix4x4 matrix;
		matrix.CreateAxisAngle( vAxis, fRadian );
		return matrix;
	}

};