#include "TTbsObj.h"
#include "TTimer.h"
bool TTbsObj::Load(ID3D11Device* pd3dDevice, const TCHAR* szLoadName, const TCHAR* pLoadShaderFile, bool bThread )
{
	m_dxobj.m_iVertexSize = sizeof(PNCT_VERTEX);
	m_dxobj.m_iIndexSize = sizeof(DWORD);

	if( LoadScene(szLoadName) )
	{
		if( !LoadMaterial() ) return false;	
		if( !LoadObject() ) return false;	
		
		CStopwatch stopwatch;
		if (!Convert(pd3dDevice))
		{
			return false;
		}
		m_dxobj.m_iNumVertex = m_iMaxVertex;
		m_dxobj.m_iNumIndex = m_iMaxIndex;

		if (!bThread && !Create(pd3dDevice, pLoadShaderFile))
		{
			m_Parser.CloseStream();
			return false;
		}
		m_Parser.CloseStream();
		stopwatch.Output(L"TTbsObj::Load After");
		return true;
	}	
	return false;
}
bool TTbsObj::LoadScene(const TCHAR* strFileName)
{
	TCHAR Drive[MAX_PATH];
	TCHAR Dir[MAX_PATH];
	TCHAR FName[MAX_PATH];
	TCHAR Ext[MAX_PATH];		
	_tsplitpath( strFileName, Drive, Dir, FName, Ext );			
	m_szDirName = Drive;
	m_szDirName+= Dir;	
	m_szName = FName;
	m_szName+= Ext;

	TCHAR strDate[256] = {0,};
	if( m_Parser.OpenStream( strFileName ) )
	{
		if( !m_Parser.GetDataFromFileLoop( _T("#TBF_FILE_EXPORT"), strDate, STRING_DATA ) )
		{
			return m_Parser.ErrorCloseStream( _T(" NOT FIND! [ #SME_FILE_EXPORT ]."));		
		}		
	}
	else{
		return false;
	}
	if( !m_Parser.GetDataFromFileLoop( _T("#SCENE" )) )
	{
		return m_Parser.ErrorCloseStream( _T(" NO FIND! [ #SCENE ]."));
	}
	
	// Scene
	_fgetts( m_Parser.m_pBuffer, 256, m_Parser.m_pStream );		
	_stscanf(m_Parser.m_pBuffer, _T("%d %d%d%d%d%d %d%d"),
											&m_Scene.iVersion,
											&m_Scene.iFirstFrame,
											&m_Scene.iLastFrame,
											&m_Scene.iFrameSpeed,
											&m_Scene.iTickPerFrame,
											&m_Scene.iNumMesh,										
											&m_Scene.iMaxWeight,
											&m_Scene.iBindPose);
	return true;
}
bool TTbsObj::LoadMaterial()
{
	// Material		
	int iNumRootMtl = 0;
	if( !m_Parser.GetDataFromFileLoop( _T("#ROOT_MATERIAL" ), &iNumRootMtl, INT_DATA ) )
	{
		return m_Parser.ErrorCloseStream( _T(" NO FIND! [ #SCENE ]."));
	}
	m_Material.resize( iNumRootMtl );
	for( int iMtl = 0; iMtl < iNumRootMtl; iMtl++ )
	{	
		if( !m_Parser.GetDataFromFileLoop( _T("#MATERIAL")) )
		{
			return m_Parser.ErrorCloseStream( _T(" NO FIND! [ #MATERIAL ]."));
		}		

		LoadMaterialHeader(&m_Material[iMtl]);

		if(m_Material[iMtl].m_dwSubCount > 0)
		{
			// Read ㅣSubMaterial Counter
			m_Material[iMtl].m_SubMaterial.resize(m_Material[iMtl].m_dwSubCount);
			for( DWORD  dwSub = 0; dwSub < m_Material[iMtl].m_dwSubCount; dwSub++ )
			{
				// SubMaterial Index
				if( !m_Parser.GetDataFromFileLoop( _T("#SUBMATERIAL"), &m_Material[iMtl].m_SubMaterial[dwSub].m_dwIndex, INT_DATA ) )
				{
					return m_Parser.ErrorCloseStream( _T(" NO FIND! [ #SCENE ]."));
				}				
				LoadMaterialHeader(&m_Material[iMtl].m_SubMaterial[dwSub]);
				LoadTexMap(&m_Material[iMtl].m_SubMaterial[dwSub], m_szDirName);
			}
		}
		else
		{		
			// 텍스처 맵 로드
			LoadTexMap(&m_Material[iMtl], m_szDirName);
		}		
	}
	return true;
}
void TTbsObj::LoadMaterialHeader(TMtrl* pMtrl)
{
	TCHAR strName[256] = _T("");
	TCHAR strClassName[256] = _T("");
	_fgetts( m_Parser.m_pBuffer, 256, m_Parser.m_pStream );		
	_stscanf(m_Parser.m_pBuffer, _T("%s%s%d%d"), strName,
												strClassName,
												&pMtrl->m_dwSubCount,
												&pMtrl->m_dwTexMapCount );
	pMtrl->m_strName = strName;
	pMtrl->m_strClassName = strClassName;
}
void TTbsObj::LoadTexMap(TMtrl* pMtrl, T_STR szDirName )
{
	for( DWORD dwTex = 0; dwTex < pMtrl->m_dwTexMapCount; dwTex++ )
	{
		TCHAR strName[256] = _T("");
		TTextexMap TexMap;
		_fgetts( m_Parser.m_pBuffer, 256, m_Parser.m_pStream );		
		_stscanf(m_Parser.m_pBuffer, _T("%d%s"),&TexMap.m_dwType, strName);

		TexMap.m_strTextureName = strName;
		// 텍스쳐 매니져 등록 및 배열 저장.
		TexMap.m_dwIndex = I_Texture.Add(
			g_pd3dDevice, TexMap.m_strTextureName.c_str(), szDirName.c_str());
		
		pMtrl->m_TexMaps.push_back(TexMap);
	}
}
bool TTbsObj::LoadObject()
{
	// Object	
	DWORD dwCount;
	for( int iMesh = 0; iMesh < m_Scene.iNumMesh; iMesh++ )
	{	
		shared_ptr<tTbsData> pData = make_shared<tTbsData>();
		shared_ptr<TMesh> pMesh = make_shared<TMesh>();
		pMesh->m_iIndex = iMesh;
		if( !m_Parser.GetDataFromFileLoop( _T("#OBJECTS"), &dwCount, INT_DATA  ) )
		{
			return m_Parser.ErrorCloseStream( _T(" NO FIND! [ #OBJECTS ]."));
		}
	
		TCHAR strName[256] = _T("");		
		TCHAR strParentName[256] = _T("");
		_fgetts( m_Parser.m_pBuffer, 256, m_Parser.m_pStream );	
		_stscanf(m_Parser.m_pBuffer, _T(" %s %s"), strName, strParentName);

		pMesh->m_strNodeName = strName;
		pMesh->m_strParentName = strParentName;
		
		_fgetts( m_Parser.m_pBuffer, 256, m_Parser.m_pStream );		
		_stscanf(m_Parser.m_pBuffer, _T("%d%d%d%d%d%d %d"),
				&pMesh->m_ClassType,
				&pMesh->m_iNumFace, 
				&pMesh->m_iNumTrack[0],
				&pMesh->m_iNumTrack[1],
				&pMesh->m_iNumTrack[2],
				&pMesh->m_iNumTrack[3],
				&pMesh->m_iMtrlRef);
		
		_fgetts( m_Parser.m_pBuffer, 256, m_Parser.m_pStream );	
		_stscanf(m_Parser.m_pBuffer, _T("%f%f%f%f%f%f"),	&pMesh->m_BBox.vMax.x, &pMesh->m_BBox.vMax.y, &pMesh->m_BBox.vMax.z,
															&pMesh->m_BBox.vMin.x, &pMesh->m_BBox.vMin.y, &pMesh->m_BBox.vMin.z );	

		if( !m_Parser.GetDataFromFileLoop( _T("#WORLD_MATRIX") ) )
		{
			return m_Parser.ErrorCloseStream( _T(" NO FIND! [ #WORLD_MATRIX ]."));
		}

		_fgetts( m_Parser.m_pBuffer, 256, m_Parser.m_pStream );				
		_stscanf(m_Parser.m_pBuffer, _T("%f%f%f%f"),&pMesh->m_matWorld._11,&pMesh->m_matWorld._12,&pMesh->m_matWorld._13,&pMesh->m_matWorld._14 );		
		_fgetts( m_Parser.m_pBuffer, 256, m_Parser.m_pStream );				
		_stscanf(m_Parser.m_pBuffer, _T("%f%f%f%f"),&pMesh->m_matWorld._21,&pMesh->m_matWorld._22,&pMesh->m_matWorld._23,&pMesh->m_matWorld._24);			
		_fgetts( m_Parser.m_pBuffer, 256, m_Parser.m_pStream );				
		_stscanf(m_Parser.m_pBuffer, _T("%f%f%f%f"),&pMesh->m_matWorld._31,&pMesh->m_matWorld._32,&pMesh->m_matWorld._33,&pMesh->m_matWorld._34);			
		_fgetts( m_Parser.m_pBuffer, 256, m_Parser.m_pStream );				
		_stscanf(m_Parser.m_pBuffer, _T("%f%f%f%f"),&pMesh->m_matWorld._41,&pMesh->m_matWorld._42,&pMesh->m_matWorld._43,&pMesh->m_matWorld._44 );

		if( !m_Parser.GetDataFromFileLoop( _T("#TRAINGLES") ) )
		{
			return m_Parser.ErrorCloseStream( _T(" NO FIND! [ #TRAINGLES ]."));
		}			
		
		D3DXMatrixInverse( &pMesh->m_matInverse, NULL, &pMesh->m_matWorld );

		// 서브메쉬에 페이스가 존재하면
		if( pMesh->m_iMtrlRef >= 0 && m_Material[pMesh->m_iMtrlRef].m_dwSubCount > 0)
		{
			for( DWORD dwMtrl = 0; dwMtrl < m_Material[pMesh->m_iMtrlRef].m_dwSubCount; dwMtrl++ )
			{
				auto pSubData = make_shared<tTbsData>();			
				auto pSubMesh = make_shared<TMesh>();

				pSubMesh->m_iNumFace	= LoadVertexIndex(pSubData.get());
				pSubMesh->m_iMtrlRef	= (int)dwMtrl;
			
				if (pSubMesh->m_iNumFace <= 0)
				{
					continue;
				}
				// 실제데이터가 들어있는 서브 오브젝트 갯수( 사용처가 없는 서브매터리얼은 제외된다. )				
				pMesh->m_pSubMesh.push_back(pSubMesh );				
				pData->m_pSubMesh.push_back(pSubData);
			}
		}
		else
		{
			// 정점 및 인덱스 데이터 로드
			LoadVertexIndex(pData.get());
		}

		// 에니메이션 트랙 로드
		LoadAnimation(pMesh.get());

		m_pMesh.push_back( pMesh );	
		m_pData.push_back(pData);
	}
	return true;
}
void TTbsObj::LoadAnimation(TMesh* pMesh)
{
	if( pMesh->m_iNumTrack[0] && SUCCEEDED(m_Parser. GetDataFromFile(_T("#POSITION_TRACK")) ) )
	{		
		LoadAnimationTrack(pMesh->m_iNumTrack[0], pMesh->m_pPosTrack );
	}
	if( pMesh->m_iNumTrack[1] &&  m_Parser.GetDataFromFile( _T("#ROTATION_TRACK") ) )  
	{
		LoadAnimationTrack(pMesh->m_iNumTrack[1], pMesh->m_pRotTrack );			
	}
	if( pMesh->m_iNumTrack[2] && m_Parser.GetDataFromFile( _T("#SCALE_TRACK") ) )
	{
		LoadAnimationTrack(pMesh->m_iNumTrack[2], pMesh->m_pSclTrack );	
	}					
	if( pMesh->m_iNumTrack[3] && m_Parser.GetDataFromFile( _T("#VISIVILITY_TRACK") ) )
	{
		LoadAnimationTrack(pMesh->m_iNumTrack[3], pMesh->m_pVisTrack );			
	}				
}
int TTbsObj::LoadVertexIndex(tTbsData* pData)
{
	int iMtrlIndex = 0;
	int iNumSubFaces = 0;
	int iVertexCounter = 0;

	_fgetts( m_Parser.m_pBuffer, 256, m_Parser.m_pStream );	
	_stscanf( m_Parser.m_pBuffer, _T("%d %d %d"),  &iMtrlIndex, &iNumSubFaces, &iVertexCounter );
	if (iVertexCounter <= 0) return 0;

	if( iNumSubFaces > 0 )
	{
		pData->m_TriList.resize(iNumSubFaces);
	}
	else
	{
		pData->m_TriList.resize(pData->m_iNumFace);
	}
	pData->m_IndexArray.resize( iNumSubFaces* 3 );
	pData->m_VertexArray.resize( iVertexCounter );

	for( int iCnt = 0; iCnt < iVertexCounter; iCnt++ )
	{
		_fgetts( m_Parser.m_pBuffer, 256, m_Parser.m_pStream );	
		_stscanf( m_Parser.m_pBuffer, _T("%f %f %f  %f %f %f  %f %f %f %f  %f %f"), 
					&pData->m_VertexArray[iCnt].p.x,	&pData->m_VertexArray[iCnt].p.y,	&pData->m_VertexArray[iCnt].p.z,
					&pData->m_VertexArray[iCnt].n.x,	&pData->m_VertexArray[iCnt].n.y,	&pData->m_VertexArray[iCnt].n.z,
					&pData->m_VertexArray[iCnt].c.x,	&pData->m_VertexArray[iCnt].c.y,	&pData->m_VertexArray[iCnt].c.z,		&pData->m_VertexArray[iCnt].c.w,
					&pData->m_VertexArray[iCnt].t.x,	&pData->m_VertexArray[iCnt].t.y );
	}

	for( int iIndex = 0; iIndex < iNumSubFaces; iIndex++ )
	{			
		_fgetts( m_Parser.m_pBuffer, 256, m_Parser.m_pStream );	
		_stscanf( m_Parser.m_pBuffer, _T("%d %d %d"),	&pData->m_IndexArray[iIndex*3+0],
														&pData->m_IndexArray[iIndex*3+1],
														&pData->m_IndexArray[iIndex*3+2] );
	}
	m_iMaxVertex += (UINT)pData->m_VertexArray.size();
	m_iMaxIndex += (UINT)pData->m_IndexArray.size();
	
	// 트라이앵글 리스트 생성
	for( int iFace = 0; iFace < iNumSubFaces; iFace++ )
	{
		for( int iCnt = 0; iCnt < 3; iCnt++ )
		{				
			pData->m_TriList[iFace].vVertex[iCnt] = pData->m_VertexArray[pData->m_IndexArray[iFace*3+iCnt]];
		}	
	}	
	return iNumSubFaces;
}
bool TTbsObj::LoadAnimationTrack( int iNumTrack, vector<shared_ptr<TAnimTrack>>& pTrackList )
{
	TAnimTrack* pPrevTrack = NULL;		
	for( int iTrack = 0; iTrack < iNumTrack; iTrack++ )
	{				
		auto pTrack = make_shared<TAnimTrack>(); 		
		_fgetts( m_Parser.m_pBuffer, 256, m_Parser.m_pStream );	
		_stscanf( m_Parser.m_pBuffer, _T("%d%f%f%f%f%f%f%f"), 
			&pTrack->iTick,
			&pTrack->vVector.x,
			&pTrack->vVector.y,
			&pTrack->vVector.z,
			&pTrack->qRotate.x,
			&pTrack->qRotate.y,
			&pTrack->qRotate.z,
			&pTrack->qRotate.w );	

		pPrevTrack = SetDoublyLinkedList( pTrack.get(), pPrevTrack );
		pTrackList.push_back(pTrack);	
	}
	return true;
}
TMatrix TTbsObj::Interpolate( TMesh* pMesh, TMatrix* matParents, float fFrameTick )
{
	// TM		= AnimMat * ParentTM;
	// AaniMat	= TM * Inverse(ParentTM)
	TQuaternion qR, qS;	
	D3DXMatrixIdentity(&pMesh->m_matCalculation);		
	TMatrix matAnim, matPos, matRotate, matScale;
	
	matRotate	= pMesh->m_matWorldRotate;
	matPos		= pMesh->m_matWorldTrans;
	matScale	= pMesh->m_matWorldScale;
	
	D3DXQuaternionRotationMatrix(  &qR,  &matRotate );
	D3DXQuaternionRotationMatrix(  &qS,  &matScale );

	// fFrameTick = m_Scene.iFirstFrame * m_Scene.iTickPerFrame + CurFame;
	float fStartTick = (float)(m_Scene.iFirstFrame * m_Scene.iTickPerFrame);
	float fEndTick   = 0.0f;

	TAnimTrack* pStartTrack = NULL;
	TAnimTrack* pEndTrack   = NULL;
	if( pMesh->m_pRotTrack.size() ) 
	{
		// pStartTrack를 찾을수 있으면
		if(  GetAnimationTrack( fFrameTick, pMesh->m_pRotTrack, &pStartTrack, &pEndTrack )  ) 
		{
			qR = pStartTrack->qRotate;
			fStartTick = (float)pStartTrack->iTick;
		}		
		if( pEndTrack )
		{
			fEndTick = (float)pEndTrack->iTick;
			D3DXQuaternionSlerp(&qR, &qR, &pEndTrack->qRotate, ( fFrameTick -  fStartTick ) /  ( fEndTick - fStartTick ) );							
		}		
		D3DXMatrixRotationQuaternion(&matRotate, &qR );	
	}
	
	pStartTrack = NULL;
	pEndTrack   = NULL;

	TVector3 Trans(matPos._41, matPos._42, matPos._43 );
	if( pMesh->m_pPosTrack.size() ) 
	{
		// pStartTrack를 찾을수 있으면
		if(  GetAnimationTrack( fFrameTick, pMesh->m_pPosTrack, &pStartTrack, &pEndTrack )  ) 
		{
			Trans = pStartTrack->vVector; 
			fStartTick = (float)pStartTrack->iTick;
		}		
		if( pEndTrack )
		{
			fEndTick = (float)pEndTrack->iTick;
			D3DXVec3Lerp( &Trans, &Trans,&pEndTrack->vVector, ( fFrameTick -  fStartTick ) /  ( fEndTick - fStartTick ) );							
		}	
		
		D3DXMatrixTranslation(&matPos, Trans.x, Trans.y, Trans.z);	
	}


	pStartTrack = NULL;
	pEndTrack   = NULL;

	TMatrix matScaleRot, matInvScaleRot;
	TVector3 vScale(matScale._11, matScale._22, matScale._33 );
	if( pMesh->m_pSclTrack.size() ) 
	{
		// pStartTrack를 찾을수 있으면
		if(  GetAnimationTrack( fFrameTick, pMesh->m_pSclTrack, &pStartTrack, &pEndTrack )  ) 
		{
			vScale = pStartTrack->vVector; 
			qS = pStartTrack->qRotate;
			fStartTick = (float)pStartTrack->iTick;
		}		
		if( pEndTrack )
		{
			fEndTick = (float)pEndTrack->iTick;
			D3DXVec3Lerp( &vScale, &vScale,&pEndTrack->vVector, ( fFrameTick -  fStartTick ) /  ( fEndTick - fStartTick ) );							
			D3DXQuaternionSlerp(&qS, &qS, &pEndTrack->qRotate, ( fFrameTick -  fStartTick ) /  ( fEndTick - fStartTick ) );
		}			
		D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z);	
		D3DXMatrixRotationQuaternion(&matScaleRot, &qS );
		D3DXMatrixInverse( &matInvScaleRot, NULL, &matScaleRot );
		matScale = matInvScaleRot  * matScale * matScaleRot;
	}

	pStartTrack = NULL;
	pEndTrack   = NULL;

	float fCurAlpha, fNextAlpha, fOffSet;
	fCurAlpha = 0.0f; 	
	fNextAlpha= 0.0f;
	if( pMesh->m_pVisTrack.size() ) 
	{
		// pStartTrack를 찾을수 있으면
		if(  GetAnimationTrack( fFrameTick, pMesh->m_pVisTrack, &pStartTrack, &pEndTrack )  ) 
		{
			fCurAlpha = pStartTrack->vVector.x;  
			fStartTick = (float)pStartTrack->iTick;
		}		
		if( pEndTrack )
		{
			fNextAlpha = pEndTrack->vVector.x;
			fEndTick = (float)pEndTrack->iTick;

			fOffSet = (fFrameTick - fStartTick)/(fEndTick - fStartTick );
			fNextAlpha = (fNextAlpha - fCurAlpha)*fOffSet;											
		}			
		pMesh->m_fVisibility = (fCurAlpha + fNextAlpha);	
	}
	else
	{		
		pMesh->m_fVisibility = 1.0f;
	}
	
	D3DXMatrixMultiply(&matAnim, &matScale, &matRotate);
	matAnim._41 = matPos._41;
	matAnim._42 = matPos._42;
	matAnim._43 = matPos._43;
	// 최종 에미메이션 행렬을 완성한다.	
	D3DXMatrixMultiply(&pMesh->m_matCalculation,&matAnim, matParents );

	// 인버스 매트릭스 확인 코드.
	TVector3 v0,v1,v2,v3;
	v0 = pMesh->m_matCalculation.m[0];
	v1 = pMesh->m_matCalculation.m[1];
	v2 = pMesh->m_matCalculation.m[2];
	D3DXVec3Cross(&v3, &v1, &v2);
	if( D3DXVec3Dot( &v3, &v0 ) < 0.0f)
	{
		TMatrix matW;
		D3DXMatrixScaling(&matW, -1.0f, -1.0f, -1.0f);
		D3DXMatrixMultiply( &pMesh->m_matCalculation, &pMesh->m_matCalculation, &matW );
	}
	return pMesh->m_matCalculation;
}
// pEndTrack 트랙이 없으면 flase 리턴( 보간할 대상이 없을 때 )
bool TTbsObj::GetAnimationTrack(float fFrame, vector<shared_ptr<TAnimTrack>> pTrackList, TAnimTrack** pStartTrack, TAnimTrack** pEndTrack )
{
	for( DWORD dwTrack = 0; dwTrack < pTrackList.size(); dwTrack++ ) 
	{
		TAnimTrack *pTrack = pTrackList[dwTrack].get();		
		_ASSERT( pTrack );		
		// fFrame 보다 큰 Tick 트랙이 있다면 이전 트랙을 넘겨 주어야 하기 때문에 break한다.
		if( pTrack->iTick > fFrame )	
		{			
			*pEndTrack = pTrack;
			break;
		}		
		// 현프레임보다 큰값이 없다면. 가장 시간경과가 오래된 것을 사용한다.
		*pStartTrack	= pTrack;		
	}
	return ( *pStartTrack != NULL ) ? true : false;
}
bool TTbsObj::Frame()
{
	m_fElapseTime += g_fSecPerFrame * m_Scene.iFrameSpeed * m_Scene.iTickPerFrame;		
	if( m_fElapseTime >= m_Scene.iLastFrame* m_Scene.iTickPerFrame ) 
	{				
		m_fElapseTime = (float)(m_Scene.iFirstFrame * m_Scene.iTickPerFrame);
	}

	TMatrix mat;
	D3DXMatrixIdentity(&mat);
	for( DWORD dwObject = 0; dwObject < m_pData.size(); dwObject++) 
	{
		TMesh* pMesh = m_pMesh[dwObject].get();
		if( pMesh->m_bUsed == false ) continue;	
		if( pMesh->m_pParent)
			Interpolate( pMesh, &pMesh->m_pParent->m_matCalculation,m_fElapseTime );
		else
			Interpolate( pMesh, &mat,m_fElapseTime);							
	}	
	/*for (DWORD dwObject = 0; dwObject < m_pData.size(); dwObject++)
	{
		TMesh* pMesh = m_pMesh[dwObject].get();
		pMesh->m_matCalculation = pMesh->m_matCalculation * m_matWorld;
	}*/
	return true;
}

