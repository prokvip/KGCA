#include "TBoneObj.h"

#define MAX_BONE_MATRICES 255

void TBoneObj::SetBoneMatrices(ID3D11DeviceContext*    pContext)
{
	TMatrix* pMatrices;
	HRESULT hr = S_OK;
	D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
	if (SUCCEEDED(pContext->Map((ID3D11Resource*)m_pBoneBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedFaceDest)))
	{
		pMatrices = (TMatrix*)MappedFaceDest.pData;
		for (int dwObject = 0; dwObject < m_Scene.iNumMesh; dwObject++)
		{
			pMatrices[dwObject] = m_pMatrix[dwObject];
		}
		pContext->Unmap(m_pBoneBuffer, 0);
	}
}
void TBoneObj::SetBoneMatrices(ID3D11DeviceContext*    pContext, TMatrix* pMatrix)
{
	TMatrix* pMatrices;
	HRESULT hr = S_OK;
	D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
	if (SUCCEEDED(pContext->Map((ID3D11Resource*)m_pBoneBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedFaceDest)))
	{
		pMatrices = (TMatrix*)MappedFaceDest.pData;
		for (int dwObject = 0; dwObject < m_Scene.iNumMesh; dwObject++)
		{
			pMatrices[dwObject] = pMatrix[dwObject];
		}
		pContext->Unmap(m_pBoneBuffer, 0);
	}
}
void TBoneObj::SetBoneMatrices(ID3D11DeviceContext*    pContext, TMatrix* pMatrix, vector<TMatrix>* pList)
{
	TMatrix* pMatrices;
	HRESULT hr = S_OK;
	D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
	if (SUCCEEDED(pContext->Map((ID3D11Resource*)m_pBoneBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedFaceDest)))
	{
		pMatrices = (TMatrix*)MappedFaceDest.pData;
		for (int iObject = 0; iObject < m_Scene.iNumMesh; iObject++)
		{
			pMatrices[iObject] = (*pList)[iObject] * pMatrix[iObject];
		}
		pContext->Unmap(m_pBoneBuffer, 0);
	}
}
bool TBoneObj::AniFrame(FLOAT fCurFrame, FLOAT fElapsedTime, int iFirstFrame, int iLastFrame, TMatrix* pMatrix)
{
	if (pMatrix == NULL)
	{
		pMatrix = m_pMatrix;
	}

	bool bResult = false;
	TQuaternion TmpQuat;
	int iCurFrame = (int)fCurFrame;
	int iNextFrame = iCurFrame + 1;
	
	TVector3 vScale, vTrans;
	TMatrix matRotate, matScale, matTrans;
	D3DXMatrixIdentity(&matScale);
	for (int i = 0; i < m_Scene.iNumMesh; i++)
	{
		if (iNextFrame >= iLastFrame)
		{
			iCurFrame = iFirstFrame;
			TQuaternion	CurQuat = m_ppAniQuater[i][iCurFrame];
			TQuaternion	NextQuat = m_ppAniQuater[i][iCurFrame + 1];
			D3DXQuaternionSlerp(&TmpQuat, &CurQuat, &NextQuat, fElapsedTime);
			D3DXMatrixRotationQuaternion(&matRotate, &TmpQuat);
			
			//D3DXVec3Lerp(&vScale, &m_ppScaleVector[i][iCurFrame], &m_ppScaleVector[i][iCurFrame + 1], fElapsedTime);
			//D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z);			
			//D3DXVec3Lerp(&vTrans, &m_ppTransVector[i][iCurFrame], &m_ppTransVector[i][iCurFrame + 1], fElapsedTime);

			D3DXMatrixScaling(&matScale, m_ppScaleVector[i][iCurFrame].x, m_ppScaleVector[i][iCurFrame].y, m_ppScaleVector[i][iCurFrame].z);
			vTrans = m_ppTransVector[i][iCurFrame];

			pMatrix[i] = matScale * matRotate;
			pMatrix[i]._41 = vTrans.x;
			pMatrix[i]._42 = vTrans.y;
			pMatrix[i]._43 = vTrans.z;
			
			bResult = true;
		}
		else
		{
			TQuaternion	CurQuat = m_ppAniQuater[i][iCurFrame];
			TQuaternion	NextQuat = m_ppAniQuater[i][iNextFrame];
			D3DXQuaternionSlerp(&TmpQuat, &CurQuat, &NextQuat, fElapsedTime);		
			D3DXMatrixRotationQuaternion(&matRotate, &TmpQuat);
			
			D3DXVec3Lerp(&vScale, &m_ppScaleVector[i][iCurFrame], &m_ppScaleVector[i][iNextFrame], fElapsedTime);			
			D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z);			
			D3DXVec3Lerp(&vTrans, &m_ppTransVector[i][iCurFrame], &m_ppTransVector[i][iNextFrame], fElapsedTime);			
		
			pMatrix[i] = matScale * matRotate;

			pMatrix[i]._41 = vTrans.x;
			pMatrix[i]._42 = vTrans.y;
			pMatrix[i]._43 = vTrans.z;
		}
	}
	return bResult;//( iCurFrame > m_iLastFrame ) ? TRUE : FALSE;	
}
bool TBoneObj::Load(ID3D11Device* pDevice,const TCHAR* szLoadName,const TCHAR* pLoadShaderFile,bool bThread)
{
	FILE	*fp;
	fp = _tfopen(szLoadName, _T("rb"));
	if (!fp) return false;

	char szBuffer[MAX_PATH] = { 0, };
	size_t convertedChars = 0;

	tm newtime;
	fread(&newtime, sizeof(tm), 1, fp);
	T_STR today = _tasctime(&newtime);	// The date string has a \n appended.
	today[today.size() - 1] = 0;
	fread(&m_Scene, sizeof(TScene), 1, fp);

	// 시작 프레임이 0일 아닐 경우가 있기 때문에 무조건 
	// 시작 프레임을 0으로 맞춘다.( 해당 프레임 만큼만 배열 할당 된다.)
	m_iStartFrame = 0;//m_Scene.iFirstFrame;
	m_iLastFrame = m_Scene.iLastFrame - m_Scene.iFirstFrame;
	m_fElapseTime = (float)m_iStartFrame;
	m_Scene.iFirstFrame = 0;
	m_Scene.iLastFrame = m_iLastFrame;

	SAFE_NEW_ARRAY_CLEAR(m_pMatrix, TMatrix, m_Scene.iNumMesh);
	SAFE_NEW_ARRAY_CLEAR(m_ppAniMatrix, TMatrix*, m_Scene.iNumMesh);
	SAFE_NEW_ARRAY_CLEAR(m_ppAniQuater, TQuaternion*, m_Scene.iNumMesh);
	SAFE_NEW_ARRAY_CLEAR(m_ppScaleVector, TVector3*, m_Scene.iNumMesh);
	SAFE_NEW_ARRAY_CLEAR(m_ppTransVector, TVector3*, m_Scene.iNumMesh);

	int iNumFrame = m_iLastFrame - m_iStartFrame;
	for (int i = 0; i < m_Scene.iNumMesh; i++)
	{
		SAFE_NEW_ARRAY(m_ppAniMatrix[i], TMatrix, iNumFrame);
		SAFE_NEW_ARRAY(m_ppAniQuater[i], TQuaternion, iNumFrame);
		SAFE_NEW_ARRAY(m_ppScaleVector[i], TVector3, iNumFrame);
		SAFE_NEW_ARRAY(m_ppTransVector[i], TVector3, iNumFrame);
	}

	TQuaternion qRotate;
	TVector3 vScale, vTrans;
	TMatrix matFrameWorld;

	for (int ibip = 0; ibip < m_Scene.iNumMesh; ibip++)
	{
		for (int iFrame = 0; iFrame < iNumFrame; iFrame++)
		{
			fread(&m_ppAniMatrix[ibip][iFrame], sizeof(TMatrix), 1, fp);
		}
	}

	for (int ibip = 0; ibip < m_Scene.iNumMesh; ibip++)
	{
		shared_ptr<tMatMesh> pData = make_shared<tMatMesh>();
		shared_ptr<TMesh> pMesh = make_shared<TMesh>();
		int iCount;

		TCHAR szBuffer[256] = { 0, };
		TCHAR szName[256] = { 0, };
		fread(&pMesh->m_ClassType, sizeof(int), 1, fp);
		fread(&iCount, sizeof(int), 1, fp);
		fread(szName, sizeof(TCHAR) * iCount, 1, fp);

		pMesh->m_strNodeName = szName;
		fread(&pMesh->m_matWorld, sizeof(TMatrix), 1, fp);
		D3DXMatrixInverse(&pData->m_matInverse, 0, &pMesh->m_matWorld);

		fread(&iCount, sizeof(int), 1, fp);
		// 메쉬 버텍스 리스트
		if (iCount == 2)
		{
			pData->m_VertexArray.resize(36);
			m_iMaxVertex += 36;
		}
		else
		{
			pData->m_VertexArray.resize(iCount);
			m_iMaxVertex += iCount;
		}

		for (int iVertex = 0; iVertex < iCount; iVertex++)
		{
			fread(&pData->m_VertexArray[iVertex], sizeof(PNC_VERTEX), 1, fp);
			if (m_Scene.iBindPose)
			{
				D3DXVec3TransformCoord(&pData->m_VertexArray[iVertex].p,
					&pData->m_VertexArray[iVertex].p,
					&pData->m_matInverse);
			}
		}

		// 본과 더미 오브젝트( 최대 및 최소값 2개 출력되어 있음 )
		if (iCount == 2)
		{
			TVector3 Quad[8];
			TVector3 vMax = pData->m_VertexArray[0].p;
			TVector3 vMin = pData->m_VertexArray[1].p;

			Quad[0] = TVector3(vMin.x, vMin.y, vMin.z);
			Quad[1] = TVector3(vMin.x, vMax.y, vMin.z);
			Quad[2] = TVector3(vMax.x, vMax.y, vMin.z);
			Quad[3] = TVector3(vMax.x, vMin.y, vMin.z);

			Quad[4] = TVector3(vMin.x, vMin.y, vMax.z);
			Quad[5] = TVector3(vMin.x, vMax.y, vMax.z);
			Quad[6] = TVector3(vMax.x, vMax.y, vMax.z);
			Quad[7] = TVector3(vMax.x, vMin.y, vMax.z);
			SetBoundBox(Quad, pData.get());
		}
		pMesh->m_iNumFace = (int)(pData->m_VertexArray.size() / 3);
		m_pMesh.push_back(pMesh);
		m_pData.push_back(pData);
	}

	for (int ibip = 0; ibip < m_Scene.iNumMesh; ibip++)
	{
		for (int jFrame = 0; jFrame < m_iLastFrame - m_iStartFrame; jFrame++)
		{
			if (m_Scene.iBindPose)
			{
				//m_ppAniMatrix[ibip][jFrame] = m_pData[ibip]->m_matWorld * m_ppAniMatrix[ibip][jFrame];
			}
			if (SUCCEEDED(D3DXMatrixDecompose(&vScale, &qRotate, &vTrans, &m_ppAniMatrix[ibip][jFrame])))
			{
				m_ppAniQuater[ibip][jFrame] = qRotate;
				m_ppScaleVector[ibip][jFrame] = vScale;
				m_ppTransVector[ibip][jFrame] = vTrans;
			}
			else
			{
				D3DXQuaternionRotationMatrix(&m_ppAniQuater[ibip][jFrame], &m_ppAniMatrix[ibip][jFrame]);
			}
		}
	}

	fclose(fp);

	m_dxobj.m_iNumVertex = m_iMaxVertex;
	m_dxobj.m_iNumIndex = m_iMaxIndex;
	m_dxobj.m_iVertexSize = sizeof(PNC_VERTEX);

	if (!bThread && !Create(pDevice, pLoadShaderFile))
	{
		return false;
	}
	return true;
}
bool TBoneObj::Convert(const TCHAR* strPathName)
{
	return true;
}
void TBoneObj::SetBoundBox(TVector3* Quad, tMatMesh* pMesh)
{
	pMesh->m_VertexArray[0].p.x = Quad[0].x; pMesh->m_VertexArray[0].p.y = Quad[0].y;
	pMesh->m_VertexArray[0].p.z = Quad[0].z; pMesh->m_VertexArray[0].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	pMesh->m_VertexArray[1].p.x = Quad[1].x; pMesh->m_VertexArray[1].p.y = Quad[1].y;
	pMesh->m_VertexArray[1].p.z = Quad[1].z; pMesh->m_VertexArray[1].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	pMesh->m_VertexArray[2].p.x = Quad[2].x; pMesh->m_VertexArray[2].p.y = Quad[2].y;
	pMesh->m_VertexArray[2].p.z = Quad[2].z; pMesh->m_VertexArray[2].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);

	pMesh->m_VertexArray[3].p.x = Quad[0].x; pMesh->m_VertexArray[3].p.y = Quad[0].y;
	pMesh->m_VertexArray[3].p.z = Quad[0].z; pMesh->m_VertexArray[3].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	pMesh->m_VertexArray[4].p.x = Quad[2].x; pMesh->m_VertexArray[4].p.y = Quad[2].y;
	pMesh->m_VertexArray[4].p.z = Quad[2].z; pMesh->m_VertexArray[4].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	pMesh->m_VertexArray[5].p.x = Quad[3].x; pMesh->m_VertexArray[5].p.y = Quad[3].y;
	pMesh->m_VertexArray[5].p.z = Quad[3].z; pMesh->m_VertexArray[5].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);



	pMesh->m_VertexArray[6].p.x = Quad[3].x; pMesh->m_VertexArray[6].p.y = Quad[3].y;
	pMesh->m_VertexArray[6].p.z = Quad[3].z; pMesh->m_VertexArray[6].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	pMesh->m_VertexArray[7].p.x = Quad[2].x; pMesh->m_VertexArray[7].p.y = Quad[2].y;
	pMesh->m_VertexArray[7].p.z = Quad[2].z; pMesh->m_VertexArray[7].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	pMesh->m_VertexArray[8].p.x = Quad[6].x; pMesh->m_VertexArray[8].p.y = Quad[6].y;
	pMesh->m_VertexArray[8].p.z = Quad[6].z; pMesh->m_VertexArray[8].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);


	pMesh->m_VertexArray[9].p.x = Quad[3].x; pMesh->m_VertexArray[9].p.y = Quad[3].y;
	pMesh->m_VertexArray[9].p.z = Quad[3].z; pMesh->m_VertexArray[9].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	pMesh->m_VertexArray[10].p.x = Quad[6].x; pMesh->m_VertexArray[10].p.y = Quad[6].y;
	pMesh->m_VertexArray[10].p.z = Quad[6].z; pMesh->m_VertexArray[10].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	pMesh->m_VertexArray[11].p.x = Quad[7].x; pMesh->m_VertexArray[11].p.y = Quad[7].y;
	pMesh->m_VertexArray[11].p.z = Quad[7].z; pMesh->m_VertexArray[11].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);

	// Back
	pMesh->m_VertexArray[12].p.x = Quad[7].x; pMesh->m_VertexArray[12].p.y = Quad[7].y;
	pMesh->m_VertexArray[12].p.z = Quad[7].z; pMesh->m_VertexArray[12].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	pMesh->m_VertexArray[13].p.x = Quad[6].x; pMesh->m_VertexArray[13].p.y = Quad[6].y;
	pMesh->m_VertexArray[13].p.z = Quad[6].z; pMesh->m_VertexArray[13].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	pMesh->m_VertexArray[14].p.x = Quad[5].x; pMesh->m_VertexArray[14].p.y = Quad[5].y;
	pMesh->m_VertexArray[14].p.z = Quad[5].z; pMesh->m_VertexArray[14].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);

	pMesh->m_VertexArray[15].p.x = Quad[7].x; pMesh->m_VertexArray[15].p.y = Quad[7].y;
	pMesh->m_VertexArray[15].p.z = Quad[7].z; pMesh->m_VertexArray[15].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	pMesh->m_VertexArray[16].p.x = Quad[5].x; pMesh->m_VertexArray[16].p.y = Quad[5].y;
	pMesh->m_VertexArray[16].p.z = Quad[5].z; pMesh->m_VertexArray[16].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	pMesh->m_VertexArray[17].p.x = Quad[4].x; pMesh->m_VertexArray[17].p.y = Quad[4].y;
	pMesh->m_VertexArray[17].p.z = Quad[4].z; pMesh->m_VertexArray[17].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);

	pMesh->m_VertexArray[18].p.x = Quad[4].x; pMesh->m_VertexArray[18].p.y = Quad[4].y;
	pMesh->m_VertexArray[18].p.z = Quad[4].z; pMesh->m_VertexArray[18].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	pMesh->m_VertexArray[19].p.x = Quad[5].x; pMesh->m_VertexArray[19].p.y = Quad[5].y;
	pMesh->m_VertexArray[19].p.z = Quad[5].z; pMesh->m_VertexArray[19].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	pMesh->m_VertexArray[20].p.x = Quad[1].x; pMesh->m_VertexArray[20].p.y = Quad[1].y;
	pMesh->m_VertexArray[20].p.z = Quad[1].z; pMesh->m_VertexArray[20].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);

	pMesh->m_VertexArray[21].p.x = Quad[4].x; pMesh->m_VertexArray[21].p.y = Quad[4].y;
	pMesh->m_VertexArray[21].p.z = Quad[4].z; pMesh->m_VertexArray[21].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	pMesh->m_VertexArray[22].p.x = Quad[1].x; pMesh->m_VertexArray[22].p.y = Quad[1].y;
	pMesh->m_VertexArray[22].p.z = Quad[1].z; pMesh->m_VertexArray[22].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	pMesh->m_VertexArray[23].p.x = Quad[0].x; pMesh->m_VertexArray[23].p.y = Quad[0].y;
	pMesh->m_VertexArray[23].p.z = Quad[0].z; pMesh->m_VertexArray[23].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);

	// Side
	pMesh->m_VertexArray[24].p.x = Quad[6].x; pMesh->m_VertexArray[24].p.y = Quad[6].y;
	pMesh->m_VertexArray[24].p.z = Quad[6].z; pMesh->m_VertexArray[24].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	pMesh->m_VertexArray[25].p.x = Quad[2].x; pMesh->m_VertexArray[25].p.y = Quad[2].y;
	pMesh->m_VertexArray[25].p.z = Quad[2].z; pMesh->m_VertexArray[25].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	pMesh->m_VertexArray[26].p.x = Quad[1].x; pMesh->m_VertexArray[26].p.y = Quad[1].y;
	pMesh->m_VertexArray[26].p.z = Quad[1].z; pMesh->m_VertexArray[26].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);

	pMesh->m_VertexArray[27].p.x = Quad[6].x; pMesh->m_VertexArray[27].p.y = Quad[6].y;
	pMesh->m_VertexArray[27].p.z = Quad[6].z; pMesh->m_VertexArray[27].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	pMesh->m_VertexArray[28].p.x = Quad[1].x; pMesh->m_VertexArray[28].p.y = Quad[1].y;
	pMesh->m_VertexArray[28].p.z = Quad[1].z; pMesh->m_VertexArray[28].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	pMesh->m_VertexArray[29].p.x = Quad[5].x; pMesh->m_VertexArray[29].p.y = Quad[5].y;
	pMesh->m_VertexArray[29].p.z = Quad[5].z; pMesh->m_VertexArray[29].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);

	pMesh->m_VertexArray[30].p.x = Quad[3].x; pMesh->m_VertexArray[30].p.y = Quad[3].y;
	pMesh->m_VertexArray[30].p.z = Quad[3].z; pMesh->m_VertexArray[30].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	pMesh->m_VertexArray[31].p.x = Quad[7].x; pMesh->m_VertexArray[31].p.y = Quad[7].y;
	pMesh->m_VertexArray[31].p.z = Quad[7].z; pMesh->m_VertexArray[31].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	pMesh->m_VertexArray[32].p.x = Quad[4].x; pMesh->m_VertexArray[32].p.y = Quad[4].y;
	pMesh->m_VertexArray[32].p.z = Quad[4].z; pMesh->m_VertexArray[32].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);

	pMesh->m_VertexArray[33].p.x = Quad[3].x; pMesh->m_VertexArray[33].p.y = Quad[3].y;
	pMesh->m_VertexArray[33].p.z = Quad[3].z; pMesh->m_VertexArray[33].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	pMesh->m_VertexArray[34].p.x = Quad[4].x; pMesh->m_VertexArray[34].p.y = Quad[4].y;
	pMesh->m_VertexArray[34].p.z = Quad[4].z; pMesh->m_VertexArray[34].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	pMesh->m_VertexArray[35].p.x = Quad[0].x; pMesh->m_VertexArray[35].p.y = Quad[0].y;
	pMesh->m_VertexArray[35].p.z = Quad[0].z; pMesh->m_VertexArray[35].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
}
HRESULT TBoneObj::SetInputLayout()
{
	// 레이아웃 생성
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	m_dxobj.g_pInputlayout.Attach(DX::CreateInputlayout(m_pd3dDevice,
		(DWORD)m_dxobj.g_pVSBlob.Get()->GetBufferSize(),
		m_dxobj.g_pVSBlob.Get()->GetBufferPointer(), layout, numElements));

	SetBuffer(m_pd3dDevice);

	return S_OK;
}
bool TBoneObj::SetBuffer(ID3D11Device* pd3dDevice)
{
	// Create a bone matrix buffer
	// It will be updated more than once per frame (in a typical game) so make it dynamic
	D3D11_BUFFER_DESC vbdesc =
	{
		MAX_BONE_MATRICES * sizeof(TMatrix),
		D3D11_USAGE_DYNAMIC,
		D3D11_BIND_SHADER_RESOURCE,
		D3D11_CPU_ACCESS_WRITE,
		0
	};
	pd3dDevice->CreateBuffer(&vbdesc, NULL, &m_pBoneBuffer);

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	// Again, we need a resource view to use it in the shader
	ZeroMemory(&SRVDesc, sizeof(SRVDesc));
	SRVDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	SRVDesc.Buffer.ElementOffset = 0;
	SRVDesc.Buffer.ElementWidth = MAX_BONE_MATRICES * 4;
	pd3dDevice->CreateShaderResourceView(m_pBoneBuffer, &SRVDesc, &m_pBoneBufferRV);
	return true;
}

