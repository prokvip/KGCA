#include ".\tbsptree.h"


bool TBspTree::Load( ID3D11Device*  pd3dDevice, TCHAR* szFileName, bool Binary )
{
	SAFE_NEW( m_pRootNode, TNode );	
	if( !m_BspParser.Load( GetDevice(), m_pRootNode, szFileName, Binary ) )
	{
		return false;
	}
	m_pRootNode->iNumPolygon = CalculateBox( &m_pRootNode->m_tBox, m_pRootNode->m_pSplitter );
	
	return true;
}
bool	TBspTree::Build(ID3D11Device*   pd3dDevice, TPolygon* pPolyList, TCHAR*	pLoadShaderString )
{	
	if( !m_BspRender.Create(pd3dDevice, pLoadShaderString ) )
	{
		return false;
	}
	SAFE_NEW ( m_pRootNode, TNode )	
	m_pRootNode->m_ChildList.resize(2);		
	m_pRootNode->iNumPolygon = CalculateBox( &m_pRootNode->m_tBox, pPolyList );
	
	BuildBSPTree( m_pRootNode, pPolyList );		

	m_BspParser.SetDevice( pd3dDevice );
	m_BspParser.LoadTexture();	
	m_BspParser.SetNodePerRenderGroup(m_pRootNode);

	m_DrawObjList.reserve( 100 );
	m_DrawNodeList.reserve( 100 );
	return true;
}

