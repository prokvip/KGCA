#include "pch.h"
#include "TNode.h"
void    TNode::CreateIndexData(TMap* pMap, DWORD dwNumCols, DWORD dwNumRows)
{
	// 0  1   2   3   4
	// 5  6   7   8   9
	// 10 11  12  13  14
	// 15 16  17  18  19
	// 20  21 22  23  24
	DWORD dwTL = m_Corner[0];
	DWORD dwTR = m_Corner[1];
	DWORD dwBL = m_Corner[2];
	DWORD dwBR = m_Corner[3];
	DWORD dwNumRowCell = (dwBL - dwTL)/ dwNumCols;
	DWORD dwNumColCell = dwTR - dwTL;
	DWORD dwNumCells = dwNumRowCell * dwNumColCell;
	m_IndexList.resize(dwNumCells * 2 * 3);
	int iIndex = 0;

	m_tBox.vMin.x = pMap->m_VertexList[dwTL].p.x;
	m_tBox.vMin.y = 100000.0f;
	m_tBox.vMin.z = pMap->m_VertexList[dwBL].p.z;
	m_tBox.vMax.x = pMap->m_VertexList[dwTR].p.x;
	m_tBox.vMax.y = -100000.0f;
	m_tBox.vMax.z = pMap->m_VertexList[dwTL].p.z;

	for (int dwRow = 0; dwRow < dwNumRowCell; dwRow++)
	{
		for (int dwCol = 0; dwCol < dwNumColCell; dwCol++)
		{
			m_IndexList[iIndex+0] = dwTL+dwCol+(dwRow* dwNumCols);
			m_IndexList[iIndex+1] = m_IndexList[iIndex + 0] +1;
			m_IndexList[iIndex+2] = dwTL + dwCol +((dwRow+1)*dwNumCols);

			m_IndexList[iIndex+3] = m_IndexList[iIndex + 2];
			m_IndexList[iIndex+4] = m_IndexList[iIndex + 1];
			m_IndexList[iIndex+5] = m_IndexList[iIndex + 2]+1;

			for (DWORD dwVertex = 0; dwVertex < 6; dwVertex++)
			{
				if (m_tBox.vMin.y > pMap->m_VertexList[m_IndexList[iIndex + dwVertex]].p.y)
				{
					m_tBox.vMin.y = pMap->m_VertexList[m_IndexList[iIndex + dwVertex]].p.y;
				}
				if (m_tBox.vMax.y < pMap->m_VertexList[m_IndexList[iIndex + dwVertex]].p.y)
				{
					m_tBox.vMax.y = pMap->m_VertexList[m_IndexList[iIndex + dwVertex]].p.y;
				}
			}
			iIndex += 6;
		}
	}	
	m_dwFace = m_IndexList.size() / 3;
	m_tBox.vCenter = (m_tBox.vMax + m_tBox.vMin)*0.5f;
	m_tBox.vAxis[0] = { 1,0,0 };
	m_tBox.vAxis[1] = { 0,1,0 };
	m_tBox.vAxis[2] = { 0,0,1 };
	m_tBox.fExtent[0] = m_tBox.vMax.x - m_tBox.vCenter.x;
	m_tBox.fExtent[1] = m_tBox.vMax.y - m_tBox.vCenter.y;
	m_tBox.fExtent[2] = m_tBox.vMax.z - m_tBox.vCenter.z;
}
HRESULT TNode::CreateIndexBuffer(	TMap* pMap,
									 DWORD dwNumRows, 
									 DWORD dNumCols)
{
	HRESULT hr;
	CreateIndexData(pMap, dwNumRows, dNumCols);
	D3D11_BUFFER_DESC       bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = sizeof(DWORD) * m_IndexList.size(); // 바이트 용량
	// GPU 메모리에 할당
	bd.Usage = D3D11_USAGE_DEFAULT; // 버퍼의 할당 장소 내지는 버퍼용도
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA  sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = &m_IndexList.at(0);
	hr = pMap->m_pd3dDevice->CreateBuffer(
		&bd, // 버퍼 할당
		&sd, // 초기 할당된 버퍼를 체우는 CPU메모리 주소
		&m_pIndexBuffer);
	return hr;
}
//  0, 1
//  2, 3
TNode::TNode(TNode* pParent, TMap* pMap,
	DWORD dwLT, DWORD dwRT, DWORD dwRB, DWORD dwLB,
	DWORD dwNumCols, DWORD dwNumRows)
{		
	if (pParent != nullptr)
	{
		m_iDepth = pParent->m_iDepth + 1;
	}
	m_Corner[0] = dwLT;
	m_Corner[1] = dwRT;
	m_Corner[2] = dwRB;
	m_Corner[3] = dwLB;
	m_bLeaf = false;	
	m_pChild.resize(4);
	// boundingBox( aabb, obb )
	// 0  1   2   3   4
	// 5  6   7   8   9
	// 10 11  12  13  14
	// 15 16  17  18  19
	// 20  21 22  23  24

	CreateIndexBuffer(pMap, dwNumCols, dwNumRows);
	
	
}
void   TNode::CreateChildNode(TNode* pParent, TMap* pMap,
	DWORD dwNumCols, DWORD dwNumRows )
{
	// 0  1   2   3   4
	// 5  6   7   8   9
	// 10 11  12  13  14
	// 15 16  17  18  19
	// 20  21 22  23  24
	TRect rt = pParent->m_rt;
	DWORD dwLT = pParent->m_Corner[0]; // 0
	DWORD dwRT = pParent->m_Corner[1]; // 512
	DWORD dwLB = pParent->m_Corner[2];
	DWORD dwRB = pParent->m_Corner[3];
	DWORD dwTopCenter = (dwLT + dwRT) / 2;
	DWORD dwRightCenter = (dwRB + dwRT) / 2;
	DWORD dwBottomCenter = (dwLB + dwRB) / 2;
	DWORD dwLeftCenter = (dwLB + dwLT) / 2;
	DWORD dwCenter = (dwRT + dwLT+ dwRB+dwLB ) / 4;
	m_pChild[0] = new TNode(pParent, pMap, dwLT, dwTopCenter, dwLeftCenter, dwCenter, dwNumCols, dwNumRows);
	m_pChild[1] = new TNode(pParent, pMap, dwTopCenter, dwRT, dwCenter, dwRightCenter, dwNumCols, dwNumRows);
	m_pChild[2] = new TNode(pParent, pMap, dwLeftCenter, dwCenter, dwLB, dwBottomCenter, dwNumCols, dwNumRows);
	m_pChild[3] = new TNode(pParent, pMap, dwCenter, dwRightCenter, dwBottomCenter, dwRB, dwNumCols, dwNumRows);
}
TNode::~TNode()
{
	if(m_pIndexBuffer)m_pIndexBuffer->Release();
	delete m_pChild[0];
	delete m_pChild[1];
	delete m_pChild[2];
	delete m_pChild[3];
	m_pChild[0] = nullptr;
	m_pChild[1] = nullptr;
	m_pChild[2] = nullptr;
	m_pChild[3] = nullptr;
}