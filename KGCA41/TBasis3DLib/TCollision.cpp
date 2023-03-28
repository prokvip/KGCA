#include "TCollision.h"
#include ".\tcollision.h"

BOOL TCollision::ChkBoxToRay(T_BOX& Box, T_RAY& Ray)
{
	float fWdU[3], fAWdU[3], fDdU[3], fADdU[3], fAWxDdU[3], fRhs;

	TVector3 vReverseDir = Ray.vOrigin - Box.vCenter;

	fWdU[0] = D3DXVec3Dot(&Ray.vDirection, &Box.vAxis[0]);
	fAWdU[0] = fabs(fWdU[0]);
	fDdU[0] = D3DXVec3Dot(&vReverseDir, &Box.vAxis[0]);
	fADdU[0] = fabs(fDdU[0]);
	if (fADdU[0] > Box.fExtent[0] && fDdU[0] * fWdU[0] >= 0.0f)
		return false;

	fWdU[1] = D3DXVec3Dot(&Ray.vDirection, &Box.vAxis[1]);
	fAWdU[1] = fabs(fWdU[1]);
	fDdU[1] = D3DXVec3Dot(&vReverseDir, &Box.vAxis[1]);
	fADdU[1] = fabs(fDdU[1]);
	if (fADdU[1] > Box.fExtent[1] && fDdU[1] * fWdU[1] >= 0.0f)
		return false;

	fWdU[2] = D3DXVec3Dot(&Ray.vDirection, &Box.vAxis[2]);
	fAWdU[2] = fabs(fWdU[2]);
	fDdU[2] = D3DXVec3Dot(&vReverseDir, &Box.vAxis[2]);
	fADdU[2] = fabs(fDdU[2]);
	if (fADdU[2] > Box.fExtent[2] && fDdU[2] * fWdU[2] >= 0.0f)
		return false;


	TVector3 kWxD;
	D3DXVec3Cross(&kWxD, &Ray.vDirection, &vReverseDir);


	fAWxDdU[0] = fabs(D3DXVec3Dot(&kWxD, &Box.vAxis[0]));
	fRhs = Box.fExtent[1] * fAWdU[2] + Box.fExtent[2] * fAWdU[1];
	if (fAWxDdU[0] > fRhs)
		return false;
	fAWxDdU[1] = fabs(D3DXVec3Dot(&kWxD, &Box.vAxis[1]));
	fRhs = Box.fExtent[0] * fAWdU[2] + Box.fExtent[2] * fAWdU[0];
	if (fAWxDdU[1] > fRhs)
		return false;
	fAWxDdU[2] = fabs(D3DXVec3Dot(&kWxD, &Box.vAxis[2]));
	fRhs = Box.fExtent[0] * fAWdU[1] + Box.fExtent[1] * fAWdU[0];
	if (fAWxDdU[2] > fRhs)
		return false;

	return true;
}

BOOL TCollision::ChkBoxToRay(T_BOX& Box, TVector3 vOrigin, TVector3 vDirection)
{
	return TRUE;
}
BOOL TCollision::ChkBoxToRay(TVector3* vBox, TVector3 vOrigin, TVector3 vDirection)
{
	return TRUE;
}
BOOL TCollision::ChkBoxToRay(TVector3& vMax, TVector3& vMin, TVector3 vOrigin, TVector3 vDirection)
{
	return TRUE;
}

