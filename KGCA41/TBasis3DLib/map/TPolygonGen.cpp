#include "TPolygonGen.h"


int FLATMAP55[] = {
	16,16,16,16,16,
	0, 16, 0, 0, 0,
	0, 16, 0,16, 0,
	16,16,16,16,16,
	0, 16,16,16,0,
};
int NORTHMAP55[] = {
	8,8,8,8,8,
	0,0,0,0,0,
	0,0,0,8,0,
	8,0,8,0,8,
	0,0,0,0,0
};
int WESTMAP55[] = {
	1,0,0,0,0,
	0,1,0,0,0,
	0,1,0,1,0,
	1,0,0,0,0,
	0,1,0,0,0
};
int EASTMAP55[] = {
	0,0,0,0,2,
	0,2,0,0,0,
	0,2,0,2,0,
	0,0,0,0,2,
	0,0,0,2,0
};
int SOUTHMAP55[] = {
	4,0,4,4,4,
	0,0,0,0,0,
	0,0,0,0,0,
	4,0,0,0,4,
	0,4,4,4,0
};

int CEILLING55[] = {
	32,32,32,32,32,
	0,32,0,0,0,
	0,32,0,32,0,
	32,32,32,32,32,
	0,32,32,32,0
};
// Number of actions
#define NUMBER_OF_MAP55    (sizeof(FLATMAP55)/sizeof(int))

enum {
	FlatWall = 16,
	WestWall = 1,
	EastWall = 2,
	SouthWall = 4,
	NorthWall = 8,
	CeillingWall = 32,
	NullWall = 0,
};