bool	TBspTree::Init()
{	
	m_pRootNode		=	NULL;		
	m_pCollisionPoly=   NULL; 

	m_vCurrentPosition = TVector3 ( 0.0f, 0.0f, 0.0f );	
	return true;
}
bool	TBspTree::Frame()
{
	m_pCollisionNode = NULL;
	m_DrawObjList.clear();
	m_DrawNodeList.clear();
	if( !m_pCamera ) return false;
	VisibleNode(m_pRootNode);	

	TNode* pNode = m_pRootNode;
	if( !pNode )
		return false;
	while( pNode->m_ChildList[0] && pNode->m_ChildList[0]->m_isLeaf != TRUE )
	{		
		switch( ClassifyPoint( pNode->m_pSplitter, &m_vCurrentPosition))
		{
			case P_ONPLANE:		
			case P_FRONT:
			pNode = pNode->m_ChildList[0];
			break;
			case P_BACK:
			pNode = pNode->m_ChildList[1];
			break;
			default:
			 DEBUGMSG(L"BSP 트리가 이상합니다.");
		}
	}	
	if( pNode->m_ChildList[0] )
	{
		// m_vCurrentPosition(오브젝트 위치)가 위치한 노드 및 충돌(폴리곤)데이터
		m_pCollisionPoly = pNode->m_ChildList[0]->m_pSplitter;
		m_pCollisionNode = pNode->m_ChildList[0];
	}

	return true;
}
bool	TBspTree::Render(ID3D11DeviceContext*		pContext )
{
	m_BspRender.m_cbData.Color = TVector4( 1, 1, 1, 1 );		

	// 화면에 보이는노드 리스ㅣ트 랜더링
	for( int iNode=0; iNode < m_DrawNodeList.size(); iNode++ )
	{		
		m_BspRender.RenderNode(pContext,m_DrawNodeList[iNode] );
	}

	// m_vCurrentPosition이 위치한 노드만 랜더링
	m_BspRender.m_cbData.Color= TVector4( 1, 1, 1, 1 );
	m_BspRender.RenderNode(pContext,m_pCollisionNode);
	// 전체 트리 순회하여 랜더링
	m_BspRender.RecursionRender(pContext, m_pRootNode);
	return true;	
}
bool	TBspTree::Release()
{	
	SAFE_DEL(m_pRootNode);	
	return true;
}
void TBspTree::BuildBSPTree( TNode* pNode, TPolygon* pPolyList )
{
	TPolygon*	pFrontList		= NULL;
	TPolygon*	pBackList		= NULL;
	TPolygon*	pCurrentPoly		= pPolyList;
	TPolygon*	pNextPoly		= NULL;

	pNode->m_pSplitter = SelectBestSplitter( pCurrentPoly );
	if( pNode->m_pSplitter )	pNode->iNumPolygon = 1;

	while( pCurrentPoly != NULL )
	{
		pNextPoly = pCurrentPoly->pNext;
		if( pNode->m_pSplitter )
		{
			switch( ClassifyPoly( pNode->m_pSplitter, pCurrentPoly ) )
			{
			case P_FRONT:
			case P_ONPLANE:
				{
					pCurrentPoly->pNext	= pFrontList;
					pFrontList			= pCurrentPoly; 		
				}break;
			case P_BACK:
				{
					pCurrentPoly->pNext	= pBackList;
					pBackList			= pCurrentPoly;					
				}break;
			case P_SPANNING:
				{						
					TPolygon*	pFrontSplitter  = NULL;
					TPolygon*	pBackSplitter  = NULL;
					SAFE_NEW( pFrontSplitter, TPolygon ) 
					SAFE_NEW( pBackSplitter, TPolygon ) 
					ZeroMemory( pFrontSplitter, sizeof( TPolygon ) );
					ZeroMemory( pBackSplitter, sizeof( TPolygon ) );

					SplitterPolygon( pCurrentPoly,pNode->m_pSplitter, pFrontSplitter, pBackSplitter );
					pFrontSplitter->pNext	= pFrontList;
					pFrontList				= pFrontSplitter; 
					
					pBackSplitter->pNext	= pBackList;
					pBackList				= pBackSplitter;
					
					if(pCurrentPoly->pIndices)
					{
						SAFE_DELETE_ARRAY( pCurrentPoly->pIndices );						
					}
					if(pCurrentPoly->pVertexList)
					{
						SAFE_DELETE_ARRAY( pCurrentPoly->pVertexList ); 								
					}					
					//SAFE_DEL(pCurrentPoly);			
				}break;
			default:
				break;
			}
		}
		pCurrentPoly = pNextPoly;
	}

	if( !pBackList )
	{			
		SAFE_NEW( pNode->m_ChildList[0], TNode )		
		pNode->m_ChildList[0]->m_ChildList.resize(2);		
		pNode->m_ChildList[0]->m_isLeaf = true; 	
		if( !pFrontList ) 
		{
			pNode->m_ChildList[0]->m_pSplitter = pPolyList;				
		}
		else
		{
			pNode->m_ChildList[0]->m_pSplitter = pFrontList;				
		}
		pNode->m_ChildList[0]->iNumPolygon = CalculateBox( &pNode->m_ChildList[0]->m_tBox, pNode->m_ChildList[0]->m_pSplitter );
	}
	else
	{
		SAFE_NEW( pNode->m_ChildList[0], TNode )
		pNode->m_ChildList[0]->m_ChildList.resize(2);		
		pNode->m_ChildList[0]->m_isLeaf = false; 			
		BuildBSPTree( pNode->m_ChildList[0], pFrontList ); 
	}
	if( !pBackList )
	{
		SAFE_NEW( pNode->m_ChildList[1], TNode )
		pNode->m_ChildList[1]->m_isLeaf = 3;
	}
	else
	{
		SAFE_NEW( pNode->m_ChildList[1], TNode )
		pNode->m_ChildList[1]->m_ChildList.resize(2);		
		pNode->m_ChildList[1]->m_isLeaf = false; 			
		BuildBSPTree( pNode->m_ChildList[1], pBackList ); 
	}
}