bool TCollision::Init()
{
	return true;
}
bool TCollision::Render()
{
	//I_Engine.m_Debug.DrawLine(m_vPointA, m_vPointB, 0x00ffffff); // Flat Ray
	//I_Engine.m_Debug.DrawLine(m_vPointC, m_vPointD, 0x00ffffff); // Flat Ray
	//I_Engine.m_Debug.DrawLine(m_vPointE, m_vPointF, 0x00ffffff); // Flat Ray

	//I_Engine.m_Debug.DrawLine(m_vCPointA, m_vCPointB, 0x00ffffff); // Flat Ray
	//I_Engine.m_Debug.DrawLine(m_vCPointC, m_vCPointD, 0x00ffffff); // Flat Ray
	//I_Engine.m_Debug.DrawLine(m_vCPointE, m_vCPointF, 0x00ffffff); // Flat Ray

	//I_Engine.m_Debug.DrawTriangle(&m_vCollisionTriangle[0],
	//	&m_vCollisionTriangle[1],
	//	&m_vCollisionTriangle[2], 0xffff00ff); // Collision Face


	// 충돌데이터에 대한 비주얼적인 검증용도의 디버그 랜더링
	//CollisionPolyRender();
	return true;
}
void TCollision::SetUserPosition(TVector3 vPos)
{
	m_vPosition = vPos;
}
void TCollision::SetCollisionPoly(TPolygon& pPolygon)
{
	m_pCollisionPoly = pPolygon;
}
bool TCollision::GetIntersection(TVector3& vStart, TVector3& vEnd, TVector3& vPlaneVertex,
	TVector3& vPlaneNormal,
	TVector3& vIntersection,
	float& fPercentage)
{
	TVector3 vDirection;
	float		fDistance;

	vDirection = vEnd - vStart;
	fDistance = D3DXVec3Dot(&vDirection, &vPlaneNormal);
	if (fabsf(fDistance) < 0.001f)
		return false;

	fPercentage = D3DXVec3Dot(&(vPlaneVertex - vStart),
		&vPlaneNormal) / fDistance;
	if (fPercentage < 0.0f || fPercentage >1.0f)
		return false;

	vIntersection = vStart + vDirection * fPercentage;
	return true;
}
bool  TCollision::CollisionCheck(TVector3& vStart, TVector3& vEnd,
	TVector3* v0, TVector3* v1, TVector3* v2,
	TVector3& vPlanNormal,
	TVector3& vDirVector,
	TVector3& vIntersetion,
	float& fDistance,
	float fUnit)
{
	TVector3* Inter;
	float fPercentage = 0.0f;

	if (GetIntersection(vStart, vEnd, *v0,
		vPlanNormal, vIntersetion, fPercentage))
	{
		float fPer = 1.0f;
		fPer -= fPercentage;
		TVector3 vDir = vEnd - vStart;
		//D3DXVec3Normalize( &vDir, &vDir );
		vDir *= -1.0f;

		if (PointInPolygon(&vIntersetion, &vPlanNormal,
			v0, v1, v2))
		{
			vDirVector = vDir;
			fDistance = fPer + fUnit;
			m_vCollisionTriangle[0] = *v0;
			m_vCollisionTriangle[1] = *v1;
			m_vCollisionTriangle[2] = *v2;
			return true;
		}
	}
	return false;
}
bool TCollision::Frame()
{
	m_vCameraIntersection = TVector3(0.0f, 50000.0f, 0.0f);

	//   
	m_vPointA = TVector3(m_vBackPosition.x, m_vBackPosition.y, m_vBackPosition.z);
	m_vPointB = TVector3(m_vPosition.x, m_vPosition.y, m_vPosition.z);
	m_vPointA += TVector3(0.0f, 1.0f, 0.0f) * 2.0f;
	m_vPointB += TVector3(0.0f, -1.0f, 0.0f) * 2.0f;
	// X-               X+
	// C--------------->D  ( 좌 : 우 )
	m_vPointC = TVector3(m_vBackPosition.x, m_vBackPosition.y + 2.0f, m_vBackPosition.z);
	m_vPointD = TVector3(m_vPosition.x, m_vBackPosition.y + 2.0f, m_vPosition.z);
	m_vPointC += TVector3(-1.0f, 0.0f, 0.0f) * 2.0f;
	m_vPointD += TVector3(1.0f, 0.0f, 0.0f) * 2.0f;

	// Z+               Z-
	// E--------------->F ( 앞 : 뒤 )
	m_vPointE = TVector3(m_vBackPosition.x, m_vBackPosition.y + 2.0f, m_vBackPosition.z);
	m_vPointF = TVector3(m_vPosition.x, m_vBackPosition.y + 2.0f, m_vPosition.z);
	m_vPointE += TVector3(0.0f, 0.0f, 1.0f) * 2.0f;
	m_vPointF += TVector3(0.0f, 0.0f, -1.0f) * 2.0f;

	m_bCollision = false;
	TVector3  vDirVector;
	TVector3  vIntersection;
	TVector3  vNormal;

	for( auto& pPoly : m_pCollisionPoly)	
	{		
		float fDistance = 0.0f;

		TVector3* vec0 = (TVector3*)&pPoly.vVertex[0];
		TVector3* vec1 = (TVector3*)&pPoly.vVertex[1];
		TVector3* vec2 = (TVector3*)&pPoly.vVertex[2];

		D3DXVec3Cross(&vNormal, &(*vec1 - *vec0), &(*vec2 - *vec0));
		D3DXVec3Normalize(&vNormal, &vNormal);

		// 좌우  // C<-------|---=D
		if (vNormal.y < 0.5f && vNormal.x >= 0.5f)
		{
			if (CollisionCheck(m_vPointD, m_vPointC,
				vec0, vec1, vec2, vNormal, vDirVector,
				vIntersection, fDistance))
			{
				// 이전포지션에서 진행방향으로 fDistatnce만큼 이동하는 것이 아니라
				// 충돌이전의 포지션에서 레이의 반대방향으로 fDistance만큼 이동한다.
				// 결국 교점에서 들어간 이동량만큼(offsetDistance를 더해서) 빼는 것이다.
				m_vPosition.x += vDirVector.x * fDistance;
			}
		}
		// C=-------|---->D
		if (vNormal.y < 0.5f && vNormal.x < 0.5f)
		{
			if (CollisionCheck(m_vPointC, m_vPointD, vec0, vec1, vec2, vNormal, vDirVector, vIntersection, fDistance))
			{
				m_vPosition.x += vDirVector.x * fDistance;
			}
		}

		// 앞뒤// E=-------|---->F
		if (vNormal.y < 0.5f && vNormal.z >= 0.5f)
		{
			if (CollisionCheck(m_vPointE, m_vPointF, vec0, vec1, vec2, vNormal, vDirVector, vIntersection, fDistance))
			{
				m_vPosition.z += vDirVector.z * fDistance;
			}
		}
		// E<-------|----=F
		if (vNormal.y < 0.5f && vNormal.z < 0.5f)
		{
			if (CollisionCheck(m_vPointF, m_vPointE, vec0, vec1, vec2, vNormal, vDirVector, vIntersection, fDistance))
			{
				m_vPosition.z += vDirVector.z * fDistance;
			}
		}

		// 바닥( 경사도 45도 기준)
		if (vNormal.y > 0.5f)
		{
			if (CollisionCheck(m_vPointA, m_vPointB, vec0, vec1, vec2, vNormal, vDirVector, vIntersection, fDistance))
			{
				m_vPosition.y = vIntersection.y;
			}
		}
		
	}

	m_vCollisionTriangle[0].y += 0.1f;
	m_vCollisionTriangle[1].y += 0.1f;
	m_vCollisionTriangle[2].y += 0.1f;
	m_vBackPosition = m_vPosition;

	return true;
}