bool	TPolygonGen::LoadTexture()
{
	T_STR strTextureName[3];
	strTextureName[0] = Res(L"ap/Rock.jpg");
	strTextureName[1] = Res(L"map/DetailMap_Red.dds");
	strTextureName[2] = Res(L"map/seafloor.dds");

	for (int iTex = 0; iTex < 3; iTex++)
	{
		PolyTexture PolyTex;
		PolyTex.szName = strTextureName[iTex];
		PolyTex.iTexIndex = I_Texture.Add(g_pd3dDevice, PolyTex.szName.c_str());
		m_TextureArray.push_back(PolyTex);
	}
	return true;
}
// Type=0 => 두개의 플랜지형 생성
// Type=1 => 직각 실외지형 생성
TPolygon* TPolygonGen::CreateMap(DWORD dwType)
{
	m_iNumPolygon = 0;
	m_pPolyList = 0;

	if (!LoadTexture()) return NULL;

	//////////////////////////////////////////////////////////////////
	if (dwType == 0) // 방법 1
	{
		SAFE_NEW(m_pPolyList, TPolygon)
			ZeroMemory(m_pPolyList, sizeof(TPolygon));

		m_pPolyList->iNumIndices = 6;
		m_pPolyList->iNumVertices = 4;
		m_pPolyList->iTexIndex = m_TextureArray[0].iTexIndex;

		SAFE_NEW_ARRAY(m_pPolyList->pVertexList, PNCT_VERTEX, m_pPolyList->iNumVertices)
			ZeroMemory(m_pPolyList->pVertexList, sizeof(PNCT_VERTEX)*m_pPolyList->iNumVertices);
		m_pPolyList->pVertexList[0].p = TVector3(0.0f, 10.0f, 10.0f);
		m_pPolyList->pVertexList[1].p = TVector3(0.0f, 10.0f, -10.0f);
		m_pPolyList->pVertexList[2].p = TVector3(0.0f, -10.0f, -10.0f);
		m_pPolyList->pVertexList[3].p = TVector3(0.0f, -10.0f, 10.0f);
		m_pPolyList->pVertexList[0].n = TVector3(-1.0f, 0.0f, 0.0f);
		m_pPolyList->pVertexList[1].n = TVector3(-1.0f, 0.0f, 0.0f);
		m_pPolyList->pVertexList[2].n = TVector3(-1.0f, 0.0f, 0.0f);
		m_pPolyList->pVertexList[3].n = TVector3(-1.0f, 0.0f, 0.0f);
		m_pPolyList->pVertexList[0].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
		m_pPolyList->pVertexList[1].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
		m_pPolyList->pVertexList[2].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
		m_pPolyList->pVertexList[3].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
		m_pPolyList->pVertexList[0].t = TVector2(0.0f, 0.0f);
		m_pPolyList->pVertexList[1].t = TVector2(1.0f, 0.0f);
		m_pPolyList->pVertexList[2].t = TVector2(1.0f, 1.0f);
		m_pPolyList->pVertexList[3].t = TVector2(0.0f, 1.0f);

		SAFE_NEW_ARRAY(m_pPolyList->pIndices, DWORD, m_pPolyList->iNumIndices);
		ZeroMemory(m_pPolyList->pIndices, sizeof(DWORD)*m_pPolyList->iNumIndices);
		m_pPolyList->pIndices[0] = 0;
		m_pPolyList->pIndices[1] = 1;
		m_pPolyList->pIndices[2] = 2;
		m_pPolyList->pIndices[3] = 0;
		m_pPolyList->pIndices[4] = 2;
		m_pPolyList->pIndices[5] = 3;
		m_pPolyList->vNormal = TVector3(-1.0f, 0.0f, 0.0f);

		TPolygon* pPolygon = NULL;
		SAFE_NEW(pPolygon, TPolygon)
			ZeroMemory(pPolygon, sizeof(TPolygon));
		pPolygon->iNumIndices = 6;
		pPolygon->iNumVertices = 4;
		pPolygon->vNormal = TVector3(0.0f, 1.0f, 0.0f);

		pPolygon->iTexIndex = m_TextureArray[1].iTexIndex;
		SAFE_NEW_ARRAY(pPolygon->pVertexList, PNCT_VERTEX, pPolygon->iNumVertices)
			ZeroMemory(pPolygon->pVertexList, sizeof(PNCT_VERTEX)*pPolygon->iNumVertices);

		pPolygon->vNormal = TVector3( 0.0f, 0.0f, -1.0f );
		pPolygon->pVertexList[0].p = TVector3( -10.0f, 10.0f, 0.0f );
		pPolygon->pVertexList[1].p = TVector3( 10.0f, 10.0f,  0.0f );
		pPolygon->pVertexList[2].p = TVector3( 10.0f, -10.0f, 0.0f );
		pPolygon->pVertexList[3].p = TVector3( -10.0f, -10.0f, 0.0f );

	/*	pPolygon->vNormal = TVector3(0.0f, 1.0f, 0.0f);
		pPolygon->pVertexList[0].p = TVector3(0.0f, 0.0f, -10.0f);
		pPolygon->pVertexList[1].p = TVector3(-10.0f, 0.0f, 10.0f);
		pPolygon->pVertexList[2].p = TVector3(-5.0f, 0.0f, 10.0f);
		pPolygon->pVertexList[3].p = TVector3(10.0f, 0.0f, 10.0f);*/

		pPolygon->pVertexList[0].n = TVector3(0.0f, 1.0f, 0.0f);
		pPolygon->pVertexList[1].n = TVector3(0.0f, 1.0f, 0.0f);
		pPolygon->pVertexList[2].n = TVector3(0.0f, 1.0f, 0.0f);
		pPolygon->pVertexList[3].n = TVector3(0.0f, 1.0f, 0.0f);
		pPolygon->pVertexList[0].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
		pPolygon->pVertexList[1].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
		pPolygon->pVertexList[2].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
		pPolygon->pVertexList[3].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
		pPolygon->pVertexList[0].t = TVector2(0.0f, 0.0f);
		pPolygon->pVertexList[1].t = TVector2(1.0f, 0.0f);
		pPolygon->pVertexList[2].t = TVector2(1.0f, 1.0f);
		pPolygon->pVertexList[3].t = TVector2(0.0f, 1.0f);

		SAFE_NEW_ARRAY(pPolygon->pIndices, DWORD, pPolygon->iNumIndices);
		ZeroMemory(pPolygon->pIndices, sizeof(DWORD)*pPolygon->iNumIndices);
		pPolygon->pIndices[0] = 0;
		pPolygon->pIndices[1] = 1;
		pPolygon->pIndices[2] = 2;
		pPolygon->pIndices[3] = 0;
		pPolygon->pIndices[4] = 2;
		pPolygon->pIndices[5] = 3;

		pPolygon->pNext = m_pPolyList;
		m_pPolyList = pPolygon;
	}
	//////////////////////////////////////////////////////////////////
	if (dwType == 1)
	{		// 방법 2
		if (!CreateBitMap(FLATMAP55, 5, 5, 100.0f))
		{
			return NULL;
		}
		if (!CreateBitMap(NORTHMAP55, 5, 5, 100.0f))
		{
			return NULL;
		}
		if (!CreateBitMap(WESTMAP55, 5, 5, 100.0f))
		{
			return NULL;
		}
		if (!CreateBitMap(EASTMAP55, 5, 5, 100.0f))
		{
			return NULL;
		}
		if (!CreateBitMap(SOUTHMAP55, 5, 5, 100.0f))

		{
			return NULL;
		}
		if (!CreateBitMap(CEILLING55, 5, 5, 100.0f))

		{
			return NULL;
		}
	}
	return m_pPolyList;
}
int TPolygonGen::MergePolygon(int *pMap, int iStartX, int iStartZ, int iMaxX, int iMazZ)
{
	int iReturnX = 1;
	for (int x = iStartX + 1; x < iMaxX; ++x)
	{
		int offset = (iStartZ * iMaxX) + x;
		if (pMap[offset] != NullWall)
		{
			iReturnX++;
			continue;
		}
		else
		{
			break;
		}
	}
	return iReturnX;
}
//  FLATMAP55
//  [0.0f 10.0f 50.0f][10.0f 10.0f 50.0f]   [10.0f 10.0f 50.0f][20.0f 10.0f 50.0f]    
//  [0.0f 0.0f  50.0f][10.0f 0.0f  50.0f]	[10.0f 0.0f  5.0f ][20.0f 0.0f  50.0f]
bool	TPolygonGen::CreateBitMap(int *pMap, int XCnt, int YCnt, float fUnit)
{
	WORD		v0, v1, v2;
	float		MaxZ = YCnt * fUnit;
	float		MaxX = XCnt * fUnit;

	float		MinZ = 0.0f;
	float		MinX = 0.0f;
	int			iNumVertex = 4;

	m_fHeight = fUnit;

	for (int z = 0; z < YCnt; ++z)
	{
		for (int x = 0; x < XCnt; ++x)
		{
			int offset = (z * YCnt) + x;
			if (pMap[offset] != NullWall)
			{
				m_iNumPolygon++;
				TPolygon*	pPolygon = NULL;
				SAFE_NEW(pPolygon, TPolygon)
					ZeroMemory(pPolygon, sizeof(TPolygon));

				SAFE_NEW_ARRAY(pPolygon->pVertexList, PNCT_VERTEX, 4)
					ZeroMemory(pPolygon->pVertexList, sizeof(PNCT_VERTEX) * 4);
				// Verties
				switch (pMap[offset])
				{
				case FlatWall:
				{
					pPolygon->iTexIndex = m_TextureArray[0].iTexIndex;
					pPolygon->vNormal = TVector3(0.0f, 1.0f, 0.0f);
					int iStartX = MergePolygon(pMap, x, z, XCnt, YCnt);
					SetBitVertex(&pPolygon->pVertexList[0], x*fUnit, 0.0f, MaxZ - (z*fUnit), pPolygon->vNormal, 0.0f, 0.0f);
					SetBitVertex(&pPolygon->pVertexList[1], x*fUnit + (fUnit*iStartX), 0.0f, MaxZ - (z*fUnit), pPolygon->vNormal, 1.0f, 0.0f);
					SetBitVertex(&pPolygon->pVertexList[2], x*fUnit + (fUnit*iStartX), 0.0f, MaxZ - (z*fUnit + fUnit), pPolygon->vNormal, 1.0f, 1.0f);
					SetBitVertex(&pPolygon->pVertexList[3], x*fUnit, 0.0f, MaxZ - (z*fUnit + fUnit), pPolygon->vNormal, 0.0f, 1.0f);
					x += iStartX;
				}break;
				case NorthWall:
				{
					pPolygon->iTexIndex = m_TextureArray[1].iTexIndex;
					pPolygon->vNormal = TVector3(0.0f, 0.0f, -1.0f);
					int iStartX = MergePolygon(pMap, x, z, XCnt, YCnt);
					SetBitVertex(&pPolygon->pVertexList[0], x*fUnit, m_fHeight, MaxZ - (z*fUnit), pPolygon->vNormal, 0.0f, 0.0f);
					SetBitVertex(&pPolygon->pVertexList[1], x*fUnit + (fUnit*iStartX), m_fHeight, MaxZ - (z*fUnit), pPolygon->vNormal, 1.0f, 0.0f);
					SetBitVertex(&pPolygon->pVertexList[2], x*fUnit + (fUnit*iStartX), 0.0f, MaxZ - (z*fUnit), pPolygon->vNormal, 1.0f, 1.0f);
					SetBitVertex(&pPolygon->pVertexList[3], x*fUnit, 0.0f, MaxZ - (z*fUnit), pPolygon->vNormal, 0.0f, 1.0f);
					x += iStartX;
				}break;
				case WestWall: // 왼쪽
				{
					pPolygon->iTexIndex = m_TextureArray[1].iTexIndex;
					pPolygon->vNormal = TVector3(1.0f, 0.0f, 0.0f);
					int iStartX = MergePolygon(pMap, x, z, XCnt, YCnt);
					SetBitVertex(&pPolygon->pVertexList[0], MinX + (x*fUnit), m_fHeight, MaxZ - (z*fUnit + fUnit), pPolygon->vNormal, 0.0f, 0.0f);
					SetBitVertex(&pPolygon->pVertexList[1], MinX + (x*fUnit), m_fHeight, MaxZ - (z*fUnit), pPolygon->vNormal, 1.0f, 0.0f);
					SetBitVertex(&pPolygon->pVertexList[2], MinX + (x*fUnit), 0.0f, MaxZ - (z*fUnit), pPolygon->vNormal, 1.0f, 1.0f);
					SetBitVertex(&pPolygon->pVertexList[3], MinX + (x*fUnit), 0.0f, MaxZ - (z*fUnit + fUnit), pPolygon->vNormal, 0.0f, 1.0f);
					x += iStartX;
				}break;
				case EastWall: // 오른쪽
				{
					pPolygon->iTexIndex = m_TextureArray[1].iTexIndex;
					pPolygon->vNormal = TVector3(-1.0f, 0.0f, 0.0f);
					int iStartX = MergePolygon(pMap, x, z, XCnt, YCnt);
					SetBitVertex(&pPolygon->pVertexList[0], MinX + ((x + 1)*fUnit), m_fHeight, MaxZ - (z*fUnit), pPolygon->vNormal, 0.0f, 0.0f);
					SetBitVertex(&pPolygon->pVertexList[1], MinX + ((x + 1)*fUnit), m_fHeight, MaxZ - (z*fUnit + fUnit), pPolygon->vNormal, 1.0f, 0.0f);
					SetBitVertex(&pPolygon->pVertexList[2], MinX + ((x + 1)*fUnit), 0.0f, MaxZ - (z*fUnit + fUnit), pPolygon->vNormal, 1.0f, 1.0f);
					SetBitVertex(&pPolygon->pVertexList[3], MinX + ((x + 1)*fUnit), 0.0f, MaxZ - (z*fUnit), pPolygon->vNormal, 0.0f, 1.0f);
					x += iStartX;
				}break;
				case SouthWall:
				{
					pPolygon->iTexIndex = m_TextureArray[1].iTexIndex;
					pPolygon->vNormal = TVector3(0.0f, 0.0f, 1.0f);
					int iStartX = MergePolygon(pMap, x, z, XCnt, YCnt);
					SetBitVertex(&pPolygon->pVertexList[0], x*fUnit + (fUnit*iStartX), m_fHeight, MaxZ - ((z + 1)*fUnit), pPolygon->vNormal, 0.0f, 0.0f);
					SetBitVertex(&pPolygon->pVertexList[1], x*fUnit, m_fHeight, MaxZ - ((z + 1)*fUnit), pPolygon->vNormal, 1.0f, 0.0f);
					SetBitVertex(&pPolygon->pVertexList[2], x*fUnit, 0.0f, MaxZ - ((z + 1)*fUnit), pPolygon->vNormal, 1.0f, 1.0f);
					SetBitVertex(&pPolygon->pVertexList[3], x*fUnit + (fUnit*iStartX), 0.0f, MaxZ - ((z + 1)*fUnit), pPolygon->vNormal, 0.0f, 1.0f);
					x += iStartX;
				}break;

				case CeillingWall:
				{
					pPolygon->iTexIndex = m_TextureArray[2].iTexIndex;
					pPolygon->vNormal = TVector3(0.0f, -1.0f, 0.0f);
					int iStartX = MergePolygon(pMap, x, z, XCnt, YCnt);
					SetBitVertex(&pPolygon->pVertexList[1], x*fUnit, m_fHeight, MaxZ - (z*fUnit), pPolygon->vNormal, 0.0f, 0.0f);
					SetBitVertex(&pPolygon->pVertexList[0], x*fUnit + (fUnit*iStartX), m_fHeight, MaxZ - (z*fUnit), pPolygon->vNormal, 1.0f, 0.0f);
					SetBitVertex(&pPolygon->pVertexList[3], x*fUnit + (fUnit*iStartX), m_fHeight, MaxZ - (z*fUnit + fUnit), pPolygon->vNormal, 1.0f, 1.0f);
					SetBitVertex(&pPolygon->pVertexList[2], x*fUnit, m_fHeight, MaxZ - (z*fUnit + fUnit), pPolygon->vNormal, 0.0f, 1.0f);
					x += iStartX;
				}break;
				}

				pPolygon->iNumVertices = iNumVertex;
				pPolygon->iNumIndices = (iNumVertex - 2) * 3;
				SAFE_NEW_ARRAY(pPolygon->pIndices, DWORD, pPolygon->iNumIndices);

				// Calculate indices
				for (DWORD i = 0; i < pPolygon->iNumIndices / 3; ++i)
				{
					if (i == 0)
					{
						v0 = 0, v1 = 1, v2 = 2;
					}
					else
					{
						v1 = v2, ++v2;
					}

					pPolygon->pIndices[i * 3] = v0;
					pPolygon->pIndices[(i * 3) + 1] = v1;
					pPolygon->pIndices[(i * 3) + 2] = v2;
				}

				// Generate polygon normal
				TVector3 *vec0 = (TVector3*)&pPolygon->pVertexList[pPolygon->pIndices[0]];
				TVector3 *vec1 = (TVector3*)&pPolygon->pVertexList[pPolygon->pIndices[1]];
				TVector3 *vec2 = (TVector3*)&pPolygon->pVertexList[pPolygon->pIndices[2]];
				//
				TVector3 edge1 = (*vec1) - (*vec0);
				TVector3 edge2 = (*vec2) - (*vec0);
				D3DXVec3Cross(&pPolygon->vNormal, &edge1, &edge2);
				D3DXVec3Normalize(&pPolygon->vNormal, &pPolygon->vNormal);

				//UnionPolygon(pPolygon);
				pPolygon->pNext = m_pPolyList;
				m_pPolyList = pPolygon;
				m_pPolyArray.push_back(pPolygon);
			}
		}
	}

	return true;
}


