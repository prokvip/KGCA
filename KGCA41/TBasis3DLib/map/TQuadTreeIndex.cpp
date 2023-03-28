#include "TQuadTreeIndex.h"
TStaticData::TStaticData()
{
}
TStaticData::~TStaticData()
{
	for (int iList = 0; iList < IndexBufferList.size(); iList++)
	{
		IndexList[iList].clear();
	}
	IndexBufferList.clear();
	IndexList.clear();
}
bool TQuadTreeIndex::Build( TMap* pMap, DWORD dwWidth, DWORD dwHeight )
{
	m_dwWidth	= dwWidth;
	m_dwHeight	= dwHeight;	
	m_pMap		= pMap;
	m_iNumFace	= ( dwWidth -1 )*( dwHeight -1 )*2;
	m_bUsedIndexList = true;

	SAFE_NEW_ARRAY( m_IndexList, DWORD, m_iNumFace *3*2 );
	m_pRootNode = CreateNode( NULL, 0,dwWidth-1, dwWidth * (dwHeight-1), dwWidth * dwHeight -1);

	InitLevelOrder( m_pRootNode, m_iMaxDepthLimit );

	if( BuildTree(m_pRootNode) )
	{		
		for( int iNode = 0; iNode < m_pRootNode->m_ChildList.size(); iNode++ )
		{
			SetNeighborNode( m_pRootNode->m_ChildList[iNode]);
		}
		SetLOD( m_dwWidth, m_iMaxDepth );
		ComputeStaticLodIndex(m_iNumCell);
		return true;
	}			
	return false;
}
bool TQuadTreeIndex::SubDivide(TNode* pNode)
{	
	if( pNode == NULL ) return false;
	
	// 현재 노드의 실제 크기를 계산한다.
	DWORD dwOffsetX = (pNode->m_CornerIndex[tTR] - pNode->m_CornerIndex[tTL] );
	DWORD dwOffsetZ = ((pNode->m_CornerIndex[tBL] - pNode->m_CornerIndex[tTL] ) / m_dwWidth );

	DWORD dwWidthSplit = CheckSize(dwOffsetX);
	DWORD dwHeightplit = CheckSize(dwOffsetZ);

	// 최대 깊이 한도로 제한한다.
	if( (DWORD)m_iMaxDepthLimit <= pNode->m_dwDepth )
	{
		pNode->m_isLeaf = TRUE;
		CreateVertexList(pNode);
		return false;
	}
	// 4등분 할수 없으면 더이상 분할하지 않는다.
	if( dwWidthSplit < (DWORD)m_iMinSplitDistance || dwHeightplit < (DWORD)m_iMinSplitDistance)
	{
		pNode->m_isLeaf = TRUE;
		CreateVertexList(pNode);
		return false;	
	}
	// 자식 노드가 지정된 분할크기보다 작다면 더이상 분할하지 않는다.
	if( dwWidthSplit < m_fMinDivideSize || dwHeightplit < m_fMinDivideSize)
	{
		pNode->m_isLeaf = TRUE;
		CreateVertexList(pNode);
		return false;
	}

	// 우상귀 = 좌측코너 + 스플릿크기
	// 우하귀 = 우상귀 + ( 스플릿 크기 * 전체맵 가로크기 )
	DWORD dwSplitCenter = (pNode->m_CornerIndex[tTL]+dwWidthSplit ) + (dwHeightplit*m_dwWidth);

	DWORD dwEdgeCenter[4];
	dwEdgeCenter[0] =	pNode->m_CornerIndex[tTL] + dwWidthSplit;
	dwEdgeCenter[1] =	pNode->m_CornerIndex[tTR] + dwHeightplit*m_dwWidth;
	dwEdgeCenter[2] =	pNode->m_CornerIndex[tBL] + dwWidthSplit;
	dwEdgeCenter[3] =	pNode->m_CornerIndex[tTL] + dwHeightplit*m_dwWidth;
	
	pNode->m_ChildList.push_back( CreateNode(	pNode,	pNode->m_CornerIndex[tTL],	
														dwEdgeCenter[0],			
														dwEdgeCenter[3],			
														dwSplitCenter) );

	pNode->m_ChildList.push_back( CreateNode(	pNode,	dwEdgeCenter[0],			
														pNode->m_CornerIndex[tTR],	
														dwSplitCenter,		
														dwEdgeCenter[1]));

	pNode->m_ChildList.push_back( CreateNode(	pNode,	dwEdgeCenter[3],			
														dwSplitCenter,		
														pNode->m_CornerIndex[tBL],	
														dwEdgeCenter[2] ) );

	pNode->m_ChildList.push_back( CreateNode(	pNode,	dwSplitCenter,		
														dwEdgeCenter[1],			
														dwEdgeCenter[2],			
														pNode->m_CornerIndex[tBR]) );
	return true;
}
void TQuadTreeIndex::UpdateBoundingBox(TNode* pNode)
{	
	ComputeBoungigBox( pNode );
	for( int iNode = 0; iNode < pNode->m_ChildList.size(); iNode++ )
	{
		UpdateBoundingBox( pNode->m_ChildList[iNode]);
	}	
}
void	TQuadTreeIndex::ComputeBoungigBox( TNode* pNode )
{
	TVector2 vHeight = GetHeightFromNode( pNode->m_CornerIndex[0], pNode->m_CornerIndex[1], 
											 pNode->m_CornerIndex[2], pNode->m_CornerIndex[3] );
	
	pNode->m_tBox.vMax = m_pMap->m_VertexList[pNode->m_CornerIndex[1]].p;
	pNode->m_tBox.vMin = m_pMap->m_VertexList[pNode->m_CornerIndex[2]].p;
	pNode->m_tBox.vMax.y = vHeight.x;
	pNode->m_tBox.vMin.y = vHeight.y;
	

	pNode->m_tBox.vCenter = ( pNode->m_tBox.vMax + pNode->m_tBox.vMin );
	pNode->m_tBox.vCenter.x /= 2.0f;
	pNode->m_tBox.vCenter.y /= 2.0f;
	pNode->m_tBox.vCenter.z /= 2.0f;
	pNode->m_tBox.fExtent[0] = pNode->m_tBox.vMax.x - pNode->m_tBox.vCenter.x;
	pNode->m_tBox.fExtent[1] = pNode->m_tBox.vMax.y - pNode->m_tBox.vCenter.y;
	pNode->m_tBox.fExtent[2] = pNode->m_tBox.vMax.z - pNode->m_tBox.vCenter.z;
}
TNode* TQuadTreeIndex::CreateNode( TNode* pParentNode, DWORD dwTL,DWORD dwTR,DWORD dwBL,DWORD dwBR )
{	
	TNode* pNode = 0;
	SAFE_NEW( pNode, TNode );
	assert( pNode );

	pNode->m_isLeaf = FALSE;
	
	pNode->m_ChildList.reserve( 4 );
	pNode->m_CornerList.reserve( 4 );
	pNode->m_CornerIndex.reserve( 4 );
	
	pNode->m_CornerIndex.push_back(dwTL);
	pNode->m_CornerIndex.push_back(dwTR);
	pNode->m_CornerIndex.push_back(dwBL);
	pNode->m_CornerIndex.push_back(dwBR);

	ComputeBoungigBox( pNode );

	ldiv_t divValue = ldiv( dwTL, m_dwWidth );
	pNode->m_dwPositionIndex[0] = divValue.rem / (dwTR-dwTL); // 나머지-> X
	pNode->m_dwPositionIndex[1] = divValue.quot / (dwTR-dwTL);//몫 -> Y

	if( pParentNode )
	{
		pNode->m_dwDepth = pParentNode->m_dwDepth + 1;		
		if((DWORD)m_iMaxDepth < pNode->m_dwDepth )
		{
			m_iMaxDepth = pNode->m_dwDepth;
		}
	}
	return pNode;
} 
TVector2	TQuadTreeIndex::GetHeightFromNode(	DWORD dwTL, DWORD dwTR, DWORD dwBL, DWORD dwBR )
{
	assert( m_pMap );

	DWORD dwStartRow = dwTL / m_dwWidth;
	DWORD dwEndRow    = dwBL / m_dwWidth;

	DWORD dwStartCol = dwTL % m_dwWidth;
	DWORD dwEndCol   = dwTR % m_dwWidth;

	TVector2 vHeight;
	vHeight.x = -999999.0f;
	vHeight.y = 999999.0f;
	for( DWORD dwRow = dwStartRow; dwRow < dwEndRow; dwRow++ )
	{
		for( DWORD dwCol = dwStartCol; dwCol < dwEndCol; dwCol++ )
		{
			if( m_pMap->m_VertexList[dwRow * m_dwWidth + dwCol].p.y > vHeight.x ) 
			{
				vHeight.x = m_pMap->m_VertexList[dwRow * m_dwWidth + dwCol].p.y;
			}				
			if( m_pMap->m_VertexList[dwRow * m_dwWidth + dwCol].p.y < vHeight.y ) 
			{
				vHeight.y = m_pMap->m_VertexList[dwRow * m_dwWidth + dwCol].p.y;
			}	
		}
	}
	return vHeight;
}
// pNode에 대한 방사형 LOD 패치 인덱스 버퍼 실시간 업데이트
int TQuadTreeIndex::UpdateIndexList(TNode* pNode, DWORD dwCurentIndex, DWORD dwNumLevel)
{
	if (!m_IndexList) return false;
	int iNumFaces = 0;

	DWORD dwTL = pNode->m_CornerIndex[0];
	DWORD dwTR = pNode->m_CornerIndex[1];
	DWORD dwBL = pNode->m_CornerIndex[2];
	DWORD dwBR = pNode->m_CornerIndex[3];

	/*ldiv_t divValue = ldiv( dwTL, m_dwWidth );
	DWORD dwStartRow = divValue.quot;
	DWORD dwStartCol = divValue.rem;

	divValue = ldiv( dwBL, m_dwWidth );
	DWORD dwEndRow = divValue.quot;

	divValue = ldiv( dwTR, m_dwWidth );
	DWORD dwEndCol = divValue.rem;*/

	DWORD dwStartRow = dwTL / m_dwWidth;
	DWORD dwEndRow = dwBL / m_dwWidth;

	DWORD dwStartCol = dwTL % m_dwWidth;
	DWORD dwEndCol = dwTR % m_dwWidth;

	DWORD dwOffset = (DWORD)(pow(2.0f, (float)dwNumLevel));
	DWORD dwCountX = ((dwEndCol - dwStartCol) / dwOffset) - 1;
	DWORD dwCountY = ((dwEndRow - dwStartRow) / dwOffset) - 1;


	DWORD dwYCell = 0;
	for (DWORD dwRow = dwStartRow; dwRow < dwEndRow; dwRow += dwOffset, dwYCell++)
	{
		DWORD dwXCell = 0;
		for (DWORD dwCol = dwStartCol; dwCol < dwEndCol; dwCol += dwOffset, dwXCell++)
		{
			//0	1    4   
			//2	   3 5
			DWORD dwNextRow = dwRow + dwOffset;
			DWORD dwNextCol = dwCol + dwOffset;
			if ((dwXCell == 0 && dwYCell == 0))
			{
				DWORD dwType = (pNode->m_dwLodType & 8) + (pNode->m_dwLodType & 1);
				iNumFaces += SetLodIndexBuffer(pNode, dwCurentIndex,
					dwNextRow * m_dwWidth + dwCol,	// 2
					dwRow * m_dwWidth + dwCol,		// 0
					dwRow * m_dwWidth + dwNextCol,	// 1											
					dwType);
			}
			else if ((dwXCell == 0) && (pNode->m_dwLodType & 8))
			{
				iNumFaces += SetLodIndexBuffer(pNode, dwCurentIndex,
					dwNextRow * m_dwWidth + dwCol,	// 2
					dwRow * m_dwWidth + dwCol,		// 0
					dwRow * m_dwWidth + dwNextCol,	// 1											
					8);
			}
			else if ((dwYCell == 0) && (pNode->m_dwLodType & 1))
			{
				iNumFaces += SetLodIndexBuffer(pNode, dwCurentIndex,
					dwNextRow * m_dwWidth + dwCol,	// 2
					dwRow * m_dwWidth + dwCol,		// 0
					dwRow * m_dwWidth + dwNextCol,	// 1											
					1);
			}
			else
			{
				m_IndexList[dwCurentIndex + 0] = dwNextRow * m_dwWidth + dwCol;
				m_IndexList[dwCurentIndex + 1] = dwRow * m_dwWidth + dwCol;
				m_IndexList[dwCurentIndex + 2] = dwRow * m_dwWidth + dwNextCol;
				iNumFaces += 1;
				dwCurentIndex += 3;
			}



			if ((dwXCell == dwCountX && dwYCell == dwCountY))
			{
				DWORD dwType = (pNode->m_dwLodType & 2) + (pNode->m_dwLodType & 4);
				iNumFaces += SetLodIndexBuffer(pNode, dwCurentIndex,
					dwRow * m_dwWidth + dwNextCol,		// 2
					dwNextRow * m_dwWidth + dwNextCol,	// 3
					dwNextRow * m_dwWidth + dwCol,		// 0																					
					dwType);
			}
			else if ((dwXCell == dwCountX) && (pNode->m_dwLodType & 2))
			{
				iNumFaces += SetLodIndexBuffer(pNode, dwCurentIndex,
					dwRow * m_dwWidth + dwNextCol,		// 2
					dwNextRow * m_dwWidth + dwNextCol,	// 3
					dwNextRow * m_dwWidth + dwCol,		// 0																					
					2);
			}
			else if ((dwYCell == dwCountY) && (pNode->m_dwLodType & 4))
			{
				iNumFaces += SetLodIndexBuffer(pNode, dwCurentIndex,
					dwRow * m_dwWidth + dwNextCol,		// 2
					dwNextRow * m_dwWidth + dwNextCol,	// 3
					dwNextRow * m_dwWidth + dwCol,		// 0																					
					4);
			}
			else
			{
				m_IndexList[dwCurentIndex + 0] = dwRow * m_dwWidth + dwNextCol;
				m_IndexList[dwCurentIndex + 1] = dwNextRow * m_dwWidth + dwNextCol;
				m_IndexList[dwCurentIndex + 2] = dwNextRow * m_dwWidth + dwCol;
				iNumFaces += 1;
				dwCurentIndex += 3;
			}
		}
	}
	return iNumFaces;
}
// 2, 0 ( 좌, 상 )
// 3, 1 ( 우, 하 )
int  TQuadTreeIndex::SetLodIndexBuffer(TNode* pNode,
	DWORD& dwCurentIndex,
	DWORD dwA, DWORD dwB, DWORD dwC,
	DWORD dwType)
{
	int iNumFaces = 0;

	if (dwType == 0)
	{
		m_IndexList[dwCurentIndex + 0] = dwA;
		m_IndexList[dwCurentIndex + 1] = dwB;
		m_IndexList[dwCurentIndex + 2] = dwC;
		iNumFaces += 1;
		dwCurentIndex += 3;
		return iNumFaces;
	}
	if (dwType == 8 || dwType == 2)// 좌우
	{
		DWORD dwCenter = (dwA + dwB) / 2;
		m_IndexList[dwCurentIndex + 0] = dwC;
		m_IndexList[dwCurentIndex + 1] = dwA;
		m_IndexList[dwCurentIndex + 2] = dwCenter;
		m_IndexList[dwCurentIndex + 3] = dwC;
		m_IndexList[dwCurentIndex + 4] = dwCenter;
		m_IndexList[dwCurentIndex + 5] = dwB;
		iNumFaces += 2;
		dwCurentIndex += 6;

		return iNumFaces;
	}
	if (dwType == 1 || dwType == 4)// 상하
	{
		DWORD dwCenter = (dwB + dwC) / 2;
		m_IndexList[dwCurentIndex + 0] = dwA;
		m_IndexList[dwCurentIndex + 1] = dwB;
		m_IndexList[dwCurentIndex + 2] = dwCenter;
		m_IndexList[dwCurentIndex + 3] = dwA;
		m_IndexList[dwCurentIndex + 4] = dwCenter;
		m_IndexList[dwCurentIndex + 5] = dwC;
		iNumFaces += 2;
		dwCurentIndex += 6;
		return iNumFaces;
	}
	if (dwType == 9 || dwType == 6)// 좌상, 우하
	{
		DWORD dwTopCenter = (dwB + dwC) / 2;
		DWORD dwLeftCenter = (dwA + dwB) / 2;

		m_IndexList[dwCurentIndex + 0] = dwLeftCenter;
		m_IndexList[dwCurentIndex + 1] = dwB;
		m_IndexList[dwCurentIndex + 2] = dwTopCenter;
		m_IndexList[dwCurentIndex + 3] = dwLeftCenter;
		m_IndexList[dwCurentIndex + 4] = dwTopCenter;
		m_IndexList[dwCurentIndex + 5] = dwA;

		m_IndexList[dwCurentIndex + 6] = dwA;
		m_IndexList[dwCurentIndex + 7] = dwTopCenter;
		m_IndexList[dwCurentIndex + 8] = dwC;
		iNumFaces += 3;
		dwCurentIndex += 9;
		return iNumFaces;
	}
	return iNumFaces;
}
bool TQuadTreeIndex::Release()
{	
	for( int iBuffer = 0;  iBuffer < m_pdwLodIndexArray.size(); iBuffer++ )
	{
		SAFE_DEL( m_pdwLodIndexArray[iBuffer] );		
	}
	m_pdwLodIndexArray.clear();

	SAFE_DELETE_ARRAY( m_IndexList );

	return TQuadTree::Release();
}

