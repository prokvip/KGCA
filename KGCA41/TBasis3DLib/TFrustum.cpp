#include "TFrustum.h"
#define		PLANE_EPSILON	0.001f

bool T_PLANE::CreatePlane( TVector3 v0, TVector3 v1, TVector3 v2 )
{
	TVector3 vEdge0 = v1 - v0;
	TVector3 vEdge1 = v2 - v0;
	TVector3 vNormal;
	D3DXVec3Cross( &vNormal, &vEdge0, &vEdge1 );
	D3DXVec3Normalize( &vNormal, &vNormal );

	fA = vNormal.x;	fB = vNormal.y;	fC = vNormal.z;
	fD = -(fA*v0.x + fB*v0.y + fC*v0.z);
	return true;
}
bool T_PLANE::CreatePlane( TVector3 vNormal, TVector3 v0 )
{
	D3DXVec3Normalize( &vNormal, &vNormal );

	fA = vNormal.x;	fB = vNormal.y;	fC = vNormal.z;
	fD = -(fA*v0.x + fB*v0.y + fC*v0.z);
	return true;
}
void TFrustum::SetMatrix( TMatrix* pWorld, TMatrix* pView, TMatrix* pProj )
{
	if( pWorld != NULL )
	{
		m_mWorld	= *pWorld;
	}
	else
	{
		D3DXMatrixIdentity(&m_mWorld);
	}
	if( pView != NULL )
	{
		m_mView	= *pView;
	}
	if( pProj != NULL )
	{
		m_mProj	= *pProj;
	}	
}
void TFrustum::ExtractPlanesD3D( T_PLANE* pPlanes, const TMatrix & comboMatrix)
{
	if( pPlanes== NULL ) return;
	// Left clipping plane
	pPlanes[0].fA = comboMatrix._14 + comboMatrix._11;
	pPlanes[0].fB = comboMatrix._24 + comboMatrix._21;
	pPlanes[0].fC = comboMatrix._34 + comboMatrix._31;
	pPlanes[0].fD = comboMatrix._44 + comboMatrix._41;
	// Right clipping plane
	pPlanes[1].fA = comboMatrix._14 - comboMatrix._11;
	pPlanes[1].fB = comboMatrix._24 - comboMatrix._21;
	pPlanes[1].fC = comboMatrix._34 - comboMatrix._31;
	pPlanes[1].fD = comboMatrix._44 - comboMatrix._41;
	// Top clipping plane
	pPlanes[2].fA = comboMatrix._14 - comboMatrix._12;
	pPlanes[2].fB = comboMatrix._24 - comboMatrix._22;
	pPlanes[2].fC = comboMatrix._34 - comboMatrix._32;
	pPlanes[2].fD = comboMatrix._44 - comboMatrix._42;
	// Bottom clipping plane
	pPlanes[3].fA = comboMatrix._14 + comboMatrix._12;
	pPlanes[3].fB = comboMatrix._24 + comboMatrix._22;
	pPlanes[3].fC = comboMatrix._34 + comboMatrix._32;
	pPlanes[3].fD = comboMatrix._44 + comboMatrix._42;
	// Near clipping plane
	pPlanes[4].fA = comboMatrix._13;
	pPlanes[4].fB = comboMatrix._23;
	pPlanes[4].fC = comboMatrix._33;
	pPlanes[4].fD = comboMatrix._43;
	// Far clipping plane
	pPlanes[5].fA = comboMatrix._14 - comboMatrix._13;
	pPlanes[5].fB = comboMatrix._24 - comboMatrix._23;
	pPlanes[5].fC = comboMatrix._34 - comboMatrix._33;
	pPlanes[5].fD = comboMatrix._44 - comboMatrix._43;
	
	pPlanes[0].Normalize();
	pPlanes[1].Normalize();
	pPlanes[2].Normalize();
	pPlanes[3].Normalize();
	pPlanes[4].Normalize();
	pPlanes[5].Normalize();	
}
bool TFrustum::CreateFrustum()
{
	D3DXMatrixMultiply(&m_matViewProj, &m_mView, &m_mProj);
	ExtractPlanesD3D( m_Plane, m_matViewProj );
	
		// view * proj의 역행렬을 구한다.
	D3DXMatrixInverse(&m_matViewProj, NULL, &m_matViewProj );
	// 상단
	// 5    6
	// 1    2
	// 하단
	// 4    7
	// 0    3  
	m_vFrustum[0] = TVector3( -1.0f, -1.0f, 0.0f );
	m_vFrustum[1] = TVector3( -1.0f,   1.0f, 0.0f );
	m_vFrustum[2] = TVector3(  1.0f,    1.0f, 0.0f );
	m_vFrustum[3] = TVector3(  1.0f,  -1.0f, 0.0f );

	m_vFrustum[4] = TVector3( -1.0f, -1.0f,  1.0f );
	m_vFrustum[5] = TVector3( -1.0f,   1.0f,  1.0f );
	m_vFrustum[6] = TVector3(  1.0f,    1.0f,  1.0f );
	m_vFrustum[7] = TVector3(  1.0f,  -1.0f,  1.0f );

	// Vertex_최종 = Vertex_local * Matrix_world * Matrix_view * Matrix_Proj 인데,
	// Vertex_world = Vertex_local * Matrix_world이므로,
	// Vertex_최종 = Vertex_world * Matrix_view * Matrix_Proj 이다.
	// Vertex_최종 = Vertex_world * ( Matrix_view * Matrix_Proj ) 에서
	// 역행렬( Matrix_view * Matrix_Proj )^-1를 양변에 곱하면
	// Vertex_최종 * 역행렬( Matrix_view * Matrix_Proj )^-1 = Vertex_World 가 된다.
	// 그러므로, m_vFrustum * matInv = Vertex_world가 되어, 월드좌표계의 프러스텀 좌표를 얻을 수 있다.
	for( int iVertex = 0; iVertex < 8; iVertex++ )
	{
		D3DXVec3TransformCoord( &m_vFrustum[iVertex], &m_vFrustum[iVertex], &m_matViewProj );
	}	

	m_Plane[0].CreatePlane( m_vFrustum[5]  , m_vFrustum[0], m_vFrustum[1]);	// 좌 평면(left)
	m_Plane[1].CreatePlane( m_vFrustum[3], m_vFrustum[6], m_vFrustum[2]);	// 우 평면(right)
	m_Plane[2].CreatePlane(m_vFrustum[5], m_vFrustum[2], m_vFrustum[6]);	// 상 평면(top)
	m_Plane[3].CreatePlane( m_vFrustum[0]  , m_vFrustum[7], m_vFrustum[3]);	// 하 평면(bottom)
	m_Plane[4].CreatePlane( m_vFrustum[0]  , m_vFrustum[2], m_vFrustum[1]);	// 근 평면(near)
	m_Plane[5].CreatePlane( m_vFrustum[6], m_vFrustum[4], m_vFrustum[5]);	// 원 평면(far)
	return true;
}
bool TFrustum::Init()
{
	m_vNearCenter =TVector3( 0.0f, 0.0f, 0.0f);
	ZeroMemory( m_vFrustum, sizeof( m_vFrustum[0] ) * 8 );
	ZeroMemory( m_DxPlane, sizeof( m_DxPlane[0] ) * 6 );	
	m_VertexList.resize(36);
	return true;
}
bool TFrustum::CreateRenderBox(ID3D11Device*  pd3dDevice, ID3D11DeviceContext* pImmediateContext)
{
	m_pd3dDevice = pd3dDevice;
	if( m_pBoxShape != NULL ) return true;
	SAFE_NEW( m_pBoxShape, TBoxShape );
	if( FAILED( m_pBoxShape->Create(pd3dDevice, Res(L"shader/box.hlsl").c_str()) ) )
	{
		MessageBox( 0, _T("m_LineShape 실패"), _T("Fatal error"), MB_OK );
		return false;
	}	
	// vertex color
	m_pPixelShader.Attach(DX::LoadPixelShaderFile(pd3dDevice, Res(L"shader/box.hlsl").c_str(), "PS_Color"));

	return true;
}
bool TFrustum::PreRender(ID3D11DeviceContext*    pContext)
{
	if( !m_pBoxShape ) return true;
	// 상단
	// 5    6
	// 1    2
	// 하단
	// 4    7
	// 0    3  
	// 앞면
	m_VertexList[0] = PNCT_VERTEX(m_vFrustum[1], TVector3( 0.0f, 0.0f, -1.0f ),TVector4( 1.0f,0.0f, 0.0f, 0.5f ),TVector2( 0.0f, 0.0f)  );
	m_VertexList[1] = PNCT_VERTEX(m_vFrustum[2],  TVector3( 0.0f, 0.0f, -1.0f ),TVector4( 1.0f,0.0f, 0.0f, 0.5f),TVector2( 1.0f, 0.0f)  );
	m_VertexList[2] = PNCT_VERTEX(m_vFrustum[3],  TVector3( 0.0f,0.0f, -1.0f ),TVector4( 1.0f,0.0f, 0.0f, 0.5f),TVector2( 1.0f, 1.0f) );
	m_VertexList[3] = PNCT_VERTEX(m_vFrustum[0], TVector3( 0.0f,0.0f, -1.0f ),TVector4( 1.0f,0.0f, 0.0f, 0.5f),TVector2( 0.0f, 1.0f) );
	// 뒷면
	m_VertexList[4] = PNCT_VERTEX(m_vFrustum[6], TVector3( 0.0f, 0.0f, 1.0f ),TVector4(0.0f,1.0f, 0.0f, 0.5f),TVector2( 0.0f, 0.0f)  );
	m_VertexList[5] = PNCT_VERTEX(m_vFrustum[5],  TVector3( 0.0f, 0.0f, 1.0f ),TVector4(0.0f,1.0f, 0.0f, 0.5f),TVector2( 1.0f, 0.0f) );
	m_VertexList[6] = PNCT_VERTEX(m_vFrustum[4],  TVector3( 0.0f, 0.0f, 1.0f ),TVector4(0.0f,1.0f, 0.0f, 0.5f),TVector2( 1.0f, 1.0f)  );
	m_VertexList[7] = PNCT_VERTEX(m_vFrustum[7], TVector3( 0.0f, 0.0f, 1.0f ), TVector4(0.0f,1.0f, 0.0f, 0.5f),TVector2( 0.0f, 1.0f) );

	// 왼쪽
	m_VertexList[8] = PNCT_VERTEX(m_vFrustum[5], TVector3( 1.0f, 0.0f, 0.0f ),TVector4(0.0f, 0.0f, 1.0f, 0.5f),TVector2( 0.0f, 0.0f)  );
	m_VertexList[9] = PNCT_VERTEX(m_vFrustum[1],  TVector3( 1.0f, 0.0f, 0.0f ),TVector4(0.0f, 0.0f, 1.0f, 0.5f),TVector2( 1.0f, 0.0f)  );
	m_VertexList[10] = PNCT_VERTEX(m_vFrustum[0],  TVector3( 1.0f, 0.0f, 0.0f ),TVector4(0.0f, 0.0f, 1.0f, 0.5f),TVector2( 1.0f, 1.0f)  );
	m_VertexList[11] = PNCT_VERTEX(m_vFrustum[4],  TVector3( 1.0f, 0.0f, 0.0f ),TVector4(0.0f, 0.0f, 1.0f, 0.5f),TVector2( 0.0f, 1.0f) );

	// 오늘쪽면
	m_VertexList[12] = PNCT_VERTEX(m_vFrustum[2],TVector3( -1.0f, 0.0f, 0.0f ), TVector4( 1.0f,0.0f, 1.0f, 0.5f),TVector2( 0.0f, 0.0f)  );
	m_VertexList[13] = PNCT_VERTEX(m_vFrustum[6],  TVector3( -1.0f, 0.0f, 0.0f ),TVector4( 1.0f,0.0f, 1.0f, 0.5f),TVector2( 1.0f, 0.0f)  );
	m_VertexList[14] = PNCT_VERTEX(m_vFrustum[7],  TVector3( -1.0f, 0.0f, 0.0f ),TVector4( 1.0f,0.0f, 1.0f, 0.5f),TVector2( 1.0f, 1.0f)  );
	m_VertexList[15] = PNCT_VERTEX(m_vFrustum[3],  TVector3( -1.0f, 0.0f, 0.0f ),TVector4( 1.0f,0.0f, 1.0f, 0.5f),TVector2( 0.0f, 1.0f)  );

	// 윗면
	m_VertexList[16] = PNCT_VERTEX(m_vFrustum[5], TVector3( 0.0f, 1.0f, 0.0f ),TVector4( 1.0f, 1.0f, 0.0f, 0.5f),TVector2( 0.0f, 0.0f)  );
	m_VertexList[17] = PNCT_VERTEX(m_vFrustum[6],  TVector3( 0.0f, 1.0f, 0.0f ),TVector4( 1.0f, 1.0f, 0.0f, 0.5f),TVector2( 1.0f, 0.0f)  );
	m_VertexList[18] = PNCT_VERTEX(m_vFrustum[2],  TVector3( 0.0f, 1.0f, 0.0f ),TVector4( 1.0f, 1.0f, 0.0f, 0.5f),TVector2( 1.0f, 1.0f) );
	m_VertexList[19] = PNCT_VERTEX(m_vFrustum[1],  TVector3( 0.0f, 1.0f, 0.0f ),TVector4( 1.0f, 1.0f, 0.0f, 0.5f),TVector2( 0.0f, 1.0f) );

	// 아랫면
	m_VertexList[20] = PNCT_VERTEX(m_vFrustum[0],TVector3( 0.0f, -1.0f, 0.0f ), TVector4(0.0f,1.0f, 1.0f, 0.5f),TVector2( 0.0f, 0.0f)  );
	m_VertexList[21] = PNCT_VERTEX(m_vFrustum[3],  TVector3( 0.0f, -1.0f, 0.0f ),TVector4(0.0f,1.0f, 1.0f, 0.5f),TVector2( 1.0f, 0.0f)  );
	m_VertexList[22] = PNCT_VERTEX(m_vFrustum[7],  TVector3( 0.0f, -1.0f, 0.0f ),TVector4(0.0f,1.0f, 1.0f, 0.5f),TVector2( 1.0f, 1.0f)  );
	m_VertexList[23] = PNCT_VERTEX(m_vFrustum[4],  TVector3( 0.0f, -1.0f, 0.0f ),TVector4(0.0f,1.0f, 1.0f, 0.5f),TVector2( 0.0f, 1.0f)  );

	pContext->UpdateSubresource( m_pBoxShape->m_dxobj.g_pVertexBuffer.Get(), 0, NULL, &m_VertexList.at(0), 0, 0);
	m_pBoxShape->SetMatrix( NULL, &m_mView, &m_mProj );
	m_pBoxShape->m_dxobj.PreRender(pContext);
	return true;
}
bool TFrustum::Render(ID3D11DeviceContext*    pContext)
{
	PreRender(pContext);
	pContext->PSSetShader(m_pPixelShader.Get(), NULL, 0);
	PostRender(pContext);
	return true;
}
bool TFrustum::PostRender(ID3D11DeviceContext*    pContext)
{
	pContext->UpdateSubresource(m_pBoxShape->m_dxobj.g_pConstantBuffer.Get(), 0, NULL, &m_pBoxShape->m_cbData, 0, 0);
	m_pBoxShape->m_dxobj.PostRender(pContext);
	return true;
}
bool TFrustum::Release()
{
	SAFE_DEL( m_pBoxShape );
	return true;
}
bool TFrustum::ResetDevice()
{
	return true;
}

