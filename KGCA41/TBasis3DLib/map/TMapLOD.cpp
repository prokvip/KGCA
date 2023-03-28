#include "TMapLOD.h"

void  TMapLOD::InitLevelOrder( TNode* pRootNode, int iMaxDepth  )
{	
	m_LevelList.resize(iMaxDepth+1);	
	for( int iLevel = 1; iLevel < iMaxDepth+1; iLevel++ )
	{
		m_LevelList[iLevel].resize( pow(4.0f, iLevel ) );
	}
	m_LevelList[0].push_back( pRootNode );
}
// ��ü ���� ���� ��������(dwWidth)�� ���� ����(iNumDepth)�� �ش��ϴ� LOD��ġ ���� ��� 
void  TMapLOD::SetLOD( DWORD dwWidth, int iNumDepth )
{
	// �ִ� Ʈ���� ���̸� ����Ѵ�.
	// m_dwWidth-1 = pow(2, Depth );
	// Depth       = log( (m_dwWidth-1) ) / log(2);
	// int iNumDepth      = log( (float)m_dwWidth-1 ) / log(2.0f);
	// m_LevelList.resize(iNumDepth+1);	

	// Ʈ���� ������ ���̿� ���� ��������� �ִ� ���� �ε��� ����(iNumLod)
	// BuildTree()�ȿ��� m_iMaxDepth�� ���� ����� Ʈ���� �ִ� ���̷� ����ȴ�.
	m_iNumCell = (int)((dwWidth-1) / pow( 2.0f,(float)iNumDepth));	
	// LOD ���� ����( z = pow( x,y )���� y = log(z) / log(x) ) 
	m_iPatchLodCount = (int)(( log((float)m_iNumCell) / log(2.0f) ));	
}
float TMapLOD::GetExpansionRatio( TVector3 vCenter, float fRadius )
{
	float fRatio = 0.0f;
	TVector3 vCenterViewPos, vSideViewPos;
	vSideViewPos = vCenter + (*m_pCamera->GetRightVector() * fRadius);
	D3DXVec3TransformCoord( &vCenterViewPos, &vCenter, m_pCamera->GetViewMatrix() );
	D3DXVec3TransformCoord( &vSideViewPos, &vSideViewPos, m_pCamera->GetViewMatrix() );

	TVector3 vCenterProjPos, vSideProjPos;
	D3DXVec3TransformCoord( &vCenterProjPos, &vCenterViewPos, m_pCamera->GetProjMatrix() );
	D3DXVec3TransformCoord( &vSideProjPos, &vSideViewPos, m_pCamera->GetProjMatrix() );

	D3DXVec3Normalize( &vCenterProjPos, &vCenterProjPos );
	TVector2 vA = TVector2( vCenterProjPos.x * 0.5f + 0.5f, vCenterProjPos.y * 0.5f + 0.5f );
	TVector2 vB = TVector2( vSideProjPos.x* 0.5f + 0.5f, vSideProjPos.y* 0.5f + 0.5f );
	
	//TViewPort m_MainVP;
	//m_MainVP.Set( 0, 0, 800, 600, 0.0f, 1.0f );
	//D3DXVec3Project( &vCenterProjPos, &vCenter,&m_MainVP.m_ViewPort,m_pCamera->GetProjMatrix(), m_pCamera->GetViewMatrix(), 0 );
	//D3DXVec3Unproject(&vCenterProjPos, &vCenterProjPos,&m_MainVP.m_ViewPort,m_pCamera->GetProjMatrix(), m_pCamera->GetViewMatrix(), 0 );
	//fRatio = D3DXVec2Length( &(TVector2(0.0f,0.0f) - TVector2(1.0f,1.0f)));
	//fRatio = vCenterProjPos.z;//D3DXVec2Length( &(TVector2(vA-vB)));
	// ī�޶� �������� �ָ� ������ ��쿡 �ش���.
	float fDistance = D3DXVec3Length(&(*m_pCamera->GetEyePt() - vCenter));
	if (fDistance > m_pCamera->m_fFarPlane)
	{
		return 1.0f;
	}
	fRatio = 1.0f- fDistance / m_pCamera->m_fFarPlane;
	return fRatio;
}
float TMapLOD::GetExpansionRatio( TVector3 vCenter )
{
	float fDistance = D3DXVec3Length(&(*m_pCamera->GetEyePt() - vCenter));
	if (fDistance > m_pCamera->m_fFarPlane)
	{
       return 1.0f;
	}
	float fRatio = fDistance / m_pCamera->m_fFarPlane;
	return fRatio;
}
float TMapLOD::GetExpansionRatio( TVector3 vMax, TVector3 vMin )
{
	float fRatio = 0.0f;
	TVector3 vCenterViewPos, vSideViewPos;

	//TMatrix matInverse = *m_pCamera->GetViewMatrix();
	//D3DXMatrixInverse( &matInverse, 0, &matInverse);
	//matInverse._41 = 0.0f;
	//matInverse._42 = 0.0f;
	//matInverse._43 = 0.0f;
	//matInverse._44 = 1.0f;
	//D3DXVec3TransformCoord( &vMax, &vMax, &matInverse );		
	//D3DXVec3TransformCoord( &vMin, &vMin, &matInverse );

	D3DXVec3TransformCoord( &vCenterViewPos, &vMax, m_pCamera->GetViewMatrix() );		
	D3DXVec3TransformCoord( &vSideViewPos, &vMin, m_pCamera->GetViewMatrix() );

	TVector3 vCenterProjPos, vSideProjPos;
	D3DXVec3TransformCoord( &vCenterProjPos, &vCenterViewPos, m_pCamera->GetProjMatrix() );
	D3DXVec3TransformCoord( &vSideProjPos, &vSideViewPos, m_pCamera->GetProjMatrix() );

	TVector2 vA = TVector2( vCenterProjPos.x * 0.5f + 0.5f, vCenterProjPos.y * 0.5f + 0.5f );
	TVector2 vB = TVector2( vSideProjPos.x* 0.5f + 0.5f, vSideProjPos.y* 0.5f + 0.5f );
	
	fRatio = D3DXVec3Length( &(vCenterProjPos - vSideProjPos));
	//fRatio = D3DXVec2Length( &(TVector2(vA-vB)));
	return fRatio;
}
DWORD TMapLOD::GetLodSubIndex(TNode* pNode )
{
	int iNumPatchIndex = m_iNumCell;
	/*float fRatio = GetExpansionRatio(	pNode->m_tBox.vCenter, 
										D3DXVec3Length( &(pNode->m_tBox.vMax-pNode->m_tBox.vCenter )));*/	

/*	float fRatio = GetExpansionRatio(	pNode->m_tBox.vMax, pNode->m_tBox.vMin);	*/	

	float fRatio = GetExpansionRatio(	pNode->m_tBox.vCenter );
	DWORD dwCurentRatio = (DWORD)(fRatio * m_iPatchLodCount);	
	DWORD dwMaxRatio	= (DWORD)(fRatio * m_iPatchLodCount + 0.5f);
	DWORD dwMinRatio	= (DWORD)(fRatio * m_iPatchLodCount - 0.5f);

	
	// ����ȿ�� (����� LOD�� ���� �ٲ�� ���� �����Ѵ�.)
	if( m_bThresHoldValue )
	{
		if( pNode->m_dwLodLevel < dwCurentRatio ) 
		{	
			if( pNode->m_dwLodLevel < dwMinRatio  )		// B
			{
				pNode->m_dwLodLevel = dwCurentRatio;			
			}
		}
		else if( pNode->m_dwLodLevel > dwCurentRatio )
		{
			if( pNode->m_dwLodLevel > dwMaxRatio )		// B
			{
				pNode->m_dwLodLevel = dwCurentRatio;			
			}
		}	
	}
	else
	{
		pNode->m_dwLodLevel = dwCurentRatio;
	}
	///����ó��
	if ((DWORD)m_iPatchLodCount < pNode->m_dwLodLevel)
	{
         pNode->m_dwLodLevel -= 1;
	}

	return pNode->m_dwLodLevel;
}
DWORD TMapLOD::GetLodType(TNode* pNode)
{
	if(pNode->m_NeighborList.size() <=0 ) return 0;
	DWORD dwType = 0;
	if( pNode->m_NeighborList[0] && pNode->m_NeighborList[0]->m_dwLodLevel < pNode->m_dwLodLevel ) dwType += 1;
	if( pNode->m_NeighborList[1] && pNode->m_NeighborList[1]->m_dwLodLevel < pNode->m_dwLodLevel ) dwType += 4;
	if( pNode->m_NeighborList[2] && pNode->m_NeighborList[2]->m_dwLodLevel < pNode->m_dwLodLevel ) dwType += 8;
	if( pNode->m_NeighborList[3] && pNode->m_NeighborList[3]->m_dwLodLevel < pNode->m_dwLodLevel ) dwType += 2;
	
	pNode->m_dwLodType = dwType;
	return dwType;
}
// m_DrawNodeList���� ������常 m_DrawPatchNodeList�� �����Ѵ�.
void	TMapLOD::GetDrawPatchNode()
{
	for( int iNode=0; iNode < m_DrawNodeList.size(); iNode++ )
	{		
		AddDrawPatchNode(m_DrawNodeList[iNode]);
	}						
}
void TMapLOD::AddDrawPatchNode( TNode* pNode )
{
	if( pNode->m_isLeaf )
	{
		m_DrawPatchNodeList.push_back(pNode);
		GetLodSubIndex( pNode );
		return;
	}
	for( int iNode = 0; iNode < pNode->m_ChildList.size(); iNode++ )
	{
		AddDrawPatchNode( pNode->m_ChildList[iNode]);
	}
}
void TMapLOD::Update( ID3D11Device* pd3dDevice, TCamera* pCamera )
{
	m_pd3dDevice	= pd3dDevice;
	m_pCamera		= pCamera;		
}
TMapLOD::TMapLOD(void)
{
	m_fExpansionRatio	= 0.1f;
	m_iNumCell	= 1;	
	m_iPatchLodCount	= 0;
	m_pCamera			= NULL;
	m_bThresHoldValue	= false;
}

TMapLOD::~TMapLOD(void)
{
}