bool TQuadTreeIndex::Frame()
{	
	m_DrawPatchNodeList.clear();
	// PreFrame함수 호출 및 오브젝트 컬링
	if( !TQuadTree::Frame() ) return false;

	// 카메라에 보이는 노드리스트에서 리프노드(패치단위)의 노드를 얻는다.
	GetDrawPatchNode();

	///////////////////// 실시간 인덱스 리스트를 갱신한다. ///////////////////
	// m_bDynamicUpdateIB= true일 경우에는 노드마다 정적 LOD 인덱스버퍼를 사용하지 않고
	// 직접 m_IndexList를 실시간 갱신하게 된다. 이후 맵에서 이를 사용하여 랜더링한다.
	// 하지만 StaticLod-1 샘플에서는 m_bDynamicUpdateIB=flase로 
	// 실시간 갱신없이 노드 랜더링시 해당 정적 LOD 인덱스 버퍼를 사용하여 랜더링하게 된다.
	if(m_bDynamicUpdateIB)
	{
		m_iNumFace = 0;
		for (int iNode = 0; iNode < m_DrawPatchNodeList.size(); iNode++)
		{
			for (int iNeighbor = 0; iNeighbor < m_DrawPatchNodeList[iNode]->m_NeighborList.size(); iNeighbor++)
			{
				if (m_DrawPatchNodeList[iNode]->m_NeighborList[iNeighbor])
				{
					DWORD dwValue = (DWORD)(abs((float)
						(m_DrawPatchNodeList[iNode]->m_NeighborList[iNeighbor]->m_dwLodLevel -
							m_DrawPatchNodeList[iNode]->m_dwLodLevel)));
					if (dwValue > 1)
					{
						int kkkk = 0;
					}
				}
			}

			GetLodType(m_DrawPatchNodeList[iNode]);
			if (m_bUsedCrossTypeIndex)
			{
				m_iNumFace += UpdateStaticIndexList(m_DrawPatchNodeList[iNode], 
					m_iNumFace * 3,
					m_DrawPatchNodeList[iNode]->m_dwLodLevel);
			}
			else
			{
				m_iNumFace += UpdateIndexList(m_DrawPatchNodeList[iNode], 
					m_iNumFace * 3, 
					m_DrawPatchNodeList[iNode]->m_dwLodLevel);
			}
		}
		//m_pMap->UpdateIndexBuffer( m_IndexList, m_iNumFace);
	}
	return true;
}
int TQuadTreeIndex::UpdateStaticIndexList( TNode* pNode, DWORD dwCurentIndex, DWORD dwLod  )
{
	if( !m_IndexList ) return false;	
	int iNumFaces = 0;
	
	DWORD dwTL = pNode->m_CornerIndex[0];
	DWORD dwTR = pNode->m_CornerIndex[1];
	DWORD dwBL = pNode->m_CornerIndex[2];
	DWORD dwBR = pNode->m_CornerIndex[3];

	DWORD dwIndexCnt = (DWORD)(m_pdwLodIndexArray[dwLod]->IndexList[pNode->m_dwLodType].size());
	for( DWORD dwIndex = 0; dwIndex < dwIndexCnt; dwIndex+=3 )
	{
		DWORD dw0  = GetIndex(m_pdwLodIndexArray[dwLod]->IndexList[pNode->m_dwLodType][dwIndex+0], dwTL, dwTR, dwBL,dwBR, dwLod );
		DWORD dw1  = GetIndex(m_pdwLodIndexArray[dwLod]->IndexList[pNode->m_dwLodType][dwIndex+1], dwTL, dwTR, dwBL,dwBR, dwLod );
		DWORD dw2  = GetIndex(m_pdwLodIndexArray[dwLod]->IndexList[pNode->m_dwLodType][dwIndex+2], dwTL, dwTR, dwBL,dwBR, dwLod );

		m_IndexList[dwCurentIndex+0] = dw0;
		m_IndexList[dwCurentIndex+1] = dw1;
		m_IndexList[dwCurentIndex+2] = dw2;
		iNumFaces += 1;
		dwCurentIndex+=3;	

	}	
	return iNumFaces;
}