bool TBoneObj::Init()
{
	return true;
}
bool TBoneObj::Frame()
{
	m_fElapseTime += (g_fSecPerFrame * m_fSpeed *  m_Scene.iFrameSpeed);
	m_iCurrentFrame = (int)m_fElapseTime;
	m_fLerpTime = m_fElapseTime - m_iCurrentFrame;// 1프레임 사이 간격

	//////////////////////////  보간 없이 투박하게////////////////////////////
	//if(m_iCurrentFrame >= m_iLastFrame)
	//{
	//	m_iCurrentFrame = m_iStartFrame;
	//	m_fElapseTime = (float)m_iStartFrame;
	//}
	//for (int i = 0; i < m_Scene.iNumMesh; i++)
	//{
	//	m_pMatrix[i] = m_ppAniMatrix[i][m_iCurrentFrame];
	//}
	//////////////////////  보간 하여////////////////////////////
	if (AniFrame((FLOAT)m_iCurrentFrame, m_fLerpTime, m_iStartFrame,	m_iLastFrame, m_pMatrix))
	{
		m_iCurrentFrame = m_iStartFrame;
		m_fElapseTime = (float)m_iStartFrame;// + fElapsedTime;		
	}
	SetBoneMatrices(g_pImmediateContext);
	return true;
}
bool TBoneObj::Render(ID3D11DeviceContext*    pContext)
{	
	PreRender(pContext);
	Draw(pContext, this);
	return true;
}
void TBoneObj::SetMatrix(TMatrix* pWorld, TMatrix* pView, TMatrix* pProj)
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
bool TBoneObj::UpdateBuffer()
{
	if (!CombineBuffer(m_dxobj.g_pVertexBuffer.Get(), m_dxobj.g_pIndexBuffer.Get()))
	{
		return false;
	}
	return true;
}
bool TBoneObj::CombineBuffer(ID3D11Buffer* pVB, ID3D11Buffer* pIB)
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
			if (pData->m_VertexArray.size() < 3) continue;

			pMesh->m_dxobj.m_iNumVertex = (UINT)pData->m_VertexArray.size();
			pMesh->m_dxobj.m_iVertexSize = m_dxobj.m_iVertexSize;

			pMesh->m_dxobj.m_BoxVB.left = (UINT)dstOffset;
			pMesh->m_dxobj.m_BoxVB.right = (UINT)(dstOffset + pMesh->m_dxobj.m_iNumVertex*m_dxobj.m_iVertexSize);
			pMesh->m_dxobj.m_BoxVB.top = 0; pMesh->m_dxobj.m_BoxVB.bottom = 1;
			pMesh->m_dxobj.m_BoxVB.front = 0; pMesh->m_dxobj.m_BoxVB.back = 1;

			g_pImmediateContext->UpdateSubresource(pVB, 0, &pMesh->m_dxobj.m_BoxVB, (void*)&pData->m_VertexArray.at(0), 0, 0);

			pMesh->m_dxobj.m_iBeginVB = (UINT)vbOffset;
			vbOffset += pMesh->m_dxobj.m_iNumVertex;
			dstOffset = pMesh->m_dxobj.m_BoxVB.right;			
			//texture
			if (pMesh->m_iDiffuseTex < 0) continue;
			pMesh->m_dxobj.g_pTextureSRV = I_Texture.GetPtr(pMesh->m_iDiffuseTex)->m_pTextureSRV;			
		}
	}
	return true;
}
bool TBoneObj::Draw(ID3D11DeviceContext*    pContext, TActor* pParent)
{
	//CStopwatch stopwatch;
	for (DWORD dwObject = 0; dwObject < m_pMesh.size(); dwObject++)
	{
		auto pMesh = m_pMesh[dwObject].get();
		if (pMesh->m_ClassType == CLASS_GEOM) continue;
		TMatrix matWorld = m_pMatrix[dwObject] * m_matWorld;
		D3DXMatrixTranspose(&m_cbData.matWorld, &matWorld);
		pContext->UpdateSubresource(pParent->m_dxobj.g_pConstantBuffer.Get(), 0, NULL, &pParent->m_cbData, 0, 0);
		
		if (pMesh->m_iNumFace < 1) continue;
		pContext->PSSetShaderResources(0, 1, pMesh->m_dxobj.g_pTextureSRV.GetAddressOf());
		pContext->Draw(pMesh->m_dxobj.m_iNumVertex, pMesh->m_dxobj.m_iBeginVB);		
	}
	//stopwatch.Output(L"\nTTbsObj::Draw");
	return true;
}
bool TBoneObj::Release()
{
	if (!m_pMatrix) return true;
	SAFE_DELETE_ARRAY(m_pMatrix);
	m_pMatrix = 0;

	for (int i = 0; i < m_Scene.iNumMesh; i++)
	{
		if (m_ppAniMatrix)	SAFE_DELETE_ARRAY(m_ppAniMatrix[i]);
		if (m_ppAniQuater)	SAFE_DELETE_ARRAY(m_ppAniQuater[i]);
		if (m_ppScaleVector)	SAFE_DELETE_ARRAY(m_ppScaleVector[i]);
		if (m_ppTransVector)	SAFE_DELETE_ARRAY(m_ppTransVector[i]);
	}
	if (m_ppAniMatrix)		SAFE_DELETE_ARRAY(m_ppAniMatrix);
	if (m_ppAniQuater)		SAFE_DELETE_ARRAY(m_ppAniQuater);
	if (m_ppScaleVector)	SAFE_DELETE_ARRAY(m_ppScaleVector);
	if (m_ppTransVector)	SAFE_DELETE_ARRAY(m_ppTransVector);

	SAFE_RELEASE(m_pBoneBuffer);
	SAFE_RELEASE(m_pBoneBufferRV);
	return true;
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
TBoneObj::TBoneObj()
{
	m_fSpeed = 1.0f;
	m_pMatrix = NULL;
	m_ppAniMatrix = NULL;
	m_ppAniQuater = NULL;
	m_ppScaleVector = NULL;
	m_ppTransVector = NULL;
	m_fLerpTime = 0.0f;
	m_pBoneBuffer = NULL;
	m_pBoneBufferRV = NULL;
}
TBoneObj::~TBoneObj()
{

}
