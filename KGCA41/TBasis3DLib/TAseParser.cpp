#include "TAseParser.h"
#include "TTextureMgr.h"
#include "TTimer.h"

bool TAseParser::Load(const TCHAR* strFileName)
{
	return LoadBuffer(strFileName);
}

bool TAseParser::LoadScene(TScene& tScene)
{
	GetData(_T("SCENE_FIRSTFRAME"), &tScene.iFirstFrame, INT_DATA);
	GetData(_T("SCENE_LASTFRAME"), &tScene.iLastFrame, INT_DATA);
	GetData(_T("SCENE_FRAMESPEED"), &tScene.iFrameSpeed, INT_DATA);
	GetData(_T("SCENE_TICKSPERFRAME"), &tScene.iTickPerFrame, INT_DATA);
	return true;
}
bool TAseParser::LoadMaterialList(vector<TMtrl>&	tMaterialList)
{
	int		iMaterialCount = 0;
	GetData(_T("MATERIAL_COUNT"), &iMaterialCount, INT_DATA);

	for (int i = 0; i<iMaterialCount; i++)
	{
		TMtrl Material;
		if (LoadMaterial(&Material))
		{
			tMaterialList.push_back(Material);
		}
	}
	return true;
}
//2012-12-05 수정
//static TCHAR* AseMaterialTokens[] = { _T("NUMSUBMTLS"),	_T("MATERIAL "),_T("SUBMATERIAL"),_T("}"), };	
//enum AseMaterialType				{ NUM_SUBMTRLS = 10,MATERIAL,		SUB_MATERIAL, };	

bool TAseParser::LoadMaterial(TMtrl*	pMaterial)
{
	if (!pMaterial) return false;
	int iObjectType = -1;
	int iNumTypes = sizeof(AseMaterialTokens) / sizeof(AseMaterialTokens[0]);

	TCHAR szTexName[256] = _T("");
	TMtrl*			pCurrentMtrl = NULL;
	TTextexMap*	pCurrentTexMap = NULL;

	GetData(_T("MATERIAL_NAME"), &szTexName, STRING_DATA);
	pMaterial->m_strName = szTexName;

	GetData(_T("MATERIAL_CLASS"), &szTexName, STRING_DATA);
	pMaterial->m_strClassName = szTexName;

	while ((iObjectType = SearchTokenArray(iNumTypes, AseMaterialTokens)) >= 0)
	{
		switch (iObjectType)
		{
		case NUM_SUBMTRLS:
		{
			int		iSubMaterialCount = 0;
			GetData(&iSubMaterialCount, INT_DATA);

			for (int i = 0; i<iSubMaterialCount; i++)
			{
				TMtrl SubMaterial;
				if (LoadMaterial(&SubMaterial))
				{
					pMaterial->m_SubMaterial.push_back(SubMaterial);
				}
			}
			return true;
		}break;
		case MAP_NAME:
		{			
			if (LoadTexture(pMaterial))
			{				
				return true;
			}
			return false;
		}break;
		default:
		{
			m_dwTokenIndex--;
			return true;
		}break;
		}
	}
	return false;
}

//static TCHAR* AseTextureTokens[] =	{_T("MAP_NAME"),_T("MAP_SUBNO"),_T("BITMAP "), _T("}"), };	
//enum AseTextureType					{MAP_NAME=20,	MAP_SUBNO,		BITMAP_FILE, };	

bool TAseParser::LoadTexture(TMtrl*	pMaterial)
{
	int iObjectType = -1;
	int iNumTypes = sizeof(AseTextureTokens) / sizeof(AseTextureTokens[0]);		
	TTextexMap* pTexMap = 0;
	TCHAR szTexName[256] = _T("");
	GetData(szTexName, STRING_DATA);	
	
	while ((iObjectType = SearchTokenArray(iNumTypes, AseTextureTokens)) >= 0)
	{
		/*TCHAR szTexName[256] = _T("");
		GetData(_T("MAP_NAME"), &szTexName, STRING_DATA);
		TexMap.m_strName = szTexName;
		GetData(_T("MAP_SUBNO"), &TexMap.m_dwType, INT_DATA);
*/
		switch (iObjectType)
		{
			case MAP_SUBNO: // BITMAP은 typedef struct tagBITMAP 정의되어 있음. 
			{
				TTextexMap TexMap;
				GetData(&TexMap.m_dwType, INT_DATA);
				pMaterial->m_TexMaps.push_back(TexMap);
				pTexMap = &pMaterial->m_TexMaps[pMaterial->m_TexMaps.size() - 1];
			}break;
			case BITMAP_FILE: // BITMAP은 typedef struct tagBITMAP 정의되어 있음. 
			{
				GetData(&szTexName, STRING_DATA);
				pTexMap->m_strTextureName = szTexName;
				pTexMap->m_dwIndex = I_Texture.Add(
					g_pd3dDevice, szTexName, m_szDirName.c_str());				
			}
			break;
			default:
			{
				m_dwTokenIndex--;
				return true;
			}
		}
	}
	return false;
}