// 점 제외 처리 ( 주의 : 평면노말은 외부을 바라보고 있다. )
BOOL TFrustum::ClassifyPoint( TVector3* pv )
{
	float		fPlaneToPoint;
	for( int iPlane = 0 ; iPlane < 6 ; iPlane++ )
	{
		fPlaneToPoint = m_Plane[iPlane].fA * pv->x + m_Plane[iPlane].fB*pv->y + 
						m_Plane[iPlane].fC * pv->z + m_Plane[iPlane].fD;
		if( fPlaneToPoint < 0.0f ) 
			return FALSE;	
		/*fPlaneToCenter = D3DXPlaneDotCoord( &m_DxPlane[i], pv );
		if( fPlaneToCenter > PLANE_EPSILON ) 
			return FALSE;	*/
	}
	return TRUE;
}
// 구 제외 처리
BOOL TFrustum::ClassifySphere( T_SPHERE* pSphere )//TVector3* pv, float radius )
{
	float		fPlaneToCenter;
	for( int iPlane = 0 ; iPlane < 6 ; iPlane++ )
	{
		fPlaneToCenter =	m_Plane[iPlane].fA * pSphere->vCenter.x + m_Plane[iPlane].fB*pSphere->vCenter.y + 
							m_Plane[iPlane].fC * pSphere->vCenter.z + m_Plane[iPlane].fD;		
		/*fDist = D3DXPlaneDotCoord( &m_DxPlane[i], pv );*/
		if( fPlaneToCenter <= -pSphere->fRadius ) 
			return FALSE;
	}	
	return TRUE;
}
// OBB 제외처리( 프로스텀 내부에 존재( 걸쳐 있어도)하면 TRUE, 아니면 FALSE )
BOOL TFrustum::CheckOBBInPlane( T_BOX*  pBox )
{
	float		fPlaneToCenter=0.0;
	float		fDistance	= 0.0f;
	TVector3 vDir;
	for( int iPlane = 0 ; iPlane < 6 ; iPlane++ )
	{
		vDir = pBox->vAxis[0] * pBox->fExtent[0];
		fDistance = fabs( m_Plane[iPlane].fA * vDir.x + m_Plane[iPlane].fB*vDir.y + m_Plane[iPlane].fC * vDir.z );
		vDir = pBox->vAxis[1] * pBox->fExtent[1];
		fDistance += fabs( m_Plane[iPlane].fA * vDir.x + m_Plane[iPlane].fB*vDir.y + m_Plane[iPlane].fC * vDir.z);
		vDir = pBox->vAxis[2] * pBox->fExtent[2];
		fDistance += fabs( m_Plane[iPlane].fA * vDir.x + m_Plane[iPlane].fB*vDir.y + m_Plane[iPlane].fC * vDir.z);

		fPlaneToCenter =	m_Plane[iPlane].fA * pBox->vCenter.x + m_Plane[iPlane].fB*pBox->vCenter.y + 
							m_Plane[iPlane].fC * pBox->vCenter.z + m_Plane[iPlane].fD;	

		if( fPlaneToCenter <= -fDistance ) 
		{
			return FALSE;
		}
	}	
	return TRUE;
}

