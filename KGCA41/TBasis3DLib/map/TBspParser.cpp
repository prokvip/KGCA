#include "TBspParser.h"
bool TBspParser::LoadTexture()	
{    
	//TCHAR szFilePath[MAX_PATH] = TEXT("");
	//_tcscpy( szFilePath, I_Register.m_szDefaultResPath);	
	//lstrcat( szFilePath, _T(""));	
	DWORD dwIndex = 0;
	dwIndex =  I_Texture.Add(g_pd3dDevice, Res(L"obj/front.bmp").c_str());
	if(dwIndex <= 0 )
	{
		MessageBox( 0, _T("LoadTexture  실패"), _T("Fatal error"), MB_OK );		
		return false;
	}
	dwIndex = I_Texture.Add(g_pd3dDevice, Res(L"obj/back.bmp").c_str());
	if(dwIndex <= 0 )
	{
		MessageBox( 0, _T("LoadTexture  실패"), _T("Fatal error"), MB_OK );		
		return false;
	}
	return true;
}
bool	TBspParser::Load( ID3D11Device*  pd3dDevice,  TNode* pRootNode, TCHAR* szFileName, bool Binary )
{
	SetDevice( pd3dDevice );
	TCHAR szFilePath[MAX_PATH] = TEXT("");		
	/*_tcscpy( szFilePath, I_Register.m_szDefaultResPath);
	lstrcat( szFilePath, _T("\\") );*/
	lstrcat( szFilePath, szFileName );

	(Binary) ? m_pStream = _tfopen(szFilePath, _T("rb") ) : m_pStream = _tfopen(szFilePath, _T("rt") );	
	if( m_pStream )
	{
		SetBinary( Binary);
		
		pRootNode->m_ChildList.resize(2);
		ReadBspTreeFile( pRootNode );
		fclose( m_pStream );
		LoadTexture();				
		return true;
	}	
	return false;	
}
bool	TBspParser::Save( TNode* pRootNode, TCHAR* szFileName, bool Binary )
{
	TCHAR szFilePath[MAX_PATH] = TEXT("");		
	/*_tcscpy( szFilePath, I_Register.m_szDefaultResPath);
	lstrcat( szFilePath, _T("\\") );*/
	lstrcat( szFilePath, szFileName );
	(Binary) ? m_pStream = _tfopen(szFilePath, _T("wb")	 ) : m_pStream = _tfopen(szFilePath, _T("wt") );	
	if( m_pStream )
	{
		SetBinary( Binary);
		WriteBspTreeFile( pRootNode );
		fclose( m_pStream );		
		return true;
	}	
	return false;	
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
void TBspParser::ReadBspTreeFile( TNode*  pBspTree )
{
	if( feof( m_pStream ) )
		return;
	TNode*	 pNode = pBspTree; 
	if( !m_bBinary )
		ReadBspNode(pNode);
	else
		BinaryReadBspNode( pNode );

	if( pNode->m_isLeaf == TRUE ) SetNodePerRenderGroup(pNode);
	if( pNode->m_isLeaf == 1 || pNode->m_isLeaf == 3 )
	{		
		return;
	}	
	if( !pNode->m_ChildList[0] )
	{
		pNode->m_ChildList[0] = new TNode;
		pNode->m_ChildList[0]->m_ChildList.resize(2);
		ReadBspTreeFile( pNode->m_ChildList[0] );
	}
	if( !pNode->m_ChildList[1] )
	{
		pNode->m_ChildList[1] = new TNode;
		pNode->m_ChildList[1]->m_ChildList.resize(2);
		ReadBspTreeFile( pNode->m_ChildList[1]  );
	}	
}
void TBspParser::WriteBspTreeFile( TNode*  pBspTree  )
{	
	if( !pBspTree )
		return;
	TNode*	 pNode = pBspTree; 
	if( !m_bBinary )
		WriteBspNode( pNode );
	else
		BinaryWriteBspNode( pNode );

	if( pNode->m_ChildList.size() )
	{
		WriteBspTreeFile( pNode->m_ChildList[0]);
		WriteBspTreeFile( pNode->m_ChildList[1]);	
	}
}

void TBspParser::ReadBspNode( TNode*  pNode )
{	
	TCHAR pCommont[4];
	int  iCnt;
	
	_fgetts( m_pBuffer, 256, m_pStream );			
	_stscanf(m_pBuffer, _T("%s%d"), pCommont, &iCnt);

	_fgetts( m_pBuffer, 256, m_pStream );	
	_stscanf( m_pBuffer, _T("%d %f%f%f %f%f%f %f%f%f  %f%f%f     %f%f%f %f%f%f  %f%f%f %d"),
		&pNode->m_isLeaf, 
		&pNode->m_tBox.vMax.x,		&pNode->m_tBox.vMax.y,		&pNode->m_tBox.vMax.z,
		&pNode->m_tBox.vMin.x,		&pNode->m_tBox.vMin.y,		&pNode->m_tBox.vMin.z,

		&pNode->m_tBox.vCenter.x,	&pNode->m_tBox.vCenter.y,	&pNode->m_tBox.vCenter.z,
		&pNode->m_tBox.fExtent[0],	&pNode->m_tBox.fExtent[1],	&pNode->m_tBox.fExtent[2],
		
		&pNode->m_tBox.vAxis[0].x,		&pNode->m_tBox.vAxis[0].y,		&pNode->m_tBox.vAxis[0].z,
		&pNode->m_tBox.vAxis[1].x,		&pNode->m_tBox.vAxis[1].y,		&pNode->m_tBox.vAxis[1].z,
		&pNode->m_tBox.vAxis[2].x,		&pNode->m_tBox.vAxis[2].y,		&pNode->m_tBox.vAxis[2].z,		

		&pNode->iNumPolygon);		

	if( pNode->m_isLeaf == 3 )
	{
		return;
	}
	for( int iPoly = 0; iPoly < pNode->iNumPolygon; iPoly++ )
	{
		TPolygon* pPoly;
		if( pNode->m_pSplitter == NULL )
		{
			SAFE_NEW( pNode->m_pSplitter, TPolygon )		 
				memset( pNode->m_pSplitter, 0, sizeof( TPolygon ) );
			pPoly = pNode->m_pSplitter;
		}
		else
		{				
			pPoly = pNode->m_pSplitter;
			for(;;)
			{
				if( pPoly->pNext == NULL )
				{
					SAFE_NEW( pPoly->pNext, TPolygon )		 
						memset( pPoly->pNext, 0, sizeof( TPolygon ) );
					break;
				}
				pPoly = pPoly->pNext;
			}
			pPoly = pPoly->pNext;
		}		

		_fgetts( m_pBuffer, 256, m_pStream );	
		int  bFlag = 0;
		_stscanf_s( m_pBuffer, _T("%d %d %d %f %f %f %d %s "),							
			&bFlag,
			&pPoly->iNumIndices, 
			&pPoly->iNumVertices,						
			&pPoly->vNormal.x,
			&pPoly->vNormal.y,
			&pPoly->vNormal.z,
			&pPoly->iTexIndex,
			pPoly->strTexName, (UINT)_countof(pPoly->strTexName)	);

		pPoly->bBeenUsedAsSplitter = bFlag;

		SAFE_NEW_ARRAY( pPoly->pIndices, DWORD, pPoly->iNumIndices )		 
			memset( pPoly->pIndices, 0, sizeof( DWORD ) * pPoly->iNumIndices );	

		_fgetts( m_pBuffer, 256, m_pStream );	
		_stscanf( m_pBuffer, _T("%d %d %d %d %d %d"), 
			&pPoly->pIndices[ 0 ],
			&pPoly->pIndices[ 1 ],
			&pPoly->pIndices[ 2 ],
			&pPoly->pIndices[ 3 ],
			&pPoly->pIndices[ 4 ],
			&pPoly->pIndices[ 5 ] );		


		SAFE_NEW_ARRAY( pPoly->pVertexList, PNCT_VERTEX, pPoly->iNumVertices )		 
			memset( pPoly->pVertexList, 0, sizeof( PNCT_VERTEX ) * pPoly->iNumVertices );	

		for( int iVer = 0; iVer < pPoly->iNumVertices; iVer++ )
		{
			_fgetts( m_pBuffer, 256, m_pStream );	
			_stscanf( m_pBuffer, _T("%f%f%f%f %f %f %f %f %f"),	
				&pPoly->pVertexList[iVer].c.x, &pPoly->pVertexList[iVer].c.y, &pPoly->pVertexList[iVer].c.z, &pPoly->pVertexList[iVer].c.w, 
				&pPoly->pVertexList[iVer].t.x, 
				&pPoly->pVertexList[iVer].t.y, 
				&pPoly->pVertexList[iVer].p.x, 
				&pPoly->pVertexList[iVer].p.y, 
				&pPoly->pVertexList[iVer].p.z );
		}	
	}
}

void TBspParser::BinaryReadBspNode( TNode*  pNode )
{	
	TCHAR pCommont[3];
	int  iCnt;	 

	fread( pCommont, sizeof(TCHAR)*3, 1, m_pStream );
	fread( &iCnt, sizeof(int), 1, m_pStream );	
	fread( &pNode->m_isLeaf, sizeof(int), 1, m_pStream );
	fread( &pNode->m_tBox, sizeof(T_BOX), 1, m_pStream );
	fread( &pNode->iNumPolygon, sizeof(int), 1, m_pStream );	
	if( pNode->m_isLeaf == 3 )
	{
		return;
	}
	for( int iPoly = 0; iPoly < pNode->iNumPolygon; iPoly++ )
	{
		TPolygon* pPoly;
		if( pNode->m_pSplitter == NULL )
		{
			SAFE_NEW( pNode->m_pSplitter, TPolygon )		 
			memset( pNode->m_pSplitter, 0, sizeof( TPolygon ) );
			pPoly = pNode->m_pSplitter;
		}
		else
		{				
			pPoly = pNode->m_pSplitter;
			for(;;)
			{
				if( pPoly->pNext == NULL )
				{
					SAFE_NEW( pPoly->pNext, TPolygon )		 
					memset( pPoly->pNext, 0, sizeof( TPolygon ) );
					break;
				}
				pPoly = pPoly->pNext;
			}
			pPoly = pPoly->pNext;
		}		

		fread( &pPoly->bBeenUsedAsSplitter, sizeof(int), 1, m_pStream );
		fread( &pPoly->iNumIndices, sizeof(DWORD), 1, m_pStream );
		fread( &pPoly->iNumVertices, sizeof(DWORD), 1, m_pStream );
		fread( &pPoly->vNormal, sizeof(TVector3), 1, m_pStream );		
		fread( &pPoly->iTexIndex, sizeof(int),    1, m_pStream );		
		fread( pPoly->strTexName, sizeof(TCHAR)*30, 1, m_pStream );	

		SAFE_NEW_ARRAY( pPoly->pIndices, DWORD, pPoly->iNumIndices )		 
		memset( pPoly->pIndices, 0, sizeof( DWORD ) * pPoly->iNumIndices );	
		fread( pPoly->pIndices, sizeof(DWORD)*pPoly->iNumIndices, 1 , m_pStream );		

		SAFE_NEW_ARRAY( pPoly->pVertexList, PNCT_VERTEX, pPoly->iNumVertices )		 
		memset( pPoly->pVertexList, 0, sizeof( PNCT_VERTEX ) * pPoly->iNumVertices );	
		fread( pPoly->pVertexList, sizeof(PNCT_VERTEX)*pPoly->iNumVertices,    1, m_pStream );		
	}
}

void TBspParser::BinaryWriteBspNode( TNode*  pNode )
{	
	//  Node Info
	//		Polygon Info
	//			pPolygon VertexList
	static int iNode = 0;
	
	TCHAR buffer[3] = _T("//") ;	
	fwrite( buffer, sizeof(TCHAR)*3, 1, m_pStream ); 
	fwrite( &iNode, sizeof(int),    1, m_pStream );
	fwrite( &pNode->m_isLeaf,	sizeof(int),    1, m_pStream );
	fwrite( &pNode->m_tBox, sizeof(T_BOX),    1, m_pStream );	
	fwrite( &pNode->iNumPolygon,  sizeof(int),    1, m_pStream );	

	for( TPolygon* pPoly = pNode->m_pSplitter; pPoly; pPoly = pPoly->pNext )
	{	
		//fwrite( &pPoly->iNumMtrl, sizeof(int),    1, m_pStream );
		//fwrite( &pPoly->m_MtrlList, sizeof(NodeMtrlInfo),    1, m_pStream );	
		fwrite( &pPoly->bBeenUsedAsSplitter,	sizeof(int),    1, m_pStream );
		fwrite( &pPoly->iNumIndices, sizeof(DWORD),    1, m_pStream );
		fwrite( &pPoly->iNumVertices, sizeof(DWORD),    1, m_pStream );
		fwrite( &pPoly->vNormal, sizeof(TVector3),    1, m_pStream );		
		fwrite( &pPoly->iTexIndex, sizeof(int),    1, m_pStream );		
		fwrite( pPoly->strTexName, sizeof(TCHAR)*30,    1, m_pStream );
		fwrite( pPoly->pIndices, sizeof(DWORD)*pPoly->iNumIndices,    1, m_pStream );		
		fwrite( pPoly->pVertexList, sizeof(PNCT_VERTEX)*pPoly->iNumVertices,    1, m_pStream );
        if( pNode->m_isLeaf != 1 )
		{
			break;
		}
	}
	iNode++;
}

void TBspParser::WriteBspNode( TNode*  pNode )
{	
	//  Node Info
	//		Polygon Info
	//			pPolygon VertexList
	static int iNode = 0;
	_ftprintf( m_pStream,_T("%s %d\n"),_T("//"), iNode++ );
	
	_ftprintf( m_pStream, _T("%d %f %f %f %f %f %f %f %f %f  %f %f %f     %f %f %f %f %f %f  %f %f %f %d\n"),
		pNode->m_isLeaf, 
		pNode->m_tBox.vMax.x,		pNode->m_tBox.vMax.y,		pNode->m_tBox.vMax.z,
		pNode->m_tBox.vMin.x,		pNode->m_tBox.vMin.y,		pNode->m_tBox.vMin.z,

		pNode->m_tBox.vCenter.x,	pNode->m_tBox.vCenter.y,	pNode->m_tBox.vCenter.z,
		pNode->m_tBox.fExtent[0],	pNode->m_tBox.fExtent[1],	pNode->m_tBox.fExtent[2],
		
		pNode->m_tBox.vAxis[0].x,	pNode->m_tBox.vAxis[0].y,	pNode->m_tBox.vAxis[0].z,
		pNode->m_tBox.vAxis[1].x,	pNode->m_tBox.vAxis[1].y,	pNode->m_tBox.vAxis[1].z,
		pNode->m_tBox.vAxis[2].x,	pNode->m_tBox.vAxis[2].y,	pNode->m_tBox.vAxis[2].z,		

		pNode->iNumPolygon);		

	for( TPolygon* pPoly = pNode->m_pSplitter; pPoly; pPoly = pPoly->pNext )
	{	
		_ftprintf_s( m_pStream, _T("\t%d %d %d %f %f %f %d %s\n"),
			pPoly->bBeenUsedAsSplitter, 
			pPoly->iNumIndices, 
			pPoly->iNumVertices,						
			pPoly->vNormal.x,
			pPoly->vNormal.y,
			pPoly->vNormal.z,
			pPoly->iTexIndex,
			pPoly->strTexName );

		for( int iInd = 0; iInd < pPoly->iNumIndices; iInd++ )
		{
			if( iInd == pPoly->iNumIndices -1 )
				_ftprintf( m_pStream, _T(" %d\n"), pPoly->pIndices[iInd] );
			else if( iInd == 0 )
				_ftprintf( m_pStream, _T("\t\t %d"), pPoly->pIndices[iInd] );
			else
				_ftprintf( m_pStream, _T(" %d"), pPoly->pIndices[iInd] );
		}

		for( int iVer = 0; iVer < pPoly->iNumVertices; iVer++ )
		{
			_ftprintf( m_pStream, _T("\t\t\t%f %f %f %f %f %f %f %f %f\n"),	
				pPoly->pVertexList[iVer].c.x, pPoly->pVertexList[iVer].c.y, pPoly->pVertexList[iVer].c.z, pPoly->pVertexList[iVer].c.w, 
				pPoly->pVertexList[iVer].t.x, 
				pPoly->pVertexList[iVer].t.y, 
				pPoly->pVertexList[iVer].p.x, 
				pPoly->pVertexList[iVer].p.y, 
				pPoly->pVertexList[iVer].p.z );

		}			
		if( pNode->m_isLeaf != 1 )
		{
			break;
		}
	}
}
void TBspParser::SetNodePerRenderGroup(TNode* pNode)
{
	if( pNode == NULL ) return;	

	if( pNode->m_isLeaf == TRUE )
	{	
		int iMtrlCnt = 0;
		typedef std::set< int > LeafPerNumTexture;	
		LeafPerNumTexture LeafTex;
		multiset<int>  PolygonList;
 
		for( TPolygon* pPoly = pNode->m_pSplitter; pPoly; pPoly = pPoly->pNext )
		{	
			for( int iface = 0; iface < pPoly->iNumIndices/3;iface++ )
			{
				if(pPoly->iTexIndex >= 0 )//|| pPoly->iTexIndex = I_Texture.Add(g_pd3dDevice, pPoly->strTexName))
				{
					LeafTex.insert(pPoly->iTexIndex);
					PolygonList.insert(pPoly->iTexIndex);				
				}
				//else
				//{
				//	MessageBox( NULL, _T("폴리곤에 텍스쳐 및 매핑이 되어 있지 않습니다."), _T("ERROR"), MB_OK );	
				//}
			}
		}		
		
		// 리프노드당 사용 텍스쳐갯수를 파악해 두면 버텍스버퍼를 할당시 사용 텍스쳐의인덱스와 관계없이 배열로써 처리가능하다.
		pNode->iNumMtrl = (int)LeafTex.size();
		pNode->m_MtrlList.resize(pNode->iNumMtrl);		
		//set 순회
		LeafPerNumTexture::const_iterator pos;
		int iInfo = 0;
		for( pos = LeafTex.begin(); pos != LeafTex.end(); ++pos, iInfo++ )
		{
			pNode->m_MtrlList[iInfo].iMtrl =  *pos;
			pNode->m_MtrlList[iInfo].iNumFaces = (int)PolygonList.count(pNode->m_MtrlList[iInfo].iMtrl);
		}
		LeafTex.clear();
		PolygonList.clear();
		
		pNode->m_pIBList.resize(pNode->iNumMtrl);
		pNode->m_pVBList.resize(pNode->iNumMtrl);
		for( int iBuffer= 0; iBuffer < pNode->iNumMtrl; iBuffer++ )
		{	
			SetBufferOpti( pNode, iBuffer);		
		}
	}
	for( int iChild=0; iChild < pNode->m_ChildList.size(); iChild++ )
	{
		SetNodePerRenderGroup( pNode->m_ChildList[iChild] );
	}	
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////
void TBspParser::SetBufferOpti(TNode* pNode, int iBuffer )
{
	HRESULT hr=S_OK;
	int iPushCnt = 0;
	int iFaceCnt = 0;		

	vector<DWORD>	IndexList;
	IndexList.reserve(1000*3);

	VertexList VListData;
	VListData.reserve(1000*3);
	int iIndexCnt = 0;
	// 폴리곤리스트의 페이스들을 VectorList에 저장한다. 저장된 페이스리스트들에 대한 버텍스최적화 작업을 한다.
	for( TPolygon* pPoly = pNode->m_pSplitter; pPoly; pPoly = pPoly->pNext )
	{		
		for( int iver = 0; iver < pPoly->iNumIndices; iver++ )
		{			
			if( pPoly->iTexIndex == pNode->m_MtrlList[iBuffer].iMtrl )
			{				
				int iReturn = -1;				
				if( !VListData.empty() )
				{
					iReturn = IsEqulVertexList(	VListData, 
												&pPoly->pVertexList[pPoly->pIndices[iver]],
												iPushCnt);
				}
				// 공유정점이 있으면
				if( iReturn  >= 0)
				{
					IndexList.push_back(iReturn);
				}
				else // 공유정점이 없으면
				{
					VListData.push_back(pPoly->pVertexList[pPoly->pIndices[iver]]);	
					IndexList.push_back(iPushCnt);
					iPushCnt++;
				}							
			}			
		}
	}
	vector<DWORD> indexlist(IndexList.begin(), IndexList.end());
	
	////////////////////////////////////////////////////////////////////////////////////////
	// 인덱스 버퍼 생성
	////////////////////////////////////////////////////////////////////////////////////////
	pNode->m_pIBList[iBuffer].Attach(DX::CreateIndexBuffer(m_pd3dDevice,
		&indexlist.at(0),
		pNode->m_MtrlList[iBuffer].iNumFaces * 3,
		sizeof(DWORD)));
	////////////////////////////////////////////////////////////////////////////////////////
	// 정점 버퍼 생성
	////////////////////////////////////////////////////////////////////////////////////////	
	vector<PNCT_VERTEX> vertexlist(VListData.begin(), VListData.end());

	pNode->m_MtrlList[iBuffer].iNumVertex = (int)VListData.size();
	pNode->m_pVBList[iBuffer].Attach(DX::CreateVertexBuffer(m_pd3dDevice,
		&vertexlist.at(0),
		pNode->m_MtrlList[iBuffer].iNumVertex,
		sizeof(PNCT_VERTEX)));	
	VListData.clear();
}

int TBspParser::IsEqulVertexList( VertexList& CollectMap, PNCT_VERTEX* pTsVertex, int iPushCnt )
{
	int iVerIndex = -1;
	int iCurVer = 0;
	if( CollectMap.empty() ) return -1;
	for( VertexListItor itor = CollectMap.begin(); itor != CollectMap.end(); itor++, iCurVer++) 
	{
		PNCT_VERTEX *pCollect = (PNCT_VERTEX*)(&itor);
		if( pCollect )
		{
			if(pCollect->p == pTsVertex->p )
			{
				if( pCollect->n == pTsVertex->n &&
					pCollect->t == pTsVertex->t &&
					pCollect->c == pTsVertex->c )
				{				
					iVerIndex = iCurVer;
					return iVerIndex;
				}			
			}	
		}
	}
	return iVerIndex;
}
TBspParser::TBspParser()
{
	m_bBinary = false;
}
TBspParser::~TBspParser()
{
}