bool TTbsObj::Render(ID3D11DeviceContext*    pContext)
{
	PreRender(pContext);
	Draw(pContext, this);
	return true;
}
bool TTbsObj::Release()
{
	return true;
}
bool TTbsObj::Convert(ID3D11Device* pd3dDevice)
{	
	if( !SetMaterial() )
	{
		return false;
	}
	if( !InheriteCollect() ) 
	{
		return false;
	}
	return true;
}
TAnimTrack* TTbsObj::SetDoublyLinkedList( TAnimTrack* pCurrentTrack, TAnimTrack* pPrev )
{	
	if( pPrev )
	{
		pPrev->pNext	= pCurrentTrack;
		pCurrentTrack->pPrev	= pPrev;		
	}
	return pCurrentTrack;
}


int	TTbsObj::GetMapID( TMtrl* pMtrl, int iTexMapType )
{
	_ASSERT( pMtrl );
	if( pMtrl->m_TexMaps.size() <= 0 ) return -1;

	for( int iTexmap = 0; iTexmap < pMtrl->m_TexMaps.size(); iTexmap++ )
	{
		if( pMtrl->m_TexMaps[iTexmap].m_dwType == iTexMapType )
		{
			return pMtrl->m_TexMaps[iTexmap].m_dwIndex;					
		}
	}	
	return -1;
}