bool TAseParser::GetVertexFromIndexData(const TCHAR* strToken, DWORD dwSearchIndex, VOID* pData, INT DataType)
{
	GetData(strToken);
	TCHAR szBuffer[256] = _T("");
	_stprintf_s(szBuffer, _T("%d"), dwSearchIndex);
	GetData(szBuffer, pData, DataType);
	return true;
}
bool TAseParser::GetData(const TCHAR* pString, VOID* pData, INT DataType, bool bLoop)
{
	if (GetSearchString(pString, bLoop) != NULL)
	{
		if (DataType == NULL_DATA) return true;
		return GetData(pData, DataType);
	}
	return false;
}
bool TAseParser::GetData(VOID* pData, INT DataType)
{
	if (pData != NULL)
	{
		switch (DataType)
		{
		case INT_DATA:
			_stscanf(m_pwcTokenData.c_str(), _T("%s%d"), m_pString, (int*)pData);
			return true;
		case FLOAT_DATA:
			_stscanf(m_pwcTokenData.c_str(), _T("%s%f"), m_pString, (float*)pData);
			return true;
		case STRING_DATA:
		{
			T_STR szFind;
			T_STR::size_type idx, idxEnd;
			idx = m_pwcTokenData.find_first_of(_T("\""));
			szFind = m_pwcTokenData.substr(idx + 1);
			idxEnd = szFind.find_first_of(_T("\""));
			_tcscpy((TCHAR*)pData, szFind.substr(0, idxEnd).c_str());
		}
		return true;
		case VERTEX_DATA:
		{
			_stscanf(m_pwcTokenData.c_str(), _T("%s%d%f%f%f"), m_pString, &m_iData,
				&((TVector3*)pData)->x,
				&((TVector3*)pData)->z,
				&((TVector3*)pData)->y);
		}
		return true;
		case VECTOR_DATA:
		{
			_stscanf(m_pwcTokenData.c_str(), _T("%s%f%f%f"), m_pString,
				&((TVector3*)pData)->x,
				&((TVector3*)pData)->z,
				&((TVector3*)pData)->y);
		}
		return true;
		case NULL_DATA:
			return true;
		}
	}
	return false;
}
bool TAseParser::GetVertexListFromString(const TCHAR* strVertexList, DWORD dwNumVertex, vector<TVector3>& VertexList, const TCHAR* strToken)
{
	if (dwNumVertex > 0 && GetData(strVertexList))
	{
		VertexList.resize(dwNumVertex);
		for (DWORD dwVertexCount = 0; dwVertexCount < dwNumVertex; dwVertexCount++)
		{
			TVector3 vVertex;
			_stscanf(GetNextTokenString(), _T("%s%d%f%f%f"), m_pString, &m_iData,
				&vVertex.x,
				&vVertex.z,
				&vVertex.y);
			VertexList[dwVertexCount] = vVertex;
		}
	}
	return true;
}
bool TAseParser::GetFaceListFromString(const TCHAR* strFaceList, DWORD dwNumFace, TVertexList& VertexList, const TCHAR* strToken)
{
	if (dwNumFace >0 && GetData(strFaceList))
	{
		VertexList.pFaceList.resize(dwNumFace);

		for (DWORD dwFaceCount = 0; dwFaceCount < dwNumFace; dwFaceCount++)
		{
			TFaceList vFaceList;
			_stscanf(GetNextTokenString(), _T("%s%d %d%d%d"), m_pString, &m_iData,
				&vFaceList._0,
				&vFaceList._2,
				&vFaceList._1);
			VertexList.pFaceList[dwFaceCount] = vFaceList;
		}
	}
	return true;
}
bool TAseParser::GetPositionFaceListFromString(const TCHAR* strFaceList, DWORD dwNumFace, TVertexList& VertexList, const TCHAR* strToken)
{
	if (dwNumFace > 0 && GetData(strFaceList))
	{
		VertexList.pFaceList.resize(dwNumFace);

		for (DWORD dwFaceCount = 0; dwFaceCount < dwNumFace; dwFaceCount++)
		{
			TFaceList vFaceList;
			// *MESH_FACE
			_stscanf(GetNextTokenString(), _T("%s %s%s %d %s %d %s %d"),
				m_pString, m_pString, m_pString,
				&vFaceList._0, m_pString, &vFaceList._2, m_pString, &vFaceList._1);
			// *MESH_SMOOTHING
			GetNextTokenString();
			// *MESH_MTLID
			_stscanf(GetNextTokenString(), _T("%s %d"), m_pString, &vFaceList.dwMtrl);

			VertexList.pFaceList[dwFaceCount] = vFaceList;
		}
	}
	return true;
}
bool TAseParser::GetNormalListFromString(const TCHAR* strFaceList, DWORD dwNumFace, TAseMesh*	pGeomObj, const TCHAR* strToken)
{
	if (GetData(strFaceList))
	{
		pGeomObj->m_NorVertexList.pVertexList.resize(dwNumFace * 4);

		for (DWORD dwFaceCount = 0; dwFaceCount < dwNumFace; dwFaceCount++)
		{
			// *MESH_FACENORMAL
			TVector3 vVertex;
			_stscanf(GetNextTokenString(), _T("%s%d%f%f%f"), m_pString, &m_iData,
				&vVertex.x,
				&vVertex.z,
				&vVertex.y);
			pGeomObj->m_NorVertexList.pVertexList[dwFaceCount * 4 + 0] = vVertex;
			// *MESH_VERTEXNORMAL				
			_stscanf(GetNextTokenString(), _T("%s%d%f%f%f"), m_pString, &m_iData,
				&vVertex.x,
				&vVertex.z,
				&vVertex.y);
			pGeomObj->m_NorVertexList.pVertexList[dwFaceCount * 4 + 1] = vVertex;
			_stscanf(GetNextTokenString(), _T("%s%d%f%f%f"), m_pString, &m_iData,
				&vVertex.x,
				&vVertex.z,
				&vVertex.y);
			pGeomObj->m_NorVertexList.pVertexList[dwFaceCount * 4 + 3] = vVertex;
			_stscanf(GetNextTokenString(), _T("%s%d%f%f%f"), m_pString, &m_iData,
				&vVertex.x,
				&vVertex.z,
				&vVertex.y);
			pGeomObj->m_NorVertexList.pVertexList[dwFaceCount * 4 + 2] = vVertex;
		}
	}
	return true;
}
TAseParser::TAseParser()
{
}