TPolygon* TBspTree::SelectBestSplitter( TPolygon* pPolyList )
{
	TPolygon*	pSelectedPoly	= NULL;
	TPolygon*	m_pSplitter= NULL;
	int			iBestScore		= 100000;
	int iNumFront, iNumBack, iNumSplitter;

	int iScore = 0;
	for( TPolygon* pSplitterPoly = pPolyList; pSplitterPoly ; pSplitterPoly = pSplitterPoly->pNext )
	{		
		iNumFront = iNumBack = iNumSplitter = 0;
		if( !pSplitterPoly->bBeenUsedAsSplitter )
		{
			for( TPolygon* pCurrentPoly = pPolyList; pCurrentPoly ; pCurrentPoly = pCurrentPoly->pNext )
			{	
				if( ( pSplitterPoly != pCurrentPoly ) &&  !pCurrentPoly->bBeenUsedAsSplitter )
				{
					switch( ClassifyPoly( pSplitterPoly, pCurrentPoly ) )
					{
					case P_ONPLANE:
						{
							iNumFront++;							
						}break;
					case P_FRONT:
						{
							iNumFront++;
						}break;
					case P_BACK:
						{
							iNumBack++;
						};break;
					case P_SPANNING:
						{
							iNumSplitter+=300;
						}break;
					}
				}
			}

			iScore= abs( iNumFront - iNumBack ) + (iNumSplitter);
			if( iScore < iBestScore )
			{
				iBestScore		= iScore;
				pSelectedPoly	= pSplitterPoly; 				
			}
		}
	}
	if( pSelectedPoly != NULL )
		pSelectedPoly->bBeenUsedAsSplitter = true;

	if( pSelectedPoly )
	{		
		SAFE_NEW( m_pSplitter, TPolygon );		
		ZeroMemory( m_pSplitter, sizeof( TPolygon ));
		SAFE_NEW_ARRAY( m_pSplitter->pVertexList, PNCT_VERTEX, pSelectedPoly->iNumVertices )		 
		memset( m_pSplitter->pVertexList, 0, sizeof( PNCT_VERTEX ) * pSelectedPoly->iNumVertices );
		memcpy( m_pSplitter->pVertexList, pSelectedPoly->pVertexList, sizeof(PNCT_VERTEX) * pSelectedPoly->iNumVertices); 
		SAFE_NEW_ARRAY( m_pSplitter->pIndices, DWORD, pSelectedPoly->iNumIndices )
		ZeroMemory( m_pSplitter->pIndices, sizeof( DWORD ) * pSelectedPoly->iNumIndices);
		memcpy( m_pSplitter->pIndices, pSelectedPoly->pIndices, sizeof(DWORD)*pSelectedPoly->iNumIndices); 
		m_pSplitter->bBeenUsedAsSplitter = pSelectedPoly->bBeenUsedAsSplitter;
		m_pSplitter->iNumIndices = pSelectedPoly->iNumIndices;
		m_pSplitter->iNumVertices = pSelectedPoly->iNumVertices;
		m_pSplitter->iTexIndex= pSelectedPoly->iTexIndex;
		m_pSplitter->vNormal = pSelectedPoly->vNormal;
		_tcscpy( m_pSplitter->strTexName, pSelectedPoly->strTexName);
		m_pSplitter->pNext = NULL;		
	}
	return m_pSplitter;
	
}
// 폴리곤의 전체 정점이 분할면 앞, 뒤 위치를 
T_POSITION TBspTree::ClassifyPoint( TPolygon* pPlane, TVector3* pPos )
{
	TVector3 *vec1	= (TVector3*)&pPlane->pVertexList[0];
	TVector3 vDirection = (*vec1) - (*pPos );
	float	fResult;
	fResult = D3DXVec3Dot( &vDirection, &pPlane->vNormal );
	if( fResult < 0.0f )
	{
		return P_FRONT;
	}
	if( fResult > 0.0f )
	{
		return P_BACK;
	}
	return P_ONPLANE;	
}