//bool TCollision::CameraTick()
//{
//	BSPNODE* pNode = Tge.m_pLeafBsp->m_pBSPTree;	
//	
//	while( pNode )
//	{		
//		if( pNode->bIsLeaf == 3 )
//			return true;
//
//		switch( Tge.m_pLeafBsp->ClassifyPoint( pNode->pSplitter, &Tge.m_Camera.m_vCameraPos ) )
//		{
//			case CP_ONPLANE:		
//			case CP_FRONT:
//				pNode = pNode->pFront;
//				break;
//			case CP_BACK:
//				pNode = pNode->pBack;
//				break;
//			default:
//				DEBUGMSG("BSP 트리가 이상합니다.")
//		}
//	}		
//	return false;
//}
bool TCollision::RayCalculation(TVector3* vPos, TVector3 vStart, TVector3 vEnd)
{
	TVector3 Inter;
	float fPercentage = 0.0f;
	for (auto& pPoly : m_pCollisionPoly)
	{
		TVector3* vec0 = (TVector3*)&pPoly.vVertex[0];
		TVector3* vec1 = (TVector3*)&pPoly.vVertex[1];
		TVector3* vec2 = (TVector3*)&pPoly.vVertex[2];
		//TVector3 *vec3 = (TVector3*)&pPoly->pVertexList[3];

		if (RayIntersectToTri(vStart, vEnd, *vec0, pPoly.vNormal,Inter, fPercentage))
		{
			if (PointInPolygon(&Inter, &pPoly.vNormal, vec0, vec1, vec2))
			{
				*vPos = Inter;
				return true;
			}
			/*if( PointInPolygon( &Inter, &pPoly.vNormal, vec0, vec2, vec3 ) )
			{
				*vPos = Inter;
				return true;
			}*/
		}
	}
	return false;
}