TAseParser::~TAseParser()
{
}



bool TAseParser::GetTrackListFromString(vector<TAnimTrack>& vTrack,
	AseTrackType TrackType)
{
	for (;;)
	{
		TAnimTrack Track;

		if (TrackType == POS_SAMLE_TRACK)
		{
			_stscanf(GetNextTokenString(), _T("%s%d%f%f%f"), m_pString, &Track.iTick,
				&Track.vVector.x,
				&Track.vVector.z,
				&Track.vVector.y);
		}
		else if (TrackType == ROT_SAMLE_TRACK)
		{
			_stscanf(GetNextTokenString(), _T("%s%d%f%f%f%f"), m_pString, &Track.iTick,
				&Track.qRotate.x, &Track.qRotate.z, &Track.qRotate.y, &Track.qRotate.w);
		}
		else if (TrackType == SCL_SAMLE_TRACK)
		{
			_stscanf(GetNextTokenString(), _T("%s%d%f%f%f %f%f%f%f"), m_pString, &Track.iTick,
				&Track.vVector.x, &Track.vVector.z, &Track.vVector.y,
				&Track.qRotate.x, &Track.qRotate.z, &Track.qRotate.y, &Track.qRotate.w);
		}
		else if (TrackType == VIS_SAMLE_TRACK)
		{
			_stscanf(GetNextTokenString(), _T("%s%d%f"),
				m_pString, &Track.iTick,
				&Track.vVector.x);
		}
		vTrack.push_back(Track);

		if (_tcsstr(m_pwcTokenData.c_str(), _T("}")) != NULL)
		{
			break;
		}
	}
	return true;
}
bool TAseParser::Release()
{
	DeleteBuffer();
	for (DWORD dwObject = 0; dwObject < m_pAseMesh.size(); dwObject++)
	{
		m_pAseMesh[dwObject]->m_PosTrack.clear();
		m_pAseMesh[dwObject]->m_RotTrack.clear();
		m_pAseMesh[dwObject]->m_SclTrack.clear();
	}
	m_pAseMesh.clear();
	return true;
}
void TAseParser::LoadTM(TAseMesh* pObject)
{
	// NodeTM	
	D3DXMatrixIdentity(&pObject->m_matWorld);
	GetData(_T("TM_ROW0"), &pObject->m_matWorld.m[0], VECTOR_DATA);
	GetData(_T("TM_ROW1"), &pObject->m_matWorld.m[2], VECTOR_DATA);
	GetData(_T("TM_ROW2"), &pObject->m_matWorld.m[1], VECTOR_DATA);
	GetData(_T("TM_ROW3"), &pObject->m_matWorld.m[3], VECTOR_DATA);

	// 인버스 매트릭스 확인 코드.
	TVector3 v0, v1, v2, v3;
	v0 = pObject->m_matWorld.m[0];
	v1 = pObject->m_matWorld.m[1];
	v2 = pObject->m_matWorld.m[2];
	D3DXVec3Cross(&v3, &v1, &v2);
	if (D3DXVec3Dot(&v3, &v0) < 0.0f)
	{
		TMatrix matW;
		D3DXMatrixScaling(&matW, -1.0f, -1.0f, -1.0f);
		D3DXMatrixMultiply(&pObject->m_matWorld, &pObject->m_matWorld, &matW);
	}

	TVector3 vVector;
	float		fAngle;
	TQuaternion qRotation;
	GetData(_T("TM_POS"), &vVector, VECTOR_DATA);
	D3DXMatrixTranslation(&pObject->m_matWorldTrans, vVector.x, vVector.y, vVector.z);

	GetData(_T("TM_ROTAXIS"), &vVector, VECTOR_DATA);
	GetData(_T("TM_ROTANGLE"), &fAngle, FLOAT_DATA);
	// 임의의 축과 각을 쿼터니언으로 변환
	D3DXQuaternionIdentity(&qRotation);
	if (vVector != TVector3(0, 0, 0))
	{
		D3DXQuaternionRotationAxis(&qRotation, &vVector, fAngle);
	}
	D3DXMatrixRotationQuaternion(&pObject->m_matWorldRotate, &qRotation);

	//스케일축을 중심으로 회전하는 값이 스테일 값이므로 
	//스케일축의 회전만큼을 먼저 반대로 회전한 이후
	//스케일값을 적용시키고 그 다음에 
	//다시 스케일축만큼을 회전시켜 원상복귀 시킨다.
	TMatrix matScl;
	GetData(_T("TM_SCALE"), &vVector, VECTOR_DATA);
	D3DXMatrixScaling(&pObject->m_matWorldScale, vVector.x, vVector.y, vVector.z);
	TVector3 vAxis;
	GetData(_T("TM_SCALEAXIS"), &vAxis, VECTOR_DATA);
	GetData(_T("TM_SCALEAXISANG"), &fAngle, FLOAT_DATA);

	// 스케일축의 행렬과 그 역행렬을 구한다.
	TMatrix matRotation, matRotationInv;
	D3DXMatrixIdentity(&matRotation);
	if (vAxis != TVector3(0, 0, 0))
	{
		D3DXQuaternionRotationAxis(&qRotation, &vVector, fAngle);
	}
	D3DXMatrixInverse(&matRotationInv, NULL, &matRotation);
	//D3DXMatrixMultiply( &pObject->m_matWorldScale, &matRotationInv, &pObject->m_matWorldScale);
	//D3DXMatrixMultiply( &pObject->m_matWorldScale, &pObject->m_matWorldScale, &matRotation );
	pObject->m_matWorldScale = matRotationInv * pObject->m_matWorldScale  * matRotation;
}
void TAseParser::LoadVisAnimation(TAseMesh* pObject)
{
	if (GetSearchString(_T("CONTROL_FLOAT_BEZIER"), false))
	{
		GetTrackListFromString(pObject->m_VisTrack, VIS_SAMLE_TRACK);
	}
}
void TAseParser::LoadAnimation(TAseMesh* pObject)
{
	GetSearchString(_T("NODE_NAME"), false);
	if (GetSearchString(_T("CONTROL_POS_TRACK"), false) ||
		GetSearchString(_T("CONTROL_POS_TCB"), false) ||
		GetSearchString(_T("CONTROL_POS_BEZIER"), false))
	{
		GetTrackListFromString(pObject->m_PosTrack, POS_SAMLE_TRACK);
	}
	if (GetSearchString(_T("CONTROL_ROT_TRACK"), false) ||
		GetSearchString(_T("CONTROL_ROT_TCB"), false) ||
		GetSearchString(_T("CONTROL_ROT_BEZIER"), false))
	{
		GetTrackListFromString(pObject->m_RotTrack, ROT_SAMLE_TRACK);
	}
	if (GetSearchString(_T("CONTROL_SCALE_TRACK"), false) ||
		GetSearchString(_T("CONTROL_SCALE_TCB"), false) ||
		GetSearchString(_T("CONTROL_SCALE_BEZIER"), false))
	{
		GetTrackListFromString(pObject->m_SclTrack, SCL_SAMLE_TRACK);
	}

}
void TAseParser::LoadMesh(TAseMesh* pObject)
{
	CStopwatch stopwatch;
	// Position Data
	GetData(_T("MESH_NUMVERTEX"), &pObject->m_PosVertexList.dwNumVertex, INT_DATA);
	pObject->m_iNumFace = 0;
	if (pObject->m_PosVertexList.dwNumVertex > 0)
	{
		GetData(_T("MESH_NUMFACES"), &pObject->m_PosVertexList.dwNumFace, INT_DATA);
		pObject->m_iNumFace = pObject->m_PosVertexList.dwNumFace;

		GetVertexListFromString(_T("MESH_VERTEX_LIST"), pObject->m_PosVertexList.dwNumVertex, pObject->m_PosVertexList.pVertexList);
		GetPositionFaceListFromString(_T("MESH_FACE_LIST"), pObject->m_PosVertexList.dwNumFace, pObject->m_PosVertexList);
	}
	__int64 qwElapsedTimeA = stopwatch.Now();
	// Texture Coord data
	GetData(_T("MESH_NUMTVERTEX"), &pObject->m_TexVertexList.dwNumVertex, INT_DATA);
	if (pObject->m_TexVertexList.dwNumVertex > 0)
	{
		GetVertexListFromString(_T("MESH_TVERTLIST"), pObject->m_TexVertexList.dwNumVertex, pObject->m_TexVertexList.pVertexList);
		GetData(_T("MESH_NUMTVFACES"), &pObject->m_TexVertexList.dwNumFace, INT_DATA);
		GetFaceListFromString(_T("MESH_TFACELIST"), pObject->m_TexVertexList.dwNumFace, pObject->m_TexVertexList);
	}
	__int64 qwElapsedTimeB = stopwatch.Now() - qwElapsedTimeA;
	// Color Data
	GetData(_T("MESH_NUMCVERTEX"), &pObject->m_ColVertexList.dwNumVertex, INT_DATA, false);
	if (pObject->m_ColVertexList.dwNumVertex > 0)
	{
		GetVertexListFromString(_T("MESH_CVERTLIST"), pObject->m_ColVertexList.dwNumVertex, pObject->m_ColVertexList.pVertexList);
		GetData(_T("MESH_NUMCVFACES"), &pObject->m_ColVertexList.dwNumFace, INT_DATA);
		GetFaceListFromString(_T("MESH_CFACELIST"), pObject->m_ColVertexList.dwNumFace, pObject->m_ColVertexList);
	}
	__int64 qwElapsedTimeC = stopwatch.Now() - qwElapsedTimeA - qwElapsedTimeB;
	// Normal Data
	if (pObject->m_iNumFace > 0)
	{
		GetNormalListFromString(_T("MESH_NORMALS"), pObject->m_iNumFace, pObject);
	}
	__int64 qwElapsedTimeD = stopwatch.Now() - qwElapsedTimeA - qwElapsedTimeB - qwElapsedTimeC;

	TCHAR  strMessage[MAX_PATH] = { 0, };
	_stprintf(strMessage, _T("\nTIME=\t%I64d \t%I64d \t%I64d \t%I64d \t%I64d"),
		qwElapsedTimeA, qwElapsedTimeB, qwElapsedTimeC, qwElapsedTimeD,
		qwElapsedTimeA + qwElapsedTimeB + qwElapsedTimeC + qwElapsedTimeD);
	OutputDebugString(strMessage);
}
bool TAseParser::LoadGeomObject()
{
	INT iObjectType = -1;
	auto pAseObject = make_shared<TAseMesh>();
	m_pAseMesh.push_back(pAseObject);

	TAseMesh*	pObject = pAseObject.get();

	TCHAR szNodeName[256] = _T("");
	TCHAR szParentName[256] = _T("");

	GetData(_T("NODE_NAME"), &szNodeName, STRING_DATA);
	pObject->m_strNodeName = szNodeName;


	int iNumTypes = sizeof(AseGeomObjectTokens) / sizeof(AseGeomObjectTokens[0]);
	while ((iObjectType = SearchTokenArray(iNumTypes, AseGeomObjectTokens, m_dwMaxStringTable)) >= 0)
	{
		switch (iObjectType)
		{
		case NODE_PARENT:
		{
			GetData(&szParentName, STRING_DATA);
			pObject->m_strParentName = szParentName;
		}break;
		case MODE_TM:
		{
			LoadTM(pObject);
		}break;
		case MESH:
		{
			LoadMesh(pObject);
		}break;
		case TM_ANIMATION:
		{
			LoadAnimation(pObject);
		}break;
		case TM_VISIBILITY:
		{
			LoadVisAnimation(pObject);
		}break;
		case MATERIAL_REF://_T("MATERIAL_REF)"
		{
			GetData(&pObject->m_iMtrlRef, INT_DATA);
		}break;
		default:
		{
			m_dwTokenIndex--;
			return true;
		}break;
		}
	}
	return true;
}