T_POSITION TBspTree::ClassifyPoly( TPolygon* pPlane, TPolygon* pPoly )
{
	TVector3 *vec1 = (TVector3*)&pPlane->pVertexList[0];
	DWORD dwInFront, dwBehind, dwOnPlane;	
	dwInFront = dwBehind = dwOnPlane = 0;

	float	fResult;
	for( int dwVec = 0; dwVec < pPoly->iNumVertices; dwVec++ )
	{	
		TVector3 *vec2	= (TVector3*)&pPoly->pVertexList[dwVec];
		TVector3 vDirection = (*vec1) - (*vec2);
		// 평면앞에 점이 있으면 양수의 값을 얻게 된다. 
		// 그러나 여기서는 D3DXVec3Dot()의 결과를 그대로 쓰기 때문에 결과의
		// 부호가 바뀐다.
		fResult = D3DXVec3Dot( &vDirection, &pPlane->vNormal );
		if( fResult < 0.0f )
		{
			dwInFront++;
		}
		else if( fResult > 0.0f )
		{
			dwBehind++;
		}
		else
		{
			dwOnPlane++;			
		}			
	}	

	if( dwOnPlane == pPoly->iNumVertices )
	{
		// 분할평면과 같은 면상에 있는 폴리곤들은 같은 노말을 갖고 있기 때문에
		// 내적의 결과의 방향을 반대로 하여 계산해야 한다.
		// 모든 점이 분할면 상위에 있을 경우는 노말의 방향에 따라 앞뒤을 정한다.
		fResult = D3DXVec3Dot( &pPoly->vNormal, &pPlane->vNormal ) ;
		if( fResult >= 0.0f)
		{
			return P_FRONT;
		}
		else if( fResult < 0.0f)
		{
			return P_BACK;
		}
	}
	// 일부의 정점이 평면 상에 존재할 경우 나머지 정점 위치에 따라 반환됨.
	if( dwInFront == pPoly->iNumVertices || (dwInFront > 0 && dwBehind == 0) ) 
	{
		return P_FRONT;
	}
	if( dwBehind == pPoly->iNumVertices ||	(dwBehind > 0 && dwInFront == 0) ) 
	{
		return P_BACK;
	}
	return P_SPANNING;
}