DWORD TQuadTreeIndex::GetIndexCounter( DWORD dwData, DWORD dwNumCell )
{	
	// 총 개수 = (셀 가로 개수 * 셀 세로 개수  * 4 )  +  
	//            셀 가로 개수 및 셀 세로 개수 * dwLevelCount ) * 3;
	// dwLevelCount는  8,4,2,1 방향으로 각각 추가 될수 있는 페이스
	DWORD	dwNumFaces = (dwNumCell*dwNumCell*4 );

	DWORD	dwLevelCount = 0;
	if( dwData & 8 ) dwLevelCount++;
	if( dwData & 4 ) dwLevelCount++;
	if( dwData & 2 ) dwLevelCount++;
	if( dwData & 1 ) dwLevelCount++;

	dwNumFaces = ( dwNumFaces + (dwNumCell*dwLevelCount) );
	return dwNumFaces*3;
}
// 십자형 LOD 패치 계산
bool TQuadTreeIndex::ComputeStaticLodIndex( int iMaxCells )
{
	m_pdwLodIndexArray.reserve(m_iPatchLodCount+1);	

	// 레벨 0
	TStaticData*  pStaticData = NULL;
	SAFE_NEW( pStaticData, TStaticData );
	pStaticData->dwLevel	= 0;
	pStaticData->IndexList.resize(1);

	for( DWORD dwRow = 0; dwRow < (DWORD)iMaxCells; dwRow++ )
	{
		for( DWORD dwCol = 0; dwCol < (DWORD)iMaxCells; dwCol++ )
		{
			//0	1    4   
			//2	   3 5
			DWORD dwNextRow = dwRow+1;
			DWORD dwNextCol = dwCol+1;
			pStaticData->IndexList[0].push_back( dwRow * (iMaxCells+1) + dwCol);
			pStaticData->IndexList[0].push_back( dwRow * (iMaxCells+1) + dwNextCol);
			pStaticData->IndexList[0].push_back( dwNextRow * (iMaxCells+1) + dwCol);
			pStaticData->IndexList[0].push_back( dwNextRow * (iMaxCells+1) + dwCol);
			pStaticData->IndexList[0].push_back( dwRow * (iMaxCells+1) + dwNextCol);
			pStaticData->IndexList[0].push_back( dwNextRow * (iMaxCells+1) + dwNextCol);	
		}
	}
	m_pdwLodIndexArray.push_back( pStaticData );
	CreateIndexBuffer( pStaticData );

	// 레벨 1부터 ~  m_iPatchLodCount+1
	for( DWORD dwLevel=0; dwLevel < (DWORD)m_iPatchLodCount; dwLevel++ )
	{
		// 최소 패치의 크기가( 0 - 1 - 2 ) 정점 3개 이기 때문에 레벨=0 일 경우 오프셋=2이어야 한다.
		DWORD dwOffset= (DWORD)(pow( 2.0f, float(dwLevel)+1));
		DWORD dwNumCell =  ( iMaxCells/dwOffset );		

		TStaticData*  pStaticData = NULL;
		SAFE_NEW( pStaticData, TStaticData );
		pStaticData->dwLevel	= dwLevel+1;
		pStaticData->IndexList.resize(16);

		for( int iData=0; iData < 16; iData++ )
		{
			DWORD dwIndexCounter = GetIndexCounter(iData, dwNumCell);
			pStaticData->IndexList[iData].resize( dwIndexCounter );
			int iIndex = 0;

			for( DWORD dwRow = 0; dwRow < (DWORD)iMaxCells; dwRow+=dwOffset )
			{
				for( DWORD dwCol = 0; dwCol < (DWORD)iMaxCells; dwCol+=dwOffset )
				{				
					DWORD dwTL	= dwRow * (iMaxCells+1) + dwCol;
					DWORD dwTR	= dwTL + dwOffset;
					DWORD dwBL	= dwOffset * (iMaxCells+1) + dwTL;
					DWORD dwBR	= dwBL + dwOffset; 
					DWORD dwCP	= (dwTL + dwBR ) / 2;

					if( iData != 0 &&  
						( dwRow == 0 || 
						  dwRow == ( (dwNumCell-1)*dwOffset) || 
						  dwCol == 0 || 
						  dwCol == ( (dwNumCell-1)*dwOffset)   ) 
					  )
					{
						if( ( iData & 1)  && dwRow == 0 )
						{
							DWORD dwUpperCenter = ( dwTL + dwTR ) / 2;
							pStaticData->IndexList[iData][iIndex++]=dwCP;
							pStaticData->IndexList[iData][iIndex++]=dwTL;
							pStaticData->IndexList[iData][iIndex++]=dwUpperCenter;
							pStaticData->IndexList[iData][iIndex++]=dwCP;
							pStaticData->IndexList[iData][iIndex++]=dwUpperCenter;
							pStaticData->IndexList[iData][iIndex++]=dwTR;	
						}
						else
						{
							pStaticData->IndexList[iData][iIndex++]=dwCP;
							pStaticData->IndexList[iData][iIndex++]=dwTL;
							pStaticData->IndexList[iData][iIndex++]=dwTR;
						}
						if( (iData & 2 ) &&  ( dwCol == (dwNumCell-1)*dwOffset )  )
						{
							DWORD dwRightCenter = ( dwTR + dwBR ) / 2;
							pStaticData->IndexList[iData][iIndex++]=dwCP;
							pStaticData->IndexList[iData][iIndex++]=dwTR;
							pStaticData->IndexList[iData][iIndex++]=dwRightCenter;
							pStaticData->IndexList[iData][iIndex++]=dwCP;
							pStaticData->IndexList[iData][iIndex++]=dwRightCenter;
							pStaticData->IndexList[iData][iIndex++]=dwBR;	
						}
						else
						{
							pStaticData->IndexList[iData][iIndex++]=dwCP;
							pStaticData->IndexList[iData][iIndex++]=dwTR;
							pStaticData->IndexList[iData][iIndex++]=dwBR;		
						}
						if( ( iData & 4 ) &&  ( dwRow == (dwNumCell-1)*dwOffset ))
						{
							DWORD dwLowerCenter = ( dwBR + dwBL ) / 2;
							pStaticData->IndexList[iData][iIndex++]=dwCP;
							pStaticData->IndexList[iData][iIndex++]=dwBR;
							pStaticData->IndexList[iData][iIndex++]=dwLowerCenter;
							pStaticData->IndexList[iData][iIndex++]=dwCP;
							pStaticData->IndexList[iData][iIndex++]=dwLowerCenter;
							pStaticData->IndexList[iData][iIndex++]=dwBL;
						}
						else
						{
							pStaticData->IndexList[iData][iIndex++]=dwCP;
							pStaticData->IndexList[iData][iIndex++]=dwBR;
							pStaticData->IndexList[iData][iIndex++]=dwBL;
						}
						if( ( iData & 8 ) &&  ( dwCol == 0 ) )
						{
							DWORD dwLeftCenter = ( dwTL + dwBL ) / 2;
							pStaticData->IndexList[iData][iIndex++]=dwCP;
							pStaticData->IndexList[iData][iIndex++]=dwBL;
							pStaticData->IndexList[iData][iIndex++]=dwLeftCenter;
							pStaticData->IndexList[iData][iIndex++]=dwCP;
							pStaticData->IndexList[iData][iIndex++]=dwLeftCenter;
							pStaticData->IndexList[iData][iIndex++]=dwTL;
						}			
						else
						{
							pStaticData->IndexList[iData][iIndex++]=dwCP;
							pStaticData->IndexList[iData][iIndex++]=dwBL;
							pStaticData->IndexList[iData][iIndex++]=dwTL;	
						}
					}
					else
					{
						pStaticData->IndexList[iData][iIndex++]=dwCP;
						pStaticData->IndexList[iData][iIndex++]=dwTL;
						pStaticData->IndexList[iData][iIndex++]=dwTR;

						pStaticData->IndexList[iData][iIndex++]=dwCP;
						pStaticData->IndexList[iData][iIndex++]=dwTR;
						pStaticData->IndexList[iData][iIndex++]=dwBR;			

						pStaticData->IndexList[iData][iIndex++]=dwCP;
						pStaticData->IndexList[iData][iIndex++]=dwBR;
						pStaticData->IndexList[iData][iIndex++]=dwBL;

						pStaticData->IndexList[iData][iIndex++]=dwCP;
						pStaticData->IndexList[iData][iIndex++]=dwBL;
						pStaticData->IndexList[iData][iIndex++]=dwTL;						
					}
				}
			}
			
		}
		CreateIndexBuffer( pStaticData );
		m_pdwLodIndexArray.push_back( pStaticData );
	}
	return true;
}

