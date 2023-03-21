#include "pch.h"
#include "TFrustum.h"
void	 TFrustum::CreateFrustum(
	TMatrix* matView, TMatrix* matProj)
{
	TBASIS_EX::TMatrix view = *(TBASIS_EX::TMatrix*)matView;
	TBASIS_EX::TMatrix proj = *(TBASIS_EX::TMatrix*)matProj;

	TBASIS_EX::TMatrix matInvViewProj = view * proj;
	TBASIS_EX::D3DXMatrixInverse(&matInvViewProj, NULL, &matInvViewProj);

	// m_vFrustum = v * world * (view * proj);
	// m_vFrustum = v * world * view*invView * proj * invproj;
	// m_vFrustum /= v.w     x,y= -1~+1,   z= 0~1;
	m_vFrustum[0] = TBASIS_EX::TVector3(-1.0f, -1.0f, 0.0f);
	m_vFrustum[1] = TBASIS_EX::TVector3(-1.0f,  1.0f, 0.0f);
	m_vFrustum[2] = TBASIS_EX::TVector3( 1.0f,  1.0f, 0.0f);
	m_vFrustum[3] = TBASIS_EX::TVector3( 1.0f, -1.0f, 0.0f);
	m_vFrustum[4] = TBASIS_EX::TVector3(-1.0f, -1.0f, 1.0f);
	m_vFrustum[5] = TBASIS_EX::TVector3(-1.0f, 1.0f, 1.0f);
	m_vFrustum[6] = TBASIS_EX::TVector3(1.0f, 1.0f, 1.0f);
	m_vFrustum[7] = TBASIS_EX::TVector3(1.0f, -1.0f, 1.0f);

	for (int iVer = 0; iVer < 8; iVer++)
	{
		// m_vFrustum[iVer] = m_vFrustum[iVer] * matInvViewProj;
		TBASIS_EX::D3DXVec3TransformCoord(
			&m_vFrustum[iVer], &m_vFrustum[iVer], &matInvViewProj);		
	}
	// 5    6  
	// 4    7
	// ->   <-
	// 1    2
	// 0    3 
	m_Plane[0].Create(  *((TVector3*)&m_vFrustum[1]),
						*((TVector3*)&m_vFrustum[5]),
						*((TVector3*)&m_vFrustum[0])); // left
	m_Plane[1].Create(*((TVector3*)&m_vFrustum[3]), // right
		*((TVector3*)&m_vFrustum[6]),
		*((TVector3*)&m_vFrustum[2])); 
	m_Plane[2].Create(*((TVector3*)&m_vFrustum[5]), // top
		*((TVector3*)&m_vFrustum[2]),
		*((TVector3*)&m_vFrustum[6])); 
	m_Plane[3].Create(*((TVector3*)&m_vFrustum[0]), // bottom
		*((TVector3*)&m_vFrustum[7]),
		*((TVector3*)&m_vFrustum[3])); 
	m_Plane[4].Create(*((TVector3*)&m_vFrustum[2]), // near
		*((TVector3*)&m_vFrustum[1]),
		*((TVector3*)&m_vFrustum[0])); 
	m_Plane[5].Create(*((TVector3*)&m_vFrustum[5]), // far
		*((TVector3*)&m_vFrustum[6]),
		*((TVector3*)&m_vFrustum[4])); 
}
T_POSITION	 TFrustum::ClassifyPoint(TVector3 v)
{
	for (int iPlane = 0; iPlane < 6; iPlane++)
	{
		float fDistance =
			m_Plane[iPlane].a * v.x +
			m_Plane[iPlane].b * v.y +
			m_Plane[iPlane].c * v.z +
			m_Plane[iPlane].d;
		if (fDistance == 0) return P_ONPLANE;
		if (fDistance < 0) return P_FRONT;
	}
	return P_BACK;
}
T_POSITION	 TFrustum::ClassifySphere(TSphere v)
{	
	return P_SPANNING;
}
T_POSITION	 TFrustum::ClassifyAABB(T_AABB v)
{
	return P_SPANNING;
}
T_POSITION	 TFrustum::ClassifyOBB(T_OBB v)
{
	return P_SPANNING;
}
T_POSITION	 TFrustum::ClassifyTBox(T_BOX box)
{
	float		fPlaneToCenter = 0.0;
	float		fDistance = 0.0f;
	TVector3 vDir;
	T_POSITION  t_Position;

	t_Position = P_FRONT;
	for (int iPlane = 0; iPlane < 6; iPlane++)
	{
		vDir = box.vAxis[0] * box.fExtent[0];
		fDistance = fabs(m_Plane[iPlane].a * vDir.x + m_Plane[iPlane].b * vDir.y + m_Plane[iPlane].c * vDir.z);
		vDir = box.vAxis[1] * box.fExtent[1];
		fDistance += fabs(m_Plane[iPlane].a * vDir.x + m_Plane[iPlane].b * vDir.y + m_Plane[iPlane].c * vDir.z);
		vDir = box.vAxis[2] * box.fExtent[2];
		fDistance += fabs(m_Plane[iPlane].a * vDir.x + m_Plane[iPlane].b * vDir.y + m_Plane[iPlane].c * vDir.z);

		fPlaneToCenter = m_Plane[iPlane].a * box.vCenter.x + m_Plane[iPlane].b * box.vCenter.y +
			m_Plane[iPlane].c * box.vCenter.z + m_Plane[iPlane].d;

		if (fPlaneToCenter > 0)
		{
			if (fPlaneToCenter < fDistance)
			{
				t_Position = P_SPANNING;
				break;
			}
		}
		else
		if(fPlaneToCenter < 0)
		{
			t_Position = P_BACK;
			if (fPlaneToCenter > -fDistance)
			{
				t_Position = P_SPANNING;					
			}
			break;
		}
	}
	return t_Position;
}