void TBspTree::SplitterPolygon( TPolygon* pPoly,				//  현재 폴리곤 
								TPolygon* pPlane,			//  분할면 
								TPolygon* pFrontSplitter,	//  분할면 앞에 위치한 폴리곤 
								TPolygon* pBackSplitter )	//  분할면 뒤에 위치한 폴리곤 
{	
	vector<PNCT_VERTEX>	FrontList;
	vector<PNCT_VERTEX>	BackList;
	TVector3				Intersection;	
	float					fT;	
	TVector3	vPlanVertex = pPlane->pVertexList[0].p;

	TVector3 PointA, PointB;
	for( int dwVertex = 0; dwVertex < pPoly->iNumVertices ; dwVertex++ )
	{
		PNCT_VERTEX vVertex	= pPoly->pVertexList[dwVertex]; 	
		switch( ClassifyPoint( pPlane, &vVertex.p ) )
		{
			case P_ONPLANE:
				FrontList.push_back(vVertex);
				BackList.push_back(vVertex);					continue;	
			case P_FRONT:	FrontList.push_back(vVertex);		break;
			case P_BACK:	BackList.push_back(vVertex);		break;		
		}
		// Ray : Plane
		DWORD dwNextVertex = dwVertex+1;
		if( dwVertex == pPoly->iNumVertices-1 )	dwNextVertex = 0;

		PointA			= pPoly->pVertexList[dwVertex].p;
		PointB			= pPoly->pVertexList[dwNextVertex].p;	
		int iPointA		= ClassifyPoint( pPlane, &PointA  );
		int iPointB		= ClassifyPoint( pPlane, &PointB  );		
		// PointA 와 PointA의 분할면에 대한 결과가 다르면 결국 걸쳐있다는 것이 된다.		
		if(	iPointA != iPointB && iPointB != P_ONPLANE )
		{
			if(	GetIntersection( PointA, PointB, vPlanVertex, pPlane->vNormal, Intersection, fT ) )
			{
				PNCT_VERTEX vNew;
				PNCT_VERTEX vE = pPoly->pVertexList[dwNextVertex];
				PNCT_VERTEX vS= pPoly->pVertexList[dwVertex];
				vNew.p = Intersection;
				D3DXVec2Lerp( &vNew.t, &vS.t, &vE.t, fT );
				D3DXVec4Lerp( &vNew.c, &vS.c, &vE.c, fT );
				D3DXVec3Lerp( &vNew.n, &vS.n, &vE.n, fT );					
				FrontList.push_back(vNew);
				BackList.push_back(vNew);
			}
		}		
	}
	
	SetPolygon(pFrontSplitter, pPoly, FrontList );
	SetPolygon(pBackSplitter, pPoly, BackList );
}
void TBspTree::SetPolygon( TPolygon* pSplitter, TPolygon* pPoly,vector<PNCT_VERTEX>&	VertexList)
{
	if( VertexList.size() )	
	{
		SAFE_NEW_ARRAY( pSplitter->pVertexList, PNCT_VERTEX, VertexList.size() )
		for( int iB = 0; iB < VertexList.size(); iB++ )
		{
			pSplitter->pVertexList[iB]	= VertexList[iB]; 
		}
		pSplitter->iNumVertices		= (int)VertexList.size();
		pSplitter->iNumIndices			= (pSplitter->iNumVertices - 2) * 3;	
		pSplitter->bBeenUsedAsSplitter	= pPoly->bBeenUsedAsSplitter;	
		pSplitter->iTexIndex			= pPoly->iTexIndex;
		pSplitter->vNormal				= pPoly->vNormal;

		SAFE_NEW_ARRAY( pSplitter->pIndices, DWORD, pSplitter->iNumIndices )
	
		int iIndexA = 0;
		int iIndexB = 0;
		int iIndexC = 0;
		TVector3 vNormal;

		for( int dwFace = 0 ; dwFace < pSplitter->iNumIndices / 3; dwFace++ )
		{	
			// Triangle List( Fan )
			iIndexB = dwFace + 1;
			iIndexC = dwFace + 2;
			D3DXVec3Cross(&vNormal,	&(pSplitter->pVertexList[iIndexB].p - pSplitter->pVertexList[iIndexA].p), 
									&(pSplitter->pVertexList[iIndexC].p - pSplitter->pVertexList[iIndexA].p));	
			D3DXVec3Normalize(&vNormal, &vNormal);	
			float fResult = D3DXVec3Dot( &vNormal, &pSplitter->vNormal );
			if( fResult < 0.0f)
			{		
					int temp; 	
					temp	= iIndexB;	
					iIndexB = iIndexC;	
					iIndexC = iIndexB;
			}	
			pSplitter->pIndices[ dwFace * 3 + 0 ]	= iIndexA;
			pSplitter->pIndices[ dwFace * 3 + 1 ]	= iIndexB;
			pSplitter->pIndices[ dwFace * 3 + 2 ]	= iIndexC;		
		}		
	}	
}
bool TBspTree::GetIntersection( TVector3& vStart, TVector3& vEnd , 
							    TVector3& vPlaneVertex, 
								TVector3& vPlaneNormal, 
								TVector3& vIntersection,
								float&	fPercentage )
{
	//TVector3 L1 = vStart - vPlaneVertex;
	//TVector3 L2 = vStart - vEnd;
	//float f0,f1;
	//f0 = D3DXVec3Dot( &L1, &vPlaneNormal );
	//f1 = D3DXVec3Dot( &L2, &vPlaneNormal );	
	//fPercentage = f0 / f1;//(L1 | N) / (L2 | N);
	//if( 0.0f<= fPercentage && fPercentage <= 1.0f )
	//{
	//	vIntersection = vStart - L2 * fPercentage;			
	//	//return true;		
	//}
	////return false;

	TVector3 vDirection;
	float		fSRcDist, fDestDist;
	vDirection = vStart - vEnd;
	fSRcDist  = D3DXVec3Dot( &vDirection, &vPlaneNormal );
	fDestDist = D3DXVec3Dot( &( vStart - vPlaneVertex), &vPlaneNormal );
	fPercentage = fDestDist / fSRcDist;
	if( 0.0f<= fPercentage && fPercentage <= 1.0f )
	{
		vIntersection = vStart - vDirection * fPercentage;			
		return true;		
	}
	return false;
}