DWORD TQuadTreeIndex::GetIndex( DWORD dwIndex, DWORD dwTL, DWORD dwTR, DWORD dwBL, DWORD dwBR, DWORD dwLod  )
{
	DWORD dwReturn = 0;
	DWORD dwStartRow = dwTL / m_dwWidth;
	DWORD dwStartCol = dwTL % m_dwWidth;

	DWORD dwRow = dwIndex / (int)(pow(2.0f, (float)m_iPatchLodCount ) +1);
	DWORD dwCol = dwIndex % (int)(pow(2.0f, (float)m_iPatchLodCount )+1);

	dwReturn = (dwStartRow + dwRow ) * m_dwWidth + dwStartCol+dwCol;
	return dwReturn; 
}

HRESULT TQuadTreeIndex::CreateIndexBuffer( TStaticData*  pData )
{	
	HRESULT hr=S_OK;
	pData->IndexBufferList.resize(pData->IndexList.size());
	for( int iData=0; iData < pData->IndexList.size(); iData++ )
	{
		if( pData->IndexList[iData].size() <= 0 ) break;
		// 정점 인덱스 버퍼 생성
		pData->IndexBufferList[iData].Attach(DX::CreateIndexBuffer(m_pd3dDevice,
			&pData->IndexList[iData].at(0),
			(UINT)pData->IndexList[iData].size(),
			sizeof(DWORD)));
	}
	return hr;
}
HRESULT TQuadTreeIndex::CreateVertexBuffer(TNode* pNode, DWORD dwCount, PNCT_VERTEX* pvHeightMap)
{	
	HRESULT hr=S_OK;
	if (dwCount <= 0) return S_OK;
	void** pData = nullptr;
	if (dwCount > 0) pData = (void**)pvHeightMap;

	pNode->m_pVertexBuffer.Attach(DX::CreateVertexBuffer(m_pd3dDevice,
		pData,
		dwCount,
		sizeof(PNCT_VERTEX)));

	return hr;
}
bool TQuadTreeIndex::CreateVertexList(TNode* pNode)
{
	DWORD dwTL = pNode->m_CornerIndex[0];
	DWORD dwTR = pNode->m_CornerIndex[1];
	DWORD dwBL = pNode->m_CornerIndex[2];
	DWORD dwBR = pNode->m_CornerIndex[3];

	DWORD dwSize = ((dwTR-dwTL)+1 )*((dwTR-dwTL)+1);
	PNCT_VERTEX* pvHeightMap = 0;
	SAFE_NEW_ARRAY( pvHeightMap, PNCT_VERTEX, dwSize );
	

	DWORD dwStartRow = dwTL / m_dwWidth;
	DWORD dwEndRow    = dwBL / m_dwWidth;

	DWORD dwStartCol = dwTL % m_dwWidth;
	DWORD dwEndCol   = dwTR % m_dwWidth;
	
	DWORD iVertexIndex = 0;
	for( DWORD dwRow = dwStartRow; dwRow <= dwEndRow; dwRow++ )
	{		
		for( DWORD dwCol = dwStartCol; dwCol <= dwEndCol; dwCol++  )
		{			
			pvHeightMap[iVertexIndex++] = m_pMap->m_VertexList[dwRow*m_dwWidth+dwCol];
		}
	}
	if( FAILED( CreateVertexBuffer( pNode, iVertexIndex, pvHeightMap ) ))
	{
		return false;
	}
	SAFE_DELETE_ARRAY( pvHeightMap );
	return true;
}