bool TCollision::RayIntersectToTri(TVector3& vStart, TVector3& vEnd,
	TVector3& vPlaneVertex,
	TVector3& vPlaneNormal,
	TVector3& vIntersection,
	float& fPercentage)
{
	TVector3 vDirection;

	// 뒤면컬링
	PointPosition vA = CheckPosition(vPlaneNormal, vStart);
	PointPosition vB = CheckPosition(vPlaneNormal, vEnd);
	if (vA == vB)	return false;
	TVector3 L1 = vStart - vPlaneVertex;
	TVector3 L2 = vStart - vEnd;
	float f0, f1;
	f0 = D3DXVec3Dot(&L1, &vPlaneNormal);
	f1 = D3DXVec3Dot(&L2, &vPlaneNormal);
	fPercentage = f0 / f1;//(L1 | N) / (L2 | N);
	if (0.0f <= fPercentage && fPercentage <= 1.0f)
	{
		vIntersection = vStart - L2 * fPercentage;
		return true;
	}
	return false;
}
PointPosition TCollision::CheckPosition(TVector3 vNormal, TVector3 pPos)
{
	float fResult = D3DXVec3Dot(&pPos, &vNormal);
	if (fResult > 0.0001f)
	{
		return CP_FRONT;
	}
	if (fResult < -0.0001f)
	{
		return CP_BACK;
	}
	return CP_ONPLANE;
}
bool TCollision::CameraTick()
{
	bool bResult = false;
	m_vCPosition = m_vCameraPos;
	m_vCPointC = TVector3(m_vCBackPosition.x + 5.0f,m_vCBackPosition.y - 5.0f,m_vCBackPosition.z);
	m_vCPointD = TVector3(m_vCPosition.x - 5.0f,m_vCBackPosition.y - 5.0f,m_vCPosition.z);
	m_vCPointE = TVector3(m_vCBackPosition.x,m_vCBackPosition.y - 5.0f,m_vCBackPosition.z + 5.0f);
	m_vCPointF = TVector3(m_vCPosition.x,m_vCBackPosition.y - 5.0f,m_vCPosition.z - 5.0f);

	TVector3 Inter;
	float fPercentage = 0.0f;
	m_bCollision = false;
	for (auto& pPoly : m_pCollisionPoly)
	{
		float fDistance = 0.0f;

		TVector3* vec0 = (TVector3*)&pPoly.vVertex[0];
		TVector3* vec1 = (TVector3*)&pPoly.vVertex[1];
		TVector3* vec2 = (TVector3*)&pPoly.vVertex[2];

		if (pPoly.vNormal.y < 0.5f && pPoly.vNormal.x >= 0.5f
			&& GetIntersection(m_vCPointC, m_vCPointD, *vec0, pPoly.vNormal,Inter, fPercentage))
		{
			float fPer = 1.0f;
			fPer -= fPercentage;
			TVector3 vDir = m_vCPointD - m_vCPointC;
			D3DXVec3Normalize(&vDir, &vDir);
			vDir *= -1.0f;
			if (PointInPolygon(&Inter, &pPoly.vNormal, vec0, vec1, vec2))
			{
				m_vCameraIntersection = Inter;
				m_vCPosition.x += vDir.x * (fPer + m_fUnit);
				m_vCollisionTriangle[0] = *vec0;
				m_vCollisionTriangle[1] = *vec1;
				m_vCollisionTriangle[2] = *vec2;
				bResult = true;
			}
		}
		if (pPoly.vNormal.y < 0.5f && pPoly.vNormal.x < 0.5f
			&& RayIntersectToTri(m_vCPointD, m_vCPointC, *vec0, pPoly.vNormal,	Inter, fPercentage))
		{
			float fPer = 1.0f;
			fPer -= fPercentage;
			TVector3 vDir = m_vCPointC - m_vCPointD;
			D3DXVec3Normalize(&vDir, &vDir);
			vDir *= -1.0f;
			if (PointInPolygon(&Inter, &pPoly.vNormal, vec0, vec1, vec2))
			{
				m_vCameraIntersection = Inter;
				m_vCPosition.x += vDir.x * (fPer + m_fUnit);
				m_vCollisionTriangle[0] = *vec0;
				m_vCollisionTriangle[1] = *vec1;
				m_vCollisionTriangle[2] = *vec2;
				bResult = true;

			}
		}
		if (pPoly.vNormal.y < 0.5f && pPoly.vNormal.z >= 0.5f
			&& RayIntersectToTri(m_vCPointE, m_vCPointF, *vec0, pPoly.vNormal,	Inter, fPercentage))
		{
			float fPer = 1.0f;
			fPer -= fPercentage;
			TVector3 vDir = m_vCPointF - m_vCPointE;
			D3DXVec3Normalize(&vDir, &vDir);
			vDir *= -1.0f;
			if (PointInPolygon(&Inter, &pPoly.vNormal, vec0, vec1, vec2))
			{
				m_vCameraIntersection = Inter;
				m_vCPosition.z += vDir.z * (fPer + m_fUnit);
				m_vCollisionTriangle[0] = *vec0;
				m_vCollisionTriangle[1] = *vec1;
				m_vCollisionTriangle[2] = *vec2;
				bResult = true;

			}
		}
		if (pPoly.vNormal.y < 0.5f && pPoly.vNormal.z < 0.5f
			&& RayIntersectToTri(m_vCPointF, m_vCPointE, *vec0, pPoly.vNormal,	Inter, fPercentage))
		{
			float fPer = 1.0f;
			fPer -= fPercentage;
			TVector3 vDir = m_vCPointE - m_vCPointF;
			D3DXVec3Normalize(&vDir, &vDir);
			vDir *= -1.0f;
			if (PointInPolygon(&Inter, &pPoly.vNormal, vec0, vec1, vec2))
			{
				m_vCameraIntersection = Inter;
				m_vCPosition.z += vDir.z * (fPer + m_fUnit);
				m_vCollisionTriangle[0] = *vec0;
				m_vCollisionTriangle[1] = *vec1;
				m_vCollisionTriangle[2] = *vec2;
				bResult = true;

			}
		}
		
	}

	m_vCollisionTriangle[0].y += 0.1f;
	m_vCollisionTriangle[1].y += 0.1f;
	m_vCollisionTriangle[2].y += 0.1f;

	if (bResult)
	{
		m_vCBackPosition = m_vCPosition;
		m_vCameraPos = m_vCPosition;
		return true;
	}
	m_vCBackPosition = m_vCPosition;
	return false;
}