TPolygon* TBspTree::GetCollisionPoly( TPolygon* pPoly )
{
	pPoly = m_pCollisionPoly;
	return pPoly;
}


void TBspTree::SetMatrix( TMatrix* pWorld, TMatrix* pView, TMatrix* pProj )
{
	m_BspRender.SetMatrix( pWorld, pView, pProj );
}

int TBspTree::AddObject( TBaseObj* pObj )
{
	if( CheckRect( m_pRootNode, pObj ) )
	{
		TNode* pNode = FindNode( m_pRootNode, pObj );	
		if( pNode )
		{
			pNode->m_ObjectList.push_back( pObj );
		}
		return 1;
	}
	return 0;
}
int TBspTree::CheckRect( TNode* pNode, TBaseObj* pObj )
{
	if( pNode->m_tBox.vMin.x <= pObj->m_tBox.vMin.x && pNode->m_tBox.vMax.x >= pObj->m_tBox.vMax.x )
	{
		if( pNode->m_tBox.vMin.y <= pObj->m_tBox.vMin.y && pNode->m_tBox.vMax.y >= pObj->m_tBox.vMax.y )
		{
			if( pNode->m_tBox.vMin.z <= pObj->m_tBox.vMin.z && pNode->m_tBox.vMax.z >= pObj->m_tBox.vMax.z )
			{	
				return 1;
			}			
		}
	}
	return 0;
}
TNode*	TBspTree::FindNode(TNode* pNode, TBaseObj* pObj  )
{
	assert( pNode );
	do{		
		for( DWORD dwChild=0; dwChild < pNode->m_ChildList.size(); dwChild++ )
		{
			if( pNode->m_ChildList[dwChild] && CheckRect( pNode->m_ChildList[dwChild], pObj )  ) 
			{		
				m_QuadTreeQueue.push( pNode->m_ChildList[dwChild] );
			}
		}			
				
		if( m_QuadTreeQueue.empty() ) break;
		
		pNode = m_QuadTreeQueue.front();
		m_QuadTreeQueue.pop();		
	}while( pNode );
	return pNode;
}
void TBspTree::VisibleNode( TNode* pNode )
{	
	if( !pNode ) return;
	if( m_pCamera->CheckOBBInPlane( &pNode->m_tBox ) )	
	{
		if( pNode->m_isLeaf== TRUE )
		{
			m_DrawNodeList.push_back( pNode );			
		}
		for( int iObj = 0; iObj < pNode->m_ObjectList.size(); iObj++ )
		{
			if( m_pCamera->CheckOBBInPlane( &pNode->m_ObjectList[iObj]->m_tBox ) )	
			{
				m_DrawObjList.push_back( pNode->m_ObjectList[iObj] );			
			}
		}		
	}
	for( int iNode = 0; iNode < pNode->m_ChildList.size(); iNode++ )
	{		
		VisibleNode( pNode->m_ChildList[iNode] );
	}
}

TBspTree::TBspTree()
{
	m_pRootNode		=	NULL;	
	m_pCollisionPoly=   NULL; 
	m_vCurrentPosition = TVector3 ( 0.0f, 0.0f, 0.0f );
}
TBspTree::~TBspTree()
{
}