bool TAseParser::LoadHelperObject()
{
	INT iObjectType = -1;
	auto pAseObject = make_shared<TAseMesh>();
	m_pAseMesh.push_back(pAseObject);
	TAseMesh*	pObject = pAseObject.get();

	TCHAR szNodeName[256] = _T("");
	TCHAR szParentName[256] = _T("");

	GetData(_T("NODE_NAME"), &szNodeName, STRING_DATA);
	pObject->m_strNodeName = szNodeName;


	int iNumTypes = sizeof(AseHelperObjectTokens) / sizeof(AseHelperObjectTokens[0]);
	while ((iObjectType = SearchTokenArray(iNumTypes, AseHelperObjectTokens, m_dwMaxStringTable)) >= 0)
	{
		switch (iObjectType)
		{
		case HELPER_NODE_PARENT:
		{
			GetData(&szParentName, STRING_DATA);
			pObject->m_strParentName = szParentName;
		}break;
		case HELPER_CLASS:
		{
			TCHAR szHelperClassName[256] = _T("");
			GetData(&szHelperClassName, STRING_DATA);
			if (_tcsicmp(szHelperClassName, _T("Bone")) == 0)
			{
				pObject->m_ClassType = CLASS_BONE;
			}
			else if (_tcsicmp(szHelperClassName, _T("Dummy")) == 0)
			{
				pObject->m_ClassType = CLASS_DUMMY;
			}
		}break;
		case HELPER_MODE_TM:
		{
			LoadTM(pObject);
		}break;
		case BOUNDINGBOX_MIN:
		{
			m_dwTokenIndex--;
			GetData(_T("BOUNDINGBOX_MIN"), &pObject->m_BBox.vMin, VECTOR_DATA);
			GetData(_T("BOUNDINGBOX_MAX"), &pObject->m_BBox.vMax, VECTOR_DATA);
		}break;
		case HELPER_TM_ANIMATION:
		{
			LoadAnimation(pObject);
		}break;
		//case WIREFRAME_COLOR:
		//	{
		//		pObject->m_iMtrlRef = -1;
		//	}break;
		default:
		{
			m_dwTokenIndex--;
			return true;
		}break;
		}
	}
	return true;
}