#include "pch.h"
#include "TQuadtree.h"
TVector2 TQuadtree::GetHeightFormNode(TNode* pNode)
{
	DWORD dwTL = pNode->m_Corner[0];
	DWORD dwTR = pNode->m_Corner[1];
	DWORD dwBL = pNode->m_Corner[2];
	DWORD dwBR = pNode->m_Corner[3];

	DWORD dwStartCol = dwTL % m_pMap->m_iNumCols;
	DWORD dwEndCol   = dwTR % m_pMap->m_iNumCols;
	DWORD dwStartRow = dwTL / m_pMap->m_iNumCols;
	DWORD dwEndRow   = dwBL / m_pMap->m_iNumCols;

	TVector2 vHeight;
	vHeight.x = -99999999.0f;
	vHeight.y = 99999999.0f;

	for (int dwRow = dwStartRow; dwRow < dwEndRow; dwRow++)
	{
		for (int dwCol = dwStartCol; dwCol < dwEndCol; dwCol++)
		{
			DWORD dwCurrent = dwRow * m_pMap->m_iNumCellCols + dwCol;
			if (m_pMap->m_VertexList[dwCurrent].p.y > vHeight.x)
			{
				vHeight.x = m_pMap->m_VertexList[dwCurrent].p.y;
			}
			if (m_pMap->m_VertexList[dwCurrent].p.y < vHeight.y)
			{
				vHeight.y = m_pMap->m_VertexList[dwCurrent].p.y;
			}
		}
	}
	return vHeight;
}
void TQuadtree::ComputeBoundingBox(TNode* pNode)
{
	DWORD dwV0 = pNode->m_Corner[0];
	DWORD dwV1 = pNode->m_Corner[3];
	TVector2 vHeight = GetHeightFormNode(pNode);
	pNode->m_tBox.vMax.x = m_pMap->m_VertexList[dwV1].p.x;
	pNode->m_tBox.vMax.z = m_pMap->m_VertexList[dwV0].p.z;
	pNode->m_tBox.vMin.x = m_pMap->m_VertexList[dwV0].p.x;
	pNode->m_tBox.vMin.z = m_pMap->m_VertexList[dwV1].p.z;
	pNode->m_tBox.vMax.y = vHeight.x;
	pNode->m_tBox.vMin.y = vHeight.y;
	pNode->m_tBox.vAxis[0] = { 1,0,0 };
	pNode->m_tBox.vAxis[1] = { 0,1,0 };
	pNode->m_tBox.vAxis[2] = { 0,0,1 };
	pNode->m_tBox.vCenter = (pNode->m_tBox.vMax + pNode->m_tBox.vMin) * 0.5f;
	pNode->m_tBox.fExtent[0] = pNode->m_tBox.vMax.x - pNode->m_tBox.vCenter.x;
	pNode->m_tBox.fExtent[1] = pNode->m_tBox.vMax.y - pNode->m_tBox.vCenter.y;
	pNode->m_tBox.fExtent[2] = pNode->m_tBox.vMax.z - pNode->m_tBox.vCenter.z;
}
TNode* TQuadtree::CreateNode(TNode* pParent,
		DWORD dwTL, DWORD dwTR,
		DWORD dwBL, DWORD dwBR )
{
	TNode* pNode = new TNode;
	pNode->m_Corner.push_back(dwTL);
	pNode->m_Corner.push_back(dwTR);
	pNode->m_Corner.push_back(dwBL);
	pNode->m_Corner.push_back(dwBR);

	ComputeBoundingBox(pNode);
	return pNode;
}
bool   TQuadtree::Build(TMap* map)
{
	m_pMap = map;
	DWORD dwWidth  = m_pMap->m_iNumCols;
	DWORD dwHeight = m_pMap->m_iNumRows;
	m_pRootNode = CreateNode(nullptr, 0, 
		dwWidth-1, dwWidth*(dwHeight-1), 
		dwWidth*dwHeight-1);
	return true;
}