//bool TCollision::ClassifyPoint( TPolygon* pPlane, TVector3* pPos )
//{
//	TVector3 *vec1	= (TVector3*)&pPlane->pVertexList[0];
//	TVector3 vDirection = (*vec1) - (*pPos );
//	float	fResult;
//	fResult = D3DXVec3Dot( &vDirection, &pPlane->vNormal );
//	if( fResult < -0.001f )
//		return true;
//
//	return false;
//}
bool TCollision::PointInPolygon(TVector3* vect,
	TVector3* faceNormal,
	TVector3* point1,
	TVector3* point2,
	TVector3* point3)
{
	TVector3 pointA;
	TVector3 PointB;
	TVector3 pointX;
	TVector3 normal;
	TVector3 normalA;


	pointA = *point3 - *point2;
	PointB = *point1 - *point2;
	pointX = *vect - *point2;

	D3DXVec3Cross(&normal, &pointA, &pointX);
	D3DXVec3Normalize(&normalA, &normal);
	float Dot = D3DXVec3Dot(faceNormal, &normalA);


	if (Dot < 0.0f)
		return false;

	D3DXVec3Cross(&normal, &pointX, &PointB);
	D3DXVec3Normalize(&normalA, &normal);
	Dot = D3DXVec3Dot(faceNormal, &normalA);
	if (Dot < 0.0f)
		return false;


	pointA = *point1 - *point3;
	PointB = *point2 - *point3;
	pointX = *vect - *point3;

	D3DXVec3Cross(&normal, &pointX, &PointB);
	D3DXVec3Normalize(&normalA, &normal);
	Dot = D3DXVec3Dot(faceNormal, &normalA);
	if (Dot < 0.0f)
		return false;
	D3DXVec3Cross(&normal, &pointA, &pointX);
	D3DXVec3Normalize(&normalA, &normal);
	Dot = D3DXVec3Dot(faceNormal, &normalA);

	if (Dot < 0.0f)
		return false;


	m_bCollision = true;
	return true;
}
void TCollision::CollisionPolyRender()
{
	//if (m_pCollisionPoly)
	//{
	//	g_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	//	g_pd3dDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	//	for (TPolygon* pPoly = m_pCollisionPoly; pPoly; pPoly = pPoly->Next)
	//	{
	//		g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
	//		g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	//		g_pd3dDevice->SetFVF(P3NCT1VERTEX::FVF);
	//		g_pd3dDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST,
	//			0,
	//			pPoly->dwNumberOfVertices,
	//			pPoly->dwNumberOfIndices / 3,
	//			pPoly->pIndices,
	//			D3DFMT_INDEX32,
	//			pPoly->pVertexList,
	//			sizeof(P3NCT1VERTEX));
	//	}
	//}
}
bool TCollision::Release()
{
	return true;
}
TCollision::TCollision()
{
	m_vPosition = m_vBackPosition = TVector3(0.0f, 0.0f, 0.0f);
	m_vCPosition = m_vCBackPosition = TVector3(0.0f, 0.0f, 0.0f);
	m_bCollision = false;
	m_fUnit = 0.001f;
}

