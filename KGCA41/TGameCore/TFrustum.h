#pragma once
#include "TShape.h"
enum T_POSITION
{
	P_BACK = 0,
	P_FRONT,
	P_ONPLANE,
	P_SPANNING,
};
struct tPlane
{
	float a, b, c, d;
	void  Create(TVector3 v0, TVector3 v1, TVector3 v2)
	{
		TVector3 vE0 = v1 - v0;
		TVector3 vE1 = v2 - v0;
		TVector3 vNormal;// = vE0 ^ vE1;
		D3DXVec3Cross(&vNormal, &vE0, &vE1);
		D3DXVec3Normalize(&vNormal, &vNormal);
		a = vNormal.x;
		b = vNormal.y;
		c = vNormal.z;
		//d = -(vNormal | v0);
		d = -D3DXVec3Dot(&vNormal, &v0);
		// p = n.x*v.x + n.y*v.y + v.z*v.z + d=0;
		// p = n.x*v.x + n.y*v.y + v.z*v.z = -d;
		// p = (n.x*v.x + n.y*v.y + v.z*v.z)*-1.0f = d;
		// p = -(n dot v) = d;
	}
	void  Create(TVector3  normal, TVector3  v0)
	{
		D3DXVec3Normalize(&normal, &normal);		
		a = normal.x;
		b = normal.y;
		c = normal.z;
		d = -D3DXVec3Dot(&normal, &v0);
	}
};
class TFrustum
{
	TMatrix   m_matView;
	TMatrix   m_matProj;
public:
	TBASIS_EX::TVector3  m_vFrustum[8];
	tPlane   m_Plane[6];
public:
	void	 CreateFrustum(TMatrix*, TMatrix*);
	T_POSITION	 ClassifyPoint(TVector3 v);
	T_POSITION	 ClassifySphere(TSphere v);
	T_POSITION	 ClassifyAABB(T_AABB v);
	T_POSITION	 ClassifyOBB(T_OBB v);
	T_POSITION	 ClassifyTBox(T_BOX v);
};