T_POSITION TFrustum::CheckPoitionOBBInPlane( T_BOX*  pBox )
{
	float		fPlaneToCenter=0.0;
	float		fDistance	= 0.0f;
	TVector3 vDir;
	T_POSITION  t_Position;

	t_Position = P_FRONT;
	for( int iPlane = 0 ; iPlane < 6 ; iPlane++ )
	{
		vDir = pBox->vAxis[0] * pBox->fExtent[0];
		fDistance = fabs( m_Plane[iPlane].fA * vDir.x + m_Plane[iPlane].fB*vDir.y + m_Plane[iPlane].fC * vDir.z );
		vDir = pBox->vAxis[1] * pBox->fExtent[1];
		fDistance += fabs( m_Plane[iPlane].fA * vDir.x + m_Plane[iPlane].fB*vDir.y + m_Plane[iPlane].fC * vDir.z);
		vDir = pBox->vAxis[2] * pBox->fExtent[2];
		fDistance += fabs( m_Plane[iPlane].fA * vDir.x + m_Plane[iPlane].fB*vDir.y + m_Plane[iPlane].fC * vDir.z);

		fPlaneToCenter =	m_Plane[iPlane].fA * pBox->vCenter.x + m_Plane[iPlane].fB*pBox->vCenter.y + 
							m_Plane[iPlane].fC * pBox->vCenter.z + m_Plane[iPlane].fD;	

		if( fPlaneToCenter <= fDistance )
		{
			t_Position = P_SPANNING;
		}

		if( fPlaneToCenter+1.0f < -fDistance ) 
		{
			return P_BACK;
		}
	}	
	return t_Position;
}

TFrustum::TFrustum()
{
	m_pd3dDevice = NULL;
	SAFE_ZERO( m_pBoxShape );
	Init();
}
TFrustum::~TFrustum()
{
	Release();
}