TCollision::~TCollision()
{

}


bool  TCollision::ChkOBBToOBB(const T_BOX& box0, const T_BOX& box1)
{
	TVector3 D(box1.vCenter.x - box0.vCenter.x,
		box1.vCenter.y - box0.vCenter.y,
		box1.vCenter.z - box0.vCenter.z);

	float C[3][3];
	float absC[3][3];
	float AD[3];
	float R0, R1, R;
	float R01;

	//A0
	C[0][0] = D3DXVec3Dot(&box0.vAxis[0], &box1.vAxis[0]);
	C[0][1] = D3DXVec3Dot(&box0.vAxis[0], &box1.vAxis[1]);
	C[0][2] = D3DXVec3Dot(&box0.vAxis[0], &box1.vAxis[2]);
	AD[0] = D3DXVec3Dot(&box0.vAxis[0], &D);
	absC[0][0] = (float)fabs(C[0][0]);
	absC[0][1] = (float)fabs(C[0][1]);
	absC[0][2] = (float)fabs(C[0][2]);
	R = (float)fabs(AD[0]);
	R1 = box1.fExtent[0] * absC[0][0] + box1.fExtent[1] * absC[0][1] + box1.fExtent[2] * absC[0][2];
	R01 = box0.fExtent[0] + R1;
	if (R > R01)return false;

	//A1
	C[1][0] = D3DXVec3Dot(&box0.vAxis[1], &box1.vAxis[0]);
	C[1][1] = D3DXVec3Dot(&box0.vAxis[1], &box1.vAxis[1]);
	C[1][2] = D3DXVec3Dot(&box0.vAxis[1], &box1.vAxis[2]);
	AD[1] = D3DXVec3Dot(&box0.vAxis[1], &D);
	absC[1][0] = (float)fabs(C[1][0]);
	absC[1][1] = (float)fabs(C[1][1]);
	absC[1][2] = (float)fabs(C[1][2]);
	R = (float)fabs(AD[1]);
	R1 = box1.fExtent[0] * absC[1][0] + box1.fExtent[1] * absC[1][1] + box1.fExtent[2] * absC[1][2];
	R01 = box0.fExtent[1] + R1;
	if (R > R01)return false;

	//A2
	C[2][0] = D3DXVec3Dot(&box0.vAxis[2], &box1.vAxis[0]);
	C[2][1] = D3DXVec3Dot(&box0.vAxis[2], &box1.vAxis[1]);
	C[2][2] = D3DXVec3Dot(&box0.vAxis[2], &box1.vAxis[2]);
	AD[2] = D3DXVec3Dot(&box0.vAxis[2], &D);
	absC[2][0] = (float)fabs(C[2][0]);
	absC[2][1] = (float)fabs(C[2][1]);
	absC[2][2] = (float)fabs(C[2][2]);
	R = (float)fabs(AD[2]);
	R1 = box1.fExtent[0] * absC[2][0] + box1.fExtent[1] * absC[2][1] + box1.fExtent[2] * absC[2][2];
	R01 = box0.fExtent[2] + R1;
	if (R > R01)return false;

	//B0
	R = (float)fabs(D3DXVec3Dot(&box1.vAxis[0], &D));
	R0 = box0.fExtent[0] * absC[0][0] + box0.fExtent[1] * absC[1][0] + box0.fExtent[2] * absC[2][0];
	R01 = R0 + box1.fExtent[0];
	if (R > R01)return false;

	//B1
	R = (float)fabs(D3DXVec3Dot(&box1.vAxis[1], &D));
	R0 = box0.fExtent[0] * absC[0][1] + box0.fExtent[1] * absC[1][1] + box0.fExtent[2] * absC[2][1];
	R01 = R0 + box1.fExtent[1];
	if (R > R01)return false;

	//B2
	R = (float)fabs(D3DXVec3Dot(&box1.vAxis[2], &D));
	R0 = box0.fExtent[0] * absC[0][2] + box0.fExtent[1] * absC[1][2] + box0.fExtent[2] * absC[2][2];
	R01 = R0 + box1.fExtent[2];
	if (R > R01)return false;

	//A0xB0
	R = (float)fabs(AD[2] * C[1][0] - AD[1] * C[2][0]);
	R0 = box0.fExtent[1] * absC[2][0] + box0.fExtent[2] * absC[1][0];
	R1 = box1.fExtent[1] * absC[0][2] + box1.fExtent[2] * absC[0][1];
	R01 = R0 + R1;
	if (R > R01)return false;

	//A0xB1
	R = (float)fabs(AD[2] * C[1][1] - AD[1] * C[2][1]);
	R0 = box0.fExtent[1] * absC[2][1] + box0.fExtent[2] * absC[1][1];
	R1 = box1.fExtent[0] * absC[0][2] + box1.fExtent[2] * absC[0][0];
	R01 = R0 + R1;
	if (R > R01)return false;

	//A0xB2
	R = (float)fabs(AD[2] * C[1][2] - AD[1] * C[2][2]);
	R0 = box0.fExtent[1] * absC[2][2] + box0.fExtent[2] * absC[1][2];
	R1 = box1.fExtent[0] * absC[0][1] + box1.fExtent[1] * absC[0][0];
	R01 = R0 + R1;
	if (R > R01)return false;

	//A1xB0
	R = (float)fabs(AD[0] * C[2][0] - AD[2] * C[0][0]);
	R0 = box0.fExtent[0] * absC[2][0] + box0.fExtent[2] * absC[0][0];
	R1 = box1.fExtent[1] * absC[1][2] + box1.fExtent[2] * absC[1][1];
	R01 = R0 + R1;
	if (R > R01)return false;

	//A1xB1
	R = (float)fabs(AD[0] * C[2][1] - AD[2] * C[0][1]);
	R0 = box0.fExtent[0] * absC[2][1] + box0.fExtent[2] * absC[0][1];
	R1 = box1.fExtent[0] * absC[1][2] + box1.fExtent[2] * absC[1][0];
	R01 = R0 + R1;
	if (R > R01)return false;

	//A1xB2
	R = (float)fabs(AD[0] * C[2][2] - AD[2] * C[0][2]);
	R0 = box0.fExtent[0] * absC[2][2] + box0.fExtent[2] * absC[0][2];
	R1 = box1.fExtent[0] * absC[1][1] + box1.fExtent[1] * absC[1][0];
	R01 = R0 + R1;
	if (R > R01)return false;

	//A2xB0
	R = (float)fabs(AD[1] * C[0][0] - AD[0] * C[1][0]);
	R0 = box0.fExtent[0] * absC[1][0] + box0.fExtent[1] * absC[0][0];
	R1 = box1.fExtent[1] * absC[2][2] + box1.fExtent[2] * absC[2][1];
	R01 = R0 + R1;
	if (R > R01)return false;

	//A2xB1
	R = (float)fabs(AD[1] * C[0][1] - AD[0] * C[1][1]);
	R0 = box0.fExtent[0] * absC[1][1] + box0.fExtent[1] * absC[0][1];
	R1 = box1.fExtent[0] * absC[2][2] + box1.fExtent[2] * absC[2][0];
	R01 = R0 + R1;
	if (R > R01)return false;

	//A2xB2
	R = (float)fabs(AD[1] * C[0][2] - AD[0] * C[1][2]);
	R0 = box0.fExtent[0] * absC[1][2] + box0.fExtent[1] * absC[0][2];
	R1 = box1.fExtent[0] * absC[2][1] + box1.fExtent[1] * absC[2][0];
	R01 = R0 + R1;
	if (R > R01)return false;

	return true;
}
