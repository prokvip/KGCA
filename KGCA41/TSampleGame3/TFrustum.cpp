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
	m_Plane[0].Create(  *((TVector*)&m_vFrustum[5]),
						*((TVector*)&m_vFrustum[0]),
						*((TVector*)&m_vFrustum[1])); // left
	m_Plane[1].Create(*((TVector*)&m_vFrustum[3]), // right
		*((TVector*)&m_vFrustum[6]),
		*((TVector*)&m_vFrustum[2])); 
	m_Plane[2].Create(*((TVector*)&m_vFrustum[5]), // top
		*((TVector*)&m_vFrustum[2]),
		*((TVector*)&m_vFrustum[6])); 
	m_Plane[3].Create(*((TVector*)&m_vFrustum[0]), // bottom
		*((TVector*)&m_vFrustum[7]),
		*((TVector*)&m_vFrustum[3])); 
	m_Plane[4].Create(*((TVector*)&m_vFrustum[0]), // near
		*((TVector*)&m_vFrustum[2]),
		*((TVector*)&m_vFrustum[1])); 
	m_Plane[5].Create(*((TVector*)&m_vFrustum[5]), // far
		*((TVector*)&m_vFrustum[6]),
		*((TVector*)&m_vFrustum[4])); 
}
bool	 TFrustum::ClassifyPoint(TVector v)
{
	for (int iPlane = 0; iPlane < 6; iPlane++)
	{
		float fDistance =
			m_Plane[iPlane].a * v.x +
			m_Plane[iPlane].b * v.y +
			m_Plane[iPlane].c * v.z +
			m_Plane[iPlane].d;
		if (fDistance < 0) return false;
	}
	return true;
}
bool	 TFrustum::ClassifySphere(TSphere v)
{	
	return true;
}
bool	 TFrustum::ClassifyAABB(T_AABB v)
{
	return true;
}
bool	 TFrustum::ClassifyOBB(T_OBB v)
{
	return true;
}
bool	 TFrustum::ClassifyTBox(T_BOX v)
{
	return true;
}