bool TQuadTreeIndex::Render(ID3D11DeviceContext*    pContext)
{
	m_pMap->PreRender(g_pImmediateContext);
	UINT stride = sizeof(PNCT_VERTEX);
	UINT offset = 0;

	for( int iNode = 0; iNode < m_DrawPatchNodeList.size(); iNode++ )
	{
		TNode* pNode = m_DrawPatchNodeList[iNode];	
		GetLodType( pNode );
		
		pContext->IASetVertexBuffers(0, 1, pNode->m_pVertexBuffer.GetAddressOf(), &stride, &offset);
		//if (m_bUsedCrossTypeIndex)
		{
			pContext->IASetIndexBuffer(m_pdwLodIndexArray[pNode->m_dwLodLevel]->IndexBufferList[pNode->m_dwLodType].Get()
				, DXGI_FORMAT_R32_UINT, 0);

			pContext->UpdateSubresource(m_pMap->m_dxobj.g_pConstantBuffer.Get(), 0, NULL, &m_pMap->m_cbData, 0, 0);
			pContext->DrawIndexed((UINT)m_pdwLodIndexArray[pNode->m_dwLodLevel]->IndexList[pNode->m_dwLodType].size(), 0, 0);
		}	
	}
	return true;
}

TQuadTreeIndex::TQuadTreeIndex(void)
{
	m_fExpansionRatio = 0.1f;
	m_fMinDivideSize = 1;
	m_iNumFace = 0;
	m_iMinSplitDistance = 2;

	m_bDynamicUpdateIB = true;
	m_bUsedCrossTypeIndex = false;
	SAFE_ZERO(m_pMap);
	SAFE_ZERO(m_IndexList);
}

TQuadTreeIndex::~TQuadTreeIndex(void)
{
}