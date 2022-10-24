#pragma once
#include "TShape.h"
struct TPlane
{
	float a, b, c, d;
	void  Create(TVector v0, TVector v1, TVector v2)
	{
		TVector vE0 = v1 - v0;
		TVector vE1 = v2 - v0;
		TVector vNormal = vE0 ^ vE1;
		vNormal.Normalized();
		a = vNormal.x;
		b = vNormal.y;
		c = vNormal.z;
		d = -(vNormal | v0);
		// p = n.x*v.x + n.y*v.y + v.z*v.z + d=0;
		// p = n.x*v.x + n.y*v.y + v.z*v.z = -d;
		// p = (n.x*v.x + n.y*v.y + v.z*v.z)*-1.0f = d;
		// p = -(n dot v) = d;
	}
	void  Create(TVector  normal, TVector  v0)
	{
		normal.Normalized();
		a = normal.x;
		b = normal.y;
		c = normal.z;
		d = -(normal | v0);
	}
};
class TFrustum
{
	TMatrix   m_matView;
	TMatrix   m_matProj;
public:
	TBASIS_EX::TVector3  m_vFrustum[8];
	TPlane   m_Plane[6];
public:
	void	 CreateFrustum(TMatrix*, TMatrix*);
	bool	 ClassifyPoint(TVector v);
};