void TPolygonGen::UnionPolygon(TPolygon* pPolygon)
{
	if (m_pPolyList == pPolygon) return;
	for (TPolygon* pPoly = m_pPolyList; pPoly; pPoly = pPoly->pNext)
	{
		if (m_pPolyList == pPolygon) continue;
		float fDot = D3DXVec3Dot(&m_pPolyList->vNormal, &pPolygon->vNormal);
		float fD = -(D3DXVec3Dot(&m_pPolyList->vNormal, &pPoly->pVertexList[0].p));
		float fDistance = D3DXVec3Dot(&m_pPolyList->vNormal, &pPolygon->pVertexList[0].p) + fD;
		if (fabs(fDot - 1.0f) < 0.001f)
		{
			if (fabs(fDistance - 0.0f) < 0.001f)
			{
				if (pPoly->iTexIndex == pPolygon->iTexIndex)
				{
					UnionAddPolygon(pPolygon, m_pPolyList);
					return;
				}
			}
		}
	}

	pPolygon->pNext = m_pPolyList;
	m_pPolyList = pPolygon;
	m_pPolyArray.push_back(pPolygon);
}
void TPolygonGen::UnionAddPolygon(TPolygon* pSrcPoly, TPolygon* pDesc)
{
	vector<DWORD>	IndexList;
	VertexList VListData;

	IndexList.reserve(pSrcPoly->iNumIndices + pDesc->iNumIndices);
	VListData.reserve(pSrcPoly->iNumIndices + pDesc->iNumIndices);

	for (int iVer = 0; iVer < pDesc->iNumVertices; iVer++)
	{
		VListData.push_back(pDesc->pVertexList[iVer]);
	}
	for (int iIndex = 0; iIndex < pDesc->iNumIndices; iIndex++)
	{
		IndexList.push_back(pDesc->pIndices[iIndex]);
	}

	int iPushCnt = VListData.size();
	for (DWORD iver = 0; iver < pSrcPoly->iNumIndices; iver++)
	{
		int iReturn = -1;
		if (!VListData.empty())
		{
			iReturn = IsEqulVertexList(VListData,
				&pSrcPoly->pVertexList[pSrcPoly->pIndices[iver]]);
		}
		// 공유정점이 있으면
		if (iReturn >= 0)
		{
			IndexList.push_back(iReturn);
		}
		else // 공유정점이 없으면
		{
			VListData.push_back(pSrcPoly->pVertexList[pSrcPoly->pIndices[iver]]);
			IndexList.push_back(iPushCnt);
			iPushCnt++;
		}
	}

	// 인덱스 리스트
	SAFE_DELETE_ARRAY(pDesc->pIndices);
	SAFE_NEW_ARRAY(pDesc->pIndices, DWORD, IndexList.size());
	for (int iList = 0; iList < IndexList.size(); iList++)
	{
		pDesc->pIndices[iList] = IndexList[iList];
	}
	// 정점 리스트
	SAFE_DELETE_ARRAY(pDesc->pVertexList);
	SAFE_NEW_ARRAY(pDesc->pVertexList, PNCT_VERTEX, VListData.size());
	for (int iList = 0; iList < VListData.size(); iList++)
	{
		pDesc->pVertexList[iList] = VListData[iList];
	}

	pDesc->iNumIndices = IndexList.size();
	pDesc->iNumVertices = VListData.size();

	VListData.clear();
	IndexList.clear();

	SAFE_DEL(pSrcPoly);
}
int TPolygonGen::IsEqulVertexList(VertexList& CollectMap, PNCT_VERTEX* pTsVertex)
{
	int iVerIndex = -1;
	int iCurVer = 0;
	if (CollectMap.empty()) return -1;
	for (VertexListItor itor = CollectMap.begin(); itor != CollectMap.end(); itor++, iCurVer++)
	{
		PNCT_VERTEX *pCollect = &(PNCT_VERTEX)(*itor);
		if (pCollect)
		{
			if (pCollect->p == pTsVertex->p)
			{
				if (pCollect->n == pTsVertex->n &&
					pCollect->t == pTsVertex->t &&
					pCollect->c == pTsVertex->c)
				{
					iVerIndex = iCurVer;
					return iVerIndex;
				}
			}
		}
	}
	return iVerIndex;
}

void TPolygonGen::SetBitVertex(PNCT_VERTEX* pVertex, float x, float y, float z,
	TVector3 vNormal,
	float tu, float tv,
	float r, float g, float b, float a)
{
	pVertex->p.x = x;
	pVertex->p.y = y;
	pVertex->p.z = z;
	pVertex->n = vNormal;
	pVertex->c = TVector4(r, g, b, a);
	pVertex->t.x = tu;
	pVertex->t.y = tv;
}

TPolygonGen::TPolygonGen(void)
{
	m_pPolyList = NULL;
	m_fHeight = 15.0f;
}

TPolygonGen::~TPolygonGen(void)
{
}
