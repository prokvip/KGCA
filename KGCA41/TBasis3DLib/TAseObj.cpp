#include "TAseObj.h"
#include "TTimer.h"

void TAseObj::SetMatrix(TMatrix* pWorld, TMatrix* pView, TMatrix* pProj)
{
	if (pWorld != NULL)
	{
		m_matWorld = *pWorld;
		/*m_vCenter.x = pWorld->_41;
		m_vCenter.y = pWorld->_42;
		m_vCenter.z = pWorld->_43;*/
	}
	if (pView != NULL)
	{
		m_matView = *pView;
	}
	if (pProj != NULL)
	{
		m_matProj = *pProj;
	}
	D3DXMatrixTranspose(&m_cbData.matWorld, &m_matWorld);
	D3DXMatrixTranspose(&m_cbData.matView, &m_matView);
	D3DXMatrixTranspose(&m_cbData.matProj, &m_matProj);
}
bool TAseObj::Load(	ID3D11Device* pd3dDevice, 
					const TCHAR* szLoadName, 
					const TCHAR* pLoadShaderFile, bool bThread)
{	
	m_dxobj.m_iVertexSize = sizeof(PNCT_VERTEX);
	m_dxobj.m_iIndexSize = sizeof(DWORD);

	__int64 qwElapsedTimeA, qwElapsedTimeB;
	if (m_Parser.Load(szLoadName))
	{
		CStopwatch stopwatch;
		int iObjectType = -1;
		int iNumTypes = sizeof(AseSections) / sizeof(AseSections[0]);
		while ((iObjectType = m_Parser.SearchTokenArray(iNumTypes, AseSections, m_Parser.m_dwMaxStringTable)) >= 0)
		{
			switch (iObjectType)
			{
			case SCENE:
			{
				if (!m_Parser.LoadScene(m_Scene))		return false;
				qwElapsedTimeA = stopwatch.Now();
			}break;
			case MATERIALLIST:
			{
				if (!m_Parser.LoadMaterialList(m_Material)) return false;
				qwElapsedTimeB = stopwatch.Now() - qwElapsedTimeA;
			}break;
			case GEOMOBJECT:
			{
				if (!m_Parser.LoadGeomObject())		return false;
			}break;
			case HELPEROBJECT:
			{
				if (!m_Parser.LoadHelperObject())		return false;
			}break;
			default:
				return false;
			}
		}
		stopwatch.Output(L"TAseObj::Load");
	}
	m_szDirName = m_Parser.m_szDirName;
	m_szName = m_Parser.m_szName;

	CStopwatch stopwatch;
	if (!Convert(pd3dDevice))
	{
		return false;
	}

	m_dxobj.m_iNumVertex = m_iMaxVertex;
	m_dxobj.m_iNumIndex = m_iMaxIndex;

	if (!bThread && !Create(pd3dDevice, pLoadShaderFile))
	{
		m_Parser.Release();
		return false;
	}
	m_Parser.Release();
	m_Material.clear();
	stopwatch.Output(L"TAseObj::Load After");
	return true;
}
void TAseObj::Interpolate(TMesh* pMesh,
	TMatrix* matParents, float fFrameTick)
{
	// TM		= AnimMat * ParentTM;
	// AaniMat	= TM * Inverse(ParentTM)
	TQuaternion qR, qS;
	D3DXMatrixIdentity(&pMesh->m_matCalculation);
	TMatrix matAnim, matPos, matRotate, matScale;

	matRotate = pMesh->m_matWorldRotate;
	matPos = pMesh->m_matWorldTrans;
	matScale = pMesh->m_matWorldScale;

	D3DXQuaternionRotationMatrix(&qR, &matRotate);
	D3DXQuaternionRotationMatrix(&qS, &matScale);

	// fFrameTick = m_Scene.iFirstFrame * m_Scene.iTickPerFrame + CurFame;
	float fStartTick = (float)(m_Scene.iFirstFrame * m_Scene.iTickPerFrame);
	float fEndTick = 0.0f;

	TAnimTrack* pStartTrack = NULL;
	TAnimTrack* pEndTrack = NULL;
	if (pMesh->m_pRotTrack.size())
	{
		// pStartTrack를 찾을수 있으면
		if (GetAnimationTrack(fFrameTick, pMesh->m_pRotTrack, &pStartTrack, &pEndTrack))
		{
			qR = pStartTrack->qRotate;
			fStartTick = (float)pStartTrack->iTick;
		}
		if (pEndTrack)
		{
			fEndTick = (float)pEndTrack->iTick;
			D3DXQuaternionSlerp(&qR, &qR, &pEndTrack->qRotate, (fFrameTick - fStartTick) / (fEndTick - fStartTick));
		}
		D3DXMatrixRotationQuaternion(&matRotate, &qR);
	}

	pStartTrack = NULL;
	pEndTrack = NULL;

	TVector3 Trans(matPos._41, matPos._42, matPos._43);
	if (pMesh->m_pPosTrack.size())
	{
		// pStartTrack를 찾을수 있으면
		if (GetAnimationTrack(fFrameTick, pMesh->m_pPosTrack, &pStartTrack, &pEndTrack))
		{
			Trans = pStartTrack->vVector;
			fStartTick = (float)pStartTrack->iTick;
		}
		if (pEndTrack)
		{
			fEndTick = (float)pEndTrack->iTick;
			D3DXVec3Lerp(&Trans, &Trans, &pEndTrack->vVector, (fFrameTick - fStartTick) / (fEndTick - fStartTick));
		}

		D3DXMatrixTranslation(&matPos, Trans.x, Trans.y, Trans.z);
	}


	pStartTrack = NULL;
	pEndTrack = NULL;

	TMatrix matScaleRot, matInvScaleRot;
	TVector3 vScale(matScale._11, matScale._22, matScale._33);
	if (pMesh->m_pSclTrack.size())
	{
		// pStartTrack를 찾을수 있으면
		if (GetAnimationTrack(fFrameTick, pMesh->m_pSclTrack, &pStartTrack, &pEndTrack))
		{
			vScale = pStartTrack->vVector;
			qS = pStartTrack->qRotate;
			fStartTick = (float)pStartTrack->iTick;
		}
		if (pEndTrack)
		{
			fEndTick = (float)pEndTrack->iTick;
			D3DXVec3Lerp(&vScale, &vScale, &pEndTrack->vVector, (fFrameTick - fStartTick) / (fEndTick - fStartTick));
			D3DXQuaternionSlerp(&qS, &qS, &pEndTrack->qRotate, (fFrameTick - fStartTick) / (fEndTick - fStartTick));
		}
		D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z);
		D3DXMatrixRotationQuaternion(&matScaleRot, &qS);
		D3DXMatrixInverse(&matInvScaleRot, NULL, &matScaleRot);
		matScale = matInvScaleRot  * matScale * matScaleRot;
	}

	pStartTrack = NULL;
	pEndTrack = NULL;

	float fCurAlpha, fNextAlpha, fOffSet;
	fCurAlpha = 0.0f;
	fNextAlpha = 0.0f;
	if (pMesh->m_pVisTrack.size())
	{
		// pStartTrack를 찾을수 있으면
		if (GetAnimationTrack(fFrameTick, pMesh->m_pVisTrack, &pStartTrack, &pEndTrack))
		{
			fCurAlpha = pStartTrack->vVector.x;
			fStartTick = (float)pStartTrack->iTick;
		}
		if (pEndTrack)
		{
			fNextAlpha = pEndTrack->vVector.x;
			fEndTick = (float)pEndTrack->iTick;

			fOffSet = (fFrameTick - fStartTick) / (fEndTick - fStartTick);
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
	D3DXMatrixMultiply(&pMesh->m_matCalculation, &matAnim, matParents);

	// 인버스 매트릭스 확인 코드.
	TVector3 v0, v1, v2, v3;
	v0 = pMesh->m_matCalculation.m[0];
	v1 = pMesh->m_matCalculation.m[1];
	v2 = pMesh->m_matCalculation.m[2];
	D3DXVec3Cross(&v3, &v1, &v2);
	if (D3DXVec3Dot(&v3, &v0) < 0.0f)
	{
		TMatrix matW;
		D3DXMatrixScaling(&matW, -1.0f, -1.0f, -1.0f);
		D3DXMatrixMultiply(&pMesh->m_matCalculation,
			&pMesh->m_matCalculation, &matW);
	}
}
// pEndTrack 트랙이 없으면 flase 리턴( 보간할 대상이 없을 때 )
bool TAseObj::GetAnimationTrack(float fFrame,
	vector<shared_ptr<TAnimTrack>> pTrackList,
	TAnimTrack** pStartTrack,
	TAnimTrack** pEndTrack)
{
	for (DWORD dwTrack = 0; dwTrack < pTrackList.size(); dwTrack++)
	{
		TAnimTrack *pTrack = pTrackList[dwTrack].get();
		_ASSERT(pTrack);
		// fFrame 보다 큰 Tick 트랙이 있다면 이전 트랙을 넘겨 주어야 하기 때문에 break한다.
		if (pTrack->iTick > fFrame)
		{
			*pEndTrack = pTrack;
			break;
		}
		// 현프레임보다 큰값이 없다면. 가장 시간경과가 오래된 것을 사용한다.
		*pStartTrack = pTrack;
	}
	return (*pStartTrack != NULL) ? true : false;
}
bool TAseObj::Init()
{
	return true;
}

bool TAseObj::Frame()
{
	m_fElapseTime += g_fSecPerFrame * m_fTickSpeed * m_Scene.iFrameSpeed * m_Scene.iTickPerFrame*0.1f;
	if (m_fElapseTime >= m_Scene.iLastFrame* m_Scene.iTickPerFrame)
	{
		m_fElapseTime = (float)(m_Scene.iFirstFrame * m_Scene.iTickPerFrame);
	}

	TMatrix mat;
	D3DXMatrixIdentity(&mat);
	for (DWORD dwObject = 0; dwObject < m_pMesh.size(); dwObject++)
	{
		TMesh* pMesh = m_pMesh[dwObject].get();
		if (pMesh->m_bUsed == false) continue;
		if (pMesh->m_pParent)
			Interpolate(pMesh, &pMesh->m_pParent->m_matCalculation, m_fElapseTime);
		else
			Interpolate(pMesh, &mat, m_fElapseTime);
	}	
	return true;
}
bool TAseObj::Render(ID3D11DeviceContext*    pContext)
{
	PreRender(pContext);
	Draw(pContext, this);	
	return true;
}
bool TAseObj::Release()
{
	return true;
}
bool TAseObj::Convert(ID3D11Device* pDevice)
{
	for (DWORD dwObject = 0; dwObject < m_Parser.m_pAseMesh.size(); dwObject++)
	{
		auto pMesh = make_shared<TMesh>();
		auto pData = make_shared<tAseData>();
		pMesh->m_iIndex = dwObject;
		pMesh->m_matWorld = m_Parser.m_pAseMesh[dwObject]->m_matWorld;		// 월드행렬	
		pMesh->m_iNumFace = m_Parser.m_pAseMesh[dwObject]->m_iNumFace;		// 전체페이스 갯수					
		pMesh->m_strNodeName = m_Parser.m_pAseMesh[dwObject]->m_strNodeName;
		pMesh->m_strParentName = m_Parser.m_pAseMesh[dwObject]->m_strParentName;

		pMesh->m_matCalculation = m_Parser.m_pAseMesh[dwObject]->m_matWorld;
		pMesh->m_matWorldTrans = m_Parser.m_pAseMesh[dwObject]->m_matWorldTrans;
		pMesh->m_matWorldRotate = m_Parser.m_pAseMesh[dwObject]->m_matWorldRotate;
		pMesh->m_matWorldScale = m_Parser.m_pAseMesh[dwObject]->m_matWorldScale;

		D3DXMatrixInverse(&pMesh->m_matInverse, NULL, &pMesh->m_matWorld);

		int iRootMtrRef = m_Parser.m_pAseMesh[dwObject]->m_iMtrlRef;

		TMtrl* pMtrl = NULL;
		if (iRootMtrRef >= 0 && iRootMtrRef < m_Material.size())
		{
			pMtrl = &m_Material[iRootMtrRef];
			_ASSERT(pMtrl);
		}
		CStopwatch watch;
		if (pMesh->m_iNumFace > 0)
		{
			SetTriangleBuffer(pMesh.get(), pData.get(), m_Parser.m_pAseMesh[dwObject].get(), 0xff);
			if(pMtrl->m_SubMaterial.size() > 0 )
			{
				std::sort(pData->m_TriList.begin(), pData->m_TriList.end(), DescendingTriSort<PNCT_VERTEX>());
			}

			if (pMtrl == NULL || pMtrl->m_SubMaterial.size() <= 0)
			{
				// 2번 인자값=-1  Face Count(_countof )를 계산하지 않는다.
				pData->SetUniqueBuffer(pData->m_TriList, -1, 0);
				pMesh->m_iDiffuseTex = -1;
				if (pMtrl)	pMesh->m_iDiffuseTex = GetMapID(pMtrl, ID_TBASIS_DI);
				m_iMaxVertex += (UINT)pData->m_VertexArray.size();
				m_iMaxIndex +=(UINT) pData->m_IndexArray.size();
			}
			else
			{
				int iAddCount = 0;
				for (int iSub = 0; iSub < pMtrl->m_SubMaterial.size(); iSub++)
				{
					auto pSubMesh = make_shared<TMesh>();
					auto pSubData = make_shared<tAseData>();

					iAddCount += pSubData->SetUniqueBuffer(pData->m_TriList, iSub, iAddCount);

					TMtrl* pSubMtrl = &pMtrl->m_SubMaterial[iSub];
					_ASSERT(pSubMtrl);
					pSubMesh->m_iDiffuseTex = -1;
					if (pSubMtrl)
						pSubMesh->m_iDiffuseTex = GetMapID(pSubMtrl, ID_TBASIS_DI);

					m_iMaxVertex += (UINT)pSubData->m_VertexArray.size();
					m_iMaxIndex += (UINT)pSubData->m_IndexArray.size();

					pSubData->m_iNumFace = (int)(pSubData->m_IndexArray.size() / 3);
					pSubMesh->m_iNumFace = (int)(pSubData->m_IndexArray.size() / 3);

					if (pSubData->m_VertexArray.size() > 0)
					{
						pMesh->m_pSubMesh.push_back(pSubMesh);
						pData->m_pSubMesh.push_back(pSubData);
					}
				}
			}
		}
		watch.Output(L"Sub Mesh");
		// 에니메이션 트랙 복사		
		TAnimTrack* pPrevTrack = NULL;
		for (int iTrack = 0; iTrack < m_Parser.m_pAseMesh[dwObject]->m_PosTrack.size(); iTrack++)
		{
			auto pTrack = make_shared<TAnimTrack>();
			pTrack->iTick = m_Parser.m_pAseMesh[dwObject]->m_PosTrack[iTrack].iTick;
			pTrack->vVector = m_Parser.m_pAseMesh[dwObject]->m_PosTrack[iTrack].vVector;

			pPrevTrack = SetDoublyLinkedList(pTrack.get(), pPrevTrack);
			pMesh->m_pPosTrack.push_back(pTrack);
		}

		pPrevTrack = NULL;
		for (int iTrack = 0; iTrack < m_Parser.m_pAseMesh[dwObject]->m_RotTrack.size(); iTrack++)
		{
			auto pTrack = make_shared<TAnimTrack>();

			pTrack->iTick = m_Parser.m_pAseMesh[dwObject]->m_RotTrack[iTrack].iTick;

			// 임의의 축과 각을 쿼터니언으로 변환			
			D3DXQuaternionRotationAxis(&pTrack->qRotate, &TVector3(m_Parser.m_pAseMesh[dwObject]->m_RotTrack[iTrack].qRotate.x,
				m_Parser.m_pAseMesh[dwObject]->m_RotTrack[iTrack].qRotate.y,
				m_Parser.m_pAseMesh[dwObject]->m_RotTrack[iTrack].qRotate.z),
				m_Parser.m_pAseMesh[dwObject]->m_RotTrack[iTrack].qRotate.w);
			// 이전트랙의 쿼터니온과 누적시킴.			
			if (pPrevTrack != NULL)
			{
				D3DXQuaternionMultiply(&pTrack->qRotate, &pPrevTrack->qRotate, &pTrack->qRotate);
			}

			pPrevTrack = SetDoublyLinkedList(pTrack.get(), pPrevTrack);

			pMesh->m_pRotTrack.push_back(pTrack);
		}

		pPrevTrack = NULL;
		for (int iTrack = 0; iTrack < m_Parser.m_pAseMesh[dwObject]->m_SclTrack.size(); iTrack++)
		{
			auto pTrack = make_shared<TAnimTrack>();

			pTrack->iTick = m_Parser.m_pAseMesh[dwObject]->m_SclTrack[iTrack].iTick;
			pTrack->vVector = m_Parser.m_pAseMesh[dwObject]->m_SclTrack[iTrack].vVector;

			// 임의의 축과 각을 쿼터니언으로 변환			
			D3DXQuaternionRotationAxis(&pTrack->qRotate,
				&TVector3(m_Parser.m_pAseMesh[dwObject]->m_SclTrack[iTrack].qRotate.x,
					m_Parser.m_pAseMesh[dwObject]->m_SclTrack[iTrack].qRotate.y,
					m_Parser.m_pAseMesh[dwObject]->m_SclTrack[iTrack].qRotate.z),
				m_Parser.m_pAseMesh[dwObject]->m_SclTrack[iTrack].qRotate.w);

			pPrevTrack = SetDoublyLinkedList(pTrack.get(), pPrevTrack);

			pMesh->m_pSclTrack.push_back(pTrack);
		}

		// 에니메이션 트랙 복사		
		pPrevTrack = NULL;
		for (int iTrack = 0; iTrack < m_Parser.m_pAseMesh[dwObject]->m_VisTrack.size(); iTrack++)
		{
			auto pTrack = make_shared<TAnimTrack>();
			pTrack->iTick = m_Parser.m_pAseMesh[dwObject]->m_VisTrack[iTrack].iTick;
			pTrack->vVector = m_Parser.m_pAseMesh[dwObject]->m_VisTrack[iTrack].vVector;

			pPrevTrack = SetDoublyLinkedList(pTrack.get(), pPrevTrack);
			pMesh->m_pVisTrack.push_back(pTrack);
		}
		m_pMesh.push_back(pMesh);
		m_pData.push_back(pData);
	}
	m_Scene.iNumMesh =(int) m_pMesh.size();
	return InheriteCollect();
}
TAnimTrack* TAseObj::SetDoublyLinkedList(TAnimTrack* pCurrentTrack, TAnimTrack* pPrev)
{
	if (pPrev)
	{
		pPrev->pNext = pCurrentTrack;
		pCurrentTrack->pPrev = pPrev;
	}
	return pCurrentTrack;
}

void TAseObj::SetTriangleBuffer(TMesh* pMesh, tAseData* pData, TAseMesh* pAseMesh,	DWORD dwMask)
{
	TMatrix matInverse;
	D3DXMatrixInverse(&matInverse, NULL, &pAseMesh->m_matWorld);

	pData->m_TriList.clear();
	pData->m_TriList.reserve(pAseMesh->m_iNumFace);
	pData->m_TriList.resize(pAseMesh->m_iNumFace);
	pMesh->m_iNumFace = pAseMesh->m_iNumFace;

	TVector3 vVertex;
	pMesh->m_BBox.vMin = TVector3(999999.0f, 999999.0f, 999999.0f);
	pMesh->m_BBox.vMax = TVector3(-999999.0f, -999999.0f, -999999.0f);

	int Index = 0;
	for (DWORD dwFace = 0; dwFace < pAseMesh->m_iNumFace; dwFace++)
	{
		DWORD dwMtrl = dwMask & pAseMesh->m_PosVertexList.pFaceList[dwFace].dwMtrl;
		pData->m_TriList[dwFace].iSubIndex = dwMtrl;
		if (pAseMesh->m_NorVertexList.pVertexList.size() > 0)
		{
			pData->m_TriList[dwFace].vNormal = pAseMesh->m_NorVertexList.pVertexList[dwFace * 4];
		}

		for (DWORD dwVertex = 0; dwVertex < 3; dwVertex++)
		{
			if (pAseMesh->m_PosVertexList.dwNumVertex >0 && pAseMesh->m_PosVertexList.dwNumFace >0)
			{
				pData->m_TriList[dwFace].vVertex[dwVertex].p =
					pAseMesh->m_PosVertexList.pVertexList[pAseMesh->m_PosVertexList.pFaceList[dwFace].v[dwVertex]];
			}

			if (pAseMesh->m_NorVertexList.pVertexList.size() > 0)
			{
				// 4 = FaceNormal[0], V0-Normal[1], V0-Normal[1], V0-Normal[2]
				pData->m_TriList[dwFace].vVertex[dwVertex].n = pAseMesh->m_NorVertexList.pVertexList[dwFace * 4 + dwVertex + 1];
			}

			if (pAseMesh->m_ColVertexList.dwNumVertex && pAseMesh->m_ColVertexList.dwNumFace)
			{
				// AseFile의 R G B => Load된 정보는 R B G로 저장되어 있다.
				pData->m_TriList[dwFace].vVertex[dwVertex].c.x = pAseMesh->m_ColVertexList.pVertexList[pAseMesh->m_ColVertexList.pFaceList[dwFace].v[dwVertex]].x;
				pData->m_TriList[dwFace].vVertex[dwVertex].c.y = pAseMesh->m_ColVertexList.pVertexList[pAseMesh->m_ColVertexList.pFaceList[dwFace].v[dwVertex]].z;
				pData->m_TriList[dwFace].vVertex[dwVertex].c.z = pAseMesh->m_ColVertexList.pVertexList[pAseMesh->m_ColVertexList.pFaceList[dwFace].v[dwVertex]].y;
				pData->m_TriList[dwFace].vVertex[dwVertex].c.w = 1.0f;
			}
			else
			{
				pData->m_TriList[dwFace].vVertex[dwVertex].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
			}

			if (pAseMesh->m_TexVertexList.dwNumVertex && pAseMesh->m_TexVertexList.dwNumFace)
			{
				float fV = pAseMesh->m_TexVertexList.pVertexList[pAseMesh->m_TexVertexList.pFaceList[dwFace].v[dwVertex]].x;
				//fV = fV * 2.0f;
				// AseFile의 U V W => Load된 정보는 U W V로 저장되어 있다.
				pData->m_TriList[dwFace].vVertex[dwVertex].t.x = fV;
				pData->m_TriList[dwFace].vVertex[dwVertex].t.y = 1.0f - pAseMesh->m_TexVertexList.pVertexList[pAseMesh->m_TexVertexList.pFaceList[dwFace].v[dwVertex]].z;
			}

			// 로칼 정점 = 월드 정점 * 월드행렬의 역행렬
			D3DXVec3TransformCoord(&pData->m_TriList[dwFace].vVertex[dwVertex].p,
				&pData->m_TriList[dwFace].vVertex[dwVertex].p,
				&matInverse);
			// 바운딩 박스 계산
			vVertex = pData->m_TriList[dwFace].vVertex[dwVertex].p;
			if (pMesh->m_BBox.vMin.x > vVertex.x) pMesh->m_BBox.vMin.x = vVertex.x;
			if (pMesh->m_BBox.vMin.y > vVertex.y) pMesh->m_BBox.vMin.y = vVertex.y;
			if (pMesh->m_BBox.vMin.z > vVertex.z) pMesh->m_BBox.vMin.z = vVertex.z;

			if (pMesh->m_BBox.vMax.x < vVertex.x) pMesh->m_BBox.vMax.x = vVertex.x;
			if (pMesh->m_BBox.vMax.y < vVertex.y) pMesh->m_BBox.vMax.y = vVertex.y;
			if (pMesh->m_BBox.vMax.z < vVertex.z) pMesh->m_BBox.vMax.z = vVertex.z;
		}
	}
	
}
int	TAseObj::GetMapID(TMtrl* pMtrl, int iTexMapType)
{
	_ASSERT(pMtrl);
	if (pMtrl->m_TexMaps.size() <= 0) return -1;

	for (int iTexmap = 0; iTexmap < pMtrl->m_TexMaps.size(); iTexmap++)
	{
		if (pMtrl->m_TexMaps[iTexmap].m_dwType == iTexMapType)
		{
			return pMtrl->m_TexMaps[iTexmap].m_dwIndex;
		}
	}
	return -1;
}

HRESULT TAseObj::LoadTextures(ID3D11Device* pDevice, const TCHAR* pLoadTextureString)
{
	HRESULT hr = S_OK;
	for (DWORD dwObject = 0; dwObject < m_pMesh.size(); dwObject++)
	{
		TMesh* pMesh = m_pMesh[dwObject].get();
		if (pMesh->m_pSubMesh.size() > 0)
		{
			for (DWORD dwSub = 0; dwSub < pMesh->m_pSubMesh.size(); dwSub++)
			{
				TMesh* pSubMesh = pMesh->m_pSubMesh[dwSub].get();
				if (pSubMesh->m_iDiffuseTex < 0) continue;
				pSubMesh->m_dxobj.g_pTextureSRV = I_Texture.GetPtr(pSubMesh->m_iDiffuseTex)->m_pTextureSRV;

			}
		}
		else
		{
			if (pMesh->m_iDiffuseTex < 0) continue;
			pMesh->m_dxobj.g_pTextureSRV = I_Texture.GetPtr(pMesh->m_iDiffuseTex)->m_pTextureSRV;

		}
	}
	return hr;
}
HRESULT TAseObj::SetInputLayout()
{
	HRESULT hr = S_OK;
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	m_dxobj.g_pInputlayout.Attach(DX::CreateInputlayout(m_pd3dDevice,
			(DWORD)m_dxobj.g_pVSBlob.Get()->GetBufferSize(),
			m_dxobj.g_pVSBlob.Get()->GetBufferPointer(), layout, numElements));
	return hr;
}
bool TAseObj::ResetResource()
{
	return true;
}
bool TAseObj::InheriteCollect()
{
	TMatrix m_matInverse;
	TQuaternion qR;
	TVector3 vTrans, vScale;
	for (DWORD dwObject = 0; dwObject < m_pMesh.size(); dwObject++)
	{
		if (!m_pMesh[dwObject]->m_strParentName.empty())
		{
			TMesh* pParentNode = SearchToCollects(m_pMesh[dwObject]->m_strParentName);
			if (pParentNode)
			{
				m_pMesh[dwObject]->m_pParent = pParentNode;

				m_matInverse = m_pMesh[dwObject]->m_matWorld * pParentNode->m_matInverse;
				D3DXMatrixDecompose(&vScale, &qR, &vTrans, &m_matInverse);
				D3DXMatrixScaling(&m_pMesh[dwObject]->m_matWorldScale, vScale.x, vScale.y, vScale.z);
				D3DXMatrixTranslation(&m_pMesh[dwObject]->m_matWorldTrans, vTrans.x, vTrans.y, vTrans.z);
				D3DXMatrixRotationQuaternion(&m_pMesh[dwObject]->m_matWorldRotate, &qR);

				pParentNode->m_pChildMesh.push_back(m_pMesh[dwObject].get());
			}
		}
	}
	// 사용되지 않은 오브젝트 삭제
	for (DWORD dwObject = 0; dwObject < m_pMesh.size(); dwObject++)
	{
		if (m_pMesh[dwObject]->m_pChildMesh.size() <= 0 &&
			(m_pMesh[dwObject]->m_ClassType == CLASS_BONE || m_pMesh[dwObject]->m_ClassType == CLASS_DUMMY))
		{
			m_pMesh[dwObject]->m_bUsed = false;
		}
	}
	return true;
}
TMesh* TAseObj::SearchToCollects(T_STR	m_strParentName)
{
	for (DWORD dwGroup = 0; dwGroup < m_pData.size(); dwGroup++)
	{
		if (m_pMesh[dwGroup]->m_strNodeName == m_strParentName)
		{
			return m_pMesh[dwGroup].get();
		}
	}
	return NULL;
}
bool TAseObj::UpdateBuffer()
{
	if (!CombineBuffer(m_dxobj.g_pVertexBuffer.Get(), m_dxobj.g_pIndexBuffer.Get()))
	{
		return false;
	}	
	return true;
}
bool TAseObj::CombineBuffer(ID3D11Buffer* pVB, ID3D11Buffer* pIB)
{
	size_t dstOffset = 0;
	size_t vbOffset = 0;
	size_t dstibOffset = 0;
	size_t ibOffset = 0;
	{
		for (DWORD dwObject = 0; dwObject < m_pMesh.size(); dwObject++)
		{
			auto pMesh = m_pMesh[dwObject].get();
			auto pData = m_pData[dwObject].get();
			if (pMesh->m_pSubMesh.size() > 0)
			{
				for (DWORD dwSub = 0; dwSub < pMesh->m_pSubMesh.size(); dwSub++)
				{
					auto pSubMesh = pMesh->m_pSubMesh[dwSub].get();
					auto pSubData = pData->m_pSubMesh[dwSub].get();
					if (pSubData->m_VertexArray.size() < 3) continue;

					pSubMesh->m_dxobj.m_iNumVertex = (UINT)pSubData->m_VertexArray.size();
					pSubMesh->m_dxobj.m_iVertexSize = m_dxobj.m_iVertexSize;

					pSubMesh->m_dxobj.m_BoxVB.left = (UINT)dstOffset;
					pSubMesh->m_dxobj.m_BoxVB.right = (UINT)(dstOffset + pSubMesh->m_dxobj.m_iNumVertex*m_dxobj.m_iVertexSize);
					pSubMesh->m_dxobj.m_BoxVB.top = 0; pSubMesh->m_dxobj.m_BoxVB.bottom = 1;
					pSubMesh->m_dxobj.m_BoxVB.front = 0; pSubMesh->m_dxobj.m_BoxVB.back = 1;

					g_pImmediateContext->UpdateSubresource(
						pVB, 0,
						&pSubMesh->m_dxobj.m_BoxVB,
						(uint8_t*)&pSubData->m_VertexArray.at(0),
						0, 0);


					/*g_pImmediateContext->CopySubresourceRegion(
					m_dxobj.g_pVertexBuffer.Get(), 0, iBeginPos, 0, 0,
					(void*)&pSubMesh->m_VertexArray.at(0),
					0, &pSubMesh->m_dxobj.m_BoxVB);*/

					pSubMesh->m_dxobj.m_iBeginVB = (UINT)vbOffset;
					vbOffset += pSubMesh->m_dxobj.m_iNumVertex;

					dstOffset = pSubMesh->m_dxobj.m_BoxVB.right;

					pSubMesh->m_dxobj.m_iNumIndex = (UINT)pSubData->m_IndexArray.size();
					pSubMesh->m_dxobj.m_BoxIB.left = (UINT)dstibOffset;
					pSubMesh->m_dxobj.m_BoxIB.right = (UINT)(dstibOffset + pSubMesh->m_dxobj.m_iNumIndex*sizeof(DWORD));
					pSubMesh->m_dxobj.m_BoxIB.top = 0;		pSubMesh->m_dxobj.m_BoxIB.bottom = 1;
					pSubMesh->m_dxobj.m_BoxIB.front = 0;	pSubMesh->m_dxobj.m_BoxIB.back = 1;

					g_pImmediateContext->UpdateSubresource(pIB, 0,
						&pSubMesh->m_dxobj.m_BoxIB, (void*)&pSubData->m_IndexArray.at(0), 0, 0);

					pSubMesh->m_dxobj.m_iBeginIB = (UINT)ibOffset;
					ibOffset += pSubMesh->m_dxobj.m_iNumIndex;
					dstibOffset = pSubMesh->m_dxobj.m_BoxIB.right;

					//texture
					if (pSubMesh->m_iDiffuseTex < 0) continue;
					pSubMesh->m_dxobj.g_pTextureSRV = I_Texture.GetPtr(pSubMesh->m_iDiffuseTex)->m_pTextureSRV;
				}
			}
			else
			{
				if (pData->m_VertexArray.size() < 3) continue;
				
				pMesh->m_dxobj.m_iNumVertex = (UINT)pData->m_VertexArray.size();
				pMesh->m_dxobj.m_iVertexSize = m_dxobj.m_iVertexSize;

				pMesh->m_dxobj.m_BoxVB.left = (UINT)dstOffset;
				pMesh->m_dxobj.m_BoxVB.right =(UINT)(dstOffset + pMesh->m_dxobj.m_iNumVertex*m_dxobj.m_iVertexSize);
				pMesh->m_dxobj.m_BoxVB.top = 0; pMesh->m_dxobj.m_BoxVB.bottom = 1;
				pMesh->m_dxobj.m_BoxVB.front = 0; pMesh->m_dxobj.m_BoxVB.back = 1;

				g_pImmediateContext->UpdateSubresource(pVB, 0,
					&pMesh->m_dxobj.m_BoxVB, (void*)&pData->m_VertexArray.at(0), 0, 0);

				pMesh->m_dxobj.m_iBeginVB = (UINT)vbOffset;
				vbOffset += pMesh->m_dxobj.m_iNumVertex;

				dstOffset = pMesh->m_dxobj.m_BoxVB.right;

				pMesh->m_dxobj.m_iNumIndex = (UINT)pData->m_IndexArray.size();
				pMesh->m_dxobj.m_BoxIB.left = (UINT)dstibOffset;
				pMesh->m_dxobj.m_BoxIB.right = (UINT)(dstibOffset + pMesh->m_dxobj.m_iNumIndex*sizeof(DWORD));
				pMesh->m_dxobj.m_BoxIB.top = 0; pMesh->m_dxobj.m_BoxIB.bottom = 1;
				pMesh->m_dxobj.m_BoxIB.front = 0; pMesh->m_dxobj.m_BoxIB.back = 1;

				g_pImmediateContext->UpdateSubresource(pIB, 0,
					&pMesh->m_dxobj.m_BoxIB, (void*)&pData->m_IndexArray.at(0), 0, 0);

				pMesh->m_dxobj.m_iBeginIB = (UINT)ibOffset;
				ibOffset += pMesh->m_dxobj.m_iNumIndex;
				dstibOffset = pMesh->m_dxobj.m_BoxIB.right;

				

				//texture
				if (pMesh->m_iDiffuseTex < 0) continue;
				pMesh->m_dxobj.g_pTextureSRV = I_Texture.GetPtr(pMesh->m_iDiffuseTex)->m_pTextureSRV;
			}
		}
	}
	return true;
}
bool TAseObj::Draw(ID3D11DeviceContext*    pContext, TActor* pParent)
{
	CStopwatch stopwatch;
	for (DWORD dwObject = 0; dwObject < m_pMesh.size(); dwObject++)
	{
		auto pMesh = m_pMesh[dwObject].get();
		pMesh->m_matCalculation = pMesh->m_matCalculation * m_matControlWorld;
		pParent->SetMatrix(&pMesh->m_matCalculation, &pParent->m_matView, &pParent->m_matProj);
		
		UpdateConstantBuffer(pContext, pParent);

		if (pMesh->m_pSubMesh.size() > 0)
		{
			for (DWORD dwSub = 0; dwSub < pMesh->m_pSubMesh.size(); dwSub++)
			{
				auto pSubMesh = pMesh->m_pSubMesh[dwSub].get();
				if (pSubMesh->m_iNumFace < 1) continue;
				pContext->PSSetShaderResources(0, 1, pSubMesh->m_dxobj.g_pTextureSRV.GetAddressOf());
				pContext->DrawIndexed(
					pSubMesh->m_dxobj.m_iNumIndex,
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
	//stopwatch.Output(L"\nTAseObj::Draw");
	return true;
}
TAseObj::TAseObj(void)
{
}

TAseObj::~TAseObj(void)
{
}