bool TTbsObj::ResetResource()
{
	return true;
}

bool TTbsObj::InheriteCollect()
{
	TMatrix m_matInverse;
	TQuaternion qR;
	TVector3 vTrans, vScale;
	for( DWORD dwObject = 0; dwObject < m_pData.size(); dwObject++) 
	{		
		if( !m_pMesh[dwObject]->m_strParentName.empty() 
			&& ( m_pMesh[dwObject]->m_strParentName != _T("(null)") ) )
		{
			TMesh* pParentNode = SearchToCollects( m_pMesh[dwObject]->m_strParentName );
			if(pParentNode)
			{
				m_pMesh[dwObject]->m_pParent = pParentNode;
				TMatrix matInverse= m_pMesh[dwObject]->m_matWorld * pParentNode->m_matInverse;					
				D3DXMatrixDecompose( &vScale, &qR, &vTrans, &matInverse );
				D3DXMatrixScaling( &m_pMesh[dwObject]->m_matWorldScale, vScale.x, vScale.y, vScale.z );
				D3DXMatrixTranslation( &m_pMesh[dwObject]->m_matWorldTrans, vTrans.x, vTrans.y, vTrans.z );
				D3DXMatrixRotationQuaternion( &m_pMesh[dwObject]->m_matWorldRotate, &qR );
				
				pParentNode->m_pChildMesh.push_back(m_pMesh[dwObject].get());
			}			
		}
		else
		{
			D3DXMatrixDecompose( &vScale, &qR, &vTrans, &m_pMesh[dwObject]->m_matWorld );
			D3DXMatrixScaling( &m_pMesh[dwObject]->m_matWorldScale, vScale.x, vScale.y, vScale.z );
			D3DXMatrixTranslation( &m_pMesh[dwObject]->m_matWorldTrans, vTrans.x, vTrans.y, vTrans.z );
			D3DXMatrixRotationQuaternion( &m_pMesh[dwObject]->m_matWorldRotate, &qR );		
		}
	}	
	// 사용되지 않은 오브젝트 삭제
	for( DWORD dwObject = 0; dwObject < m_pMesh.size(); dwObject++) 
	{
		if( m_pMesh[dwObject]->m_pChildMesh.size() <= 0 && 
			(m_pMesh[dwObject]->m_ClassType == CLASS_BONE || m_pMesh[dwObject]->m_ClassType == CLASS_DUMMY))
		{
			m_pMesh[dwObject]->m_bUsed = false;
		}
	}
	return true;
}
TMesh* TTbsObj::SearchToCollects( T_STR	m_strParentName )
{
	for( DWORD dwGroup = 0; dwGroup < m_pData.size(); dwGroup++ ) 
	{		
		if( m_pMesh[dwGroup]->m_strNodeName == m_strParentName )
		{
			return m_pMesh[dwGroup].get();
		}
	}	
	return NULL;	
}
bool TTbsObj::SetMaterial()
{
	for( DWORD dwObject = 0; dwObject < m_pData.size(); dwObject++)
	{		
		TMesh* pMesh = m_pMesh[dwObject].get();
		if( pMesh->m_iNumFace <= 0 || pMesh->m_iMtrlRef < 0 ) continue;

		if( pMesh->m_pSubMesh.size() > 0 )
		{
			for( int iSubMesh =0; iSubMesh < pMesh->m_pSubMesh.size(); iSubMesh++ )
			{
				TMesh* pSubMesh = pMesh->m_pSubMesh[iSubMesh].get();
				if( pSubMesh )
				{
					pSubMesh->m_iDiffuseTex = GetMapID( &m_Material[pMesh->m_iMtrlRef].m_SubMaterial[pSubMesh->m_iMtrlRef], ID_TBASIS_DI );
				}	
			}
		}		
		else
		{
			pMesh->m_iDiffuseTex = GetMapID( &m_Material[pMesh->m_iMtrlRef], ID_TBASIS_DI );			
		}		
	}
	return true;
}
bool TTbsObj::UpdateBuffer()
{
	if (!CombineBuffer(m_dxobj.g_pVertexBuffer.Get(), m_dxobj.g_pIndexBuffer.Get()))
	{
		return false;
	}
	return true;
}
bool TTbsObj::CombineBuffer(ID3D11Buffer* pVB, ID3D11Buffer* pIB)
{
	size_t dstOffset = 0;
	size_t vbOffset = 0;
	size_t dstibOffset = 0;
	size_t ibOffset = 0;
	{
		for (DWORD dwObject = 0; dwObject < m_pData.size(); dwObject++)
		{
			auto pData = m_pData[dwObject].get();
			auto ptMesh = m_pMesh[dwObject].get();
			if (ptMesh->m_pSubMesh.size() > 0)
			{
				for (DWORD dwSub = 0; dwSub < ptMesh->m_pSubMesh.size(); dwSub++)
				{
					auto pSubData = pData->m_pSubMesh[dwSub].get();
					auto ptSubMesh = ptMesh->m_pSubMesh[dwSub].get();
					if (pSubData->m_VertexArray.size() < 3) continue;

					ptSubMesh->m_dxobj.m_iNumVertex = (UINT)pSubData->m_VertexArray.size();
					ptSubMesh->m_dxobj.m_iVertexSize = m_dxobj.m_iVertexSize;

					ptSubMesh->m_dxobj.m_BoxVB.left = (UINT)dstOffset;
					ptSubMesh->m_dxobj.m_BoxVB.right = (UINT)(dstOffset + ptSubMesh->m_dxobj.m_iNumVertex*m_dxobj.m_iVertexSize);
					ptSubMesh->m_dxobj.m_BoxVB.top = 0; ptSubMesh->m_dxobj.m_BoxVB.bottom = 1;
					ptSubMesh->m_dxobj.m_BoxVB.front = 0; ptSubMesh->m_dxobj.m_BoxVB.back = 1;

					g_pImmediateContext->UpdateSubresource(
						pVB, 0,
						&ptSubMesh->m_dxobj.m_BoxVB,
						(uint8_t*)&pSubData->m_VertexArray.at(0),
						0, 0);


					/*g_pImmediateContext->CopySubresourceRegion(
					m_dxobj.g_pVertexBuffer.Get(), 0, iBeginPos, 0, 0,
					(void*)&pSubMesh->m_VertexArray.at(0),
					0, &pSubMesh->m_dxobj.m_BoxVB);*/

					ptSubMesh->m_dxobj.m_iBeginVB = (UINT)vbOffset;
					vbOffset += ptSubMesh->m_dxobj.m_iNumVertex;
					dstOffset = ptSubMesh->m_dxobj.m_BoxVB.right;

					ptSubMesh->m_dxobj.m_iNumIndex = (UINT)pSubData->m_IndexArray.size();
					ptSubMesh->m_dxobj.m_BoxIB.left = (UINT)dstibOffset;
					ptSubMesh->m_dxobj.m_BoxIB.right = (UINT)(dstibOffset + ptSubMesh->m_dxobj.m_iNumIndex*sizeof(DWORD));
					ptSubMesh->m_dxobj.m_BoxIB.top = 0;	ptSubMesh->m_dxobj.m_BoxIB.bottom = 1;
					ptSubMesh->m_dxobj.m_BoxIB.front = 0;	ptSubMesh->m_dxobj.m_BoxIB.back = 1;

					g_pImmediateContext->UpdateSubresource(pIB, 0,
						&ptSubMesh->m_dxobj.m_BoxIB, (void*)&pSubData->m_IndexArray.at(0), 0, 0);

					ptSubMesh->m_dxobj.m_iBeginIB = (UINT)ibOffset;
					ibOffset += ptSubMesh->m_dxobj.m_iNumIndex;
					dstibOffset = ptSubMesh->m_dxobj.m_BoxIB.right;

					//texture
					if (ptSubMesh->m_iDiffuseTex < 0) continue;
					ptSubMesh->m_dxobj.g_pTextureSRV = I_Texture.GetPtr(ptSubMesh->m_iDiffuseTex)->m_pTextureSRV;
				}
			}
			else
			{
				if (pData->m_VertexArray.size() < 3) continue;
				ptMesh->m_dxobj.m_iNumVertex = (UINT)pData->m_VertexArray.size();
				ptMesh->m_dxobj.m_iVertexSize = m_dxobj.m_iVertexSize;

				ptMesh->m_dxobj.m_BoxVB.left = (UINT)dstOffset;
				ptMesh->m_dxobj.m_BoxVB.right = (UINT)(dstOffset + ptMesh->m_dxobj.m_iNumVertex*m_dxobj.m_iVertexSize);
				ptMesh->m_dxobj.m_BoxVB.top = 0; ptMesh->m_dxobj.m_BoxVB.bottom = 1;
				ptMesh->m_dxobj.m_BoxVB.front = 0; ptMesh->m_dxobj.m_BoxVB.back = 1;

				g_pImmediateContext->UpdateSubresource(pVB, 0, &ptMesh->m_dxobj.m_BoxVB, (void*)&pData->m_VertexArray.at(0), 0, 0);

				ptMesh->m_dxobj.m_iBeginVB = (UINT)vbOffset;
				vbOffset += ptMesh->m_dxobj.m_iNumVertex;
				dstOffset = ptMesh->m_dxobj.m_BoxVB.right;

				ptMesh->m_dxobj.m_iNumIndex = (UINT)pData->m_IndexArray.size();
				ptMesh->m_dxobj.m_BoxIB.left = (UINT)dstibOffset;
				ptMesh->m_dxobj.m_BoxIB.right = (UINT)(dstibOffset + ptMesh->m_dxobj.m_iNumIndex*sizeof(DWORD));
				ptMesh->m_dxobj.m_BoxIB.top = 0; ptMesh->m_dxobj.m_BoxIB.bottom = 1;
				ptMesh->m_dxobj.m_BoxIB.front = 0; ptMesh->m_dxobj.m_BoxIB.back = 1;

				g_pImmediateContext->UpdateSubresource(pIB, 0, &ptMesh->m_dxobj.m_BoxIB, (void*)&pData->m_IndexArray.at(0), 0, 0);
				ptMesh->m_dxobj.m_iBeginIB = (UINT)ibOffset;
				ibOffset += ptMesh->m_dxobj.m_iNumIndex;
				dstibOffset = ptMesh->m_dxobj.m_BoxIB.right;

				//texture
				if (ptMesh->m_iDiffuseTex < 0) continue;
				ptMesh->m_dxobj.g_pTextureSRV = I_Texture.GetPtr(ptMesh->m_iDiffuseTex)->m_pTextureSRV;
			}
		}
	}
	return true;
}
bool TTbsObj::Draw(ID3D11DeviceContext*    pContext, TActor* pParent)
{
	//CStopwatch stopwatch;
	for (DWORD dwObject = 0; dwObject < m_pData.size(); dwObject++)
	{
		auto pMesh = m_pMesh[dwObject].get();
		TMatrix matWorld = pMesh->m_matCalculation * pParent->m_matWorld;
		D3DXMatrixTranspose(&m_cbData.matWorld, &matWorld);
		UpdateConstantBuffer(pContext, pParent);
		
		if (pMesh->m_pSubMesh.size() > 0)
		{
			for (DWORD dwSub = 0; dwSub < pMesh->m_pSubMesh.size(); dwSub++)
			{
				auto pSubMesh = pMesh->m_pSubMesh[dwSub].get();
				if (pSubMesh->m_iNumFace < 1) continue;
				pContext->PSSetShaderResources(0, 1, pSubMesh->m_dxobj.g_pTextureSRV.GetAddressOf());
				pContext->DrawIndexed(pSubMesh->m_dxobj.m_iNumIndex,
					pSubMesh->m_dxobj.m_iBeginIB,
					pSubMesh->m_dxobj.m_iBeginVB);
			}
		}
		else
		{
			if (pMesh->m_iNumFace < 1) continue;
			pContext->PSSetShaderResources(0, 1, pMesh->m_dxobj.g_pTextureSRV.GetAddressOf());
			pContext->DrawIndexed(pMesh->m_dxobj.m_iNumIndex,
				pMesh->m_dxobj.m_iBeginIB,
				pMesh->m_dxobj.m_iBeginVB);
		}
	}
	//stopwatch.Output(L"\nTTbsObj::Draw");
	return true;
}
TTbsObj::TTbsObj(void)
{
}

TTbsObj::~TTbsObj(void)
{
}
