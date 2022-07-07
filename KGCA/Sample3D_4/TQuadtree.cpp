#include "TQuadtree.h"
int TQuadtree::g_iCount = 0;

// 0(0,0)   1(1,0)   4(2,0)   5(3,0)
// 2(0,1)   3(1,1)   6(2,1)   7(3,1)
// 8        9        12       13
// 10(0,3)  11       14       14(3,3)
TNode*   TQuadtree::CheckBoxtoPoint(T::TVector3 p)
{
	for (auto node : g_pLeafNodes)
	{
		if (node->m_Box.vMin.x <= p.x && node->m_Box.vMax.x >= p.x &&
			node->m_Box.vMin.z <= p.z && node->m_Box.vMax.z >= p.z)
		{
			return node;
		}		
	}
	return nullptr;
}
void   TQuadtree::FindNeighborNode()
{	
	for (auto node : g_pLeafNodes)
	{
		node->m_pNeighborList.resize(4);
		T::TVector3 p;		
		p.x = node->m_Box.vCenter.x;
		p.y = 0.0f;
		p.z = node->m_Box.vMax.z + node->m_Box.size.z;
		node->m_pNeighborList[0] = CheckBoxtoPoint(p); // 북

		p.z = node->m_Box.vMin.z - node->m_Box.size.z;
		node->m_pNeighborList[1] = CheckBoxtoPoint(p); // 남

		p.z = node->m_Box.vCenter.z;
		p.x = node->m_Box.vMin.x - node->m_Box.size.x;
		node->m_pNeighborList[2] = CheckBoxtoPoint(p); // 서	

		p.x = node->m_Box.vMax.x + node->m_Box.size.x;
		node->m_pNeighborList[3] = CheckBoxtoPoint(p); // 동		
		
	}
}
void		TQuadtree::GetRatio(TNode* pNode)
{
	T::TVector3 v = m_pCamera->m_vCamera - pNode->m_Box.vCenter;
	float fDistance = T::D3DXVec3Length(&v);
	float fRatio = fDistance / m_pCamera->m_fFarDistance;
	// lod level==> 0 ~ 1 :  0 ~ 0.25,  0.25 ~ 05,  0.5 ~  1.0f
	pNode->m_iCurrentLod = fRatio * m_iNumLOD; // 0,  1 , 2
	if (pNode->m_iCurrentLod >= m_iNumLOD)
	{
		pNode->m_iCurrentLod = m_iNumLOD - 1;
	}
}
int		TQuadtree::GetLodType(TNode* pNode)
{
	if (pNode->m_pNeighborList.size() <= 0) return 0;
	int dwType = 0;
	if (pNode->m_pNeighborList[0] && pNode->m_pNeighborList[0]->m_iCurrentLod < pNode->m_iCurrentLod) dwType += 1;
	if (pNode->m_pNeighborList[1] && pNode->m_pNeighborList[1]->m_iCurrentLod < pNode->m_iCurrentLod) dwType += 4;
	if (pNode->m_pNeighborList[2] && pNode->m_pNeighborList[2]->m_iCurrentLod < pNode->m_iCurrentLod) dwType += 8;
	if (pNode->m_pNeighborList[3] && pNode->m_pNeighborList[3]->m_iCurrentLod < pNode->m_iCurrentLod) dwType += 2;

	pNode->m_dwLodType = dwType;
	return dwType;
}
void		TQuadtree::Update(TCamera* pCamera)
{
	g_pDrawLeafNodes.clear();
	m_ObjectList.clear();
	RenderTile(m_pRootNode);

	// 전체 노드의 LOD레벨을 저장
	for (auto node : g_pLeafNodes)
	{
		GetRatio(node);
	}
	// 보이는 노드들의 LOD 타입(0~ 15타입)을 결정
	m_iNumFace = 0;
	for (auto node : g_pDrawLeafNodes)
	{		
		GetLodType(node);
		// 전체 인덱스 버퍼를 1개만 사용한다.
		// LOD버퍼를 저장
		m_iNumFace += UpdateIndexList(node,m_iNumFace * 3, node->m_iCurrentLod);
	}
}
void TQuadtree::RenderTile(TNode* pNode)
{
	if (pNode == nullptr) return;
	if (m_pCamera->ClassifyOBB(&pNode->m_Box) == TRUE)
	{
		for (auto obj : pNode->m_StaticObjectList)
		{
			if (m_pCamera->ClassifyOBB(&obj->box) == TRUE)
			{
				m_ObjectList.push_back(obj);
			}
		}
		if( pNode->m_bLeaf == true)
		{
			g_pDrawLeafNodes.push_back(pNode);
			return;
		}		
		for (int iNode = 0; iNode < pNode->m_pChild.size(); iNode++)
		{
			RenderTile(pNode->m_pChild[iNode]);
		}
	}
}
bool		TQuadtree::Render()
{
	m_pMap->PreRender();
	m_pMap->Draw();

	m_pMap->m_pContext->UpdateSubresource(
		m_pIndexBuffer.Get(), 0, NULL, &m_IndexList.at(0), 0, 0);
	m_pMap->m_pContext->IASetIndexBuffer(
		m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	m_pMap->m_pContext->DrawIndexed(m_iNumFace*3, 0, 0);

	// 노드의 바운딩 박스를 랜더링
	// 	   	// 노드의 바운딩 박스를 랜더링
	/*for (int iNode = 0; iNode < g_pDrawLeafNodes.size(); iNode++)
	{
		DrawDebugRender(&g_pDrawLeafNodes[iNode]->m_Box);
	}*/
	//for (int iNode = 0; iNode < g_pDrawLeafNodes.size(); iNode++)
	//{
	//	DrawDebugRender(&g_pDrawLeafNodes[iNode]->m_Box);
	//	/*m_pMap->m_ConstantList.Color = T::TVector4(1, 1, 0, 1);
	//	m_pMap->m_pContext->UpdateSubresource(
	//		m_pMap->m_pConstantBuffer, 0, NULL, &m_pMap->m_ConstantList, 0, 0);

	//	int iLod = g_pDrawLeafNodes[iNode]->m_iCurrentLod;
	//	
	//	m_pMap->m_pContext->IASetIndexBuffer(
	//			g_pDrawLeafNodes[iNode]->m_pIndexBuffer[iLod].Get(), DXGI_FORMAT_R32_UINT, 0);
	//	m_pMap->m_pContext->DrawIndexed(
	//		g_pDrawLeafNodes[iNode]->m_IndexList[iLod].size(), 0, 0);*/
	//}
	for (auto obj : m_ObjectList)
	{
		obj->pObject->SetMatrix(&obj->matWorld,
			&m_pMap->m_matView,
			&m_pMap->m_matProj);
		obj->pObject->m_ConstantList.Color = T::TVector4(1, 1, 1, 1);
		obj->pObject->Render();
	}
	return true;
}
void TQuadtree::DynamicDeleteObject(TNode* pNode)
{
	if (pNode == nullptr) return;
	pNode->m_DynamicObjectList.clear();
	for (int iNode = 0; iNode < 4; iNode++)
	{
		DynamicDeleteObject(pNode->m_pChild[iNode]);
	}
}
TNode* TQuadtree::CreateNode(
	TNode* pParent, float x, float y, float w, float h)
{
	// 0, 4, 20 ,24
	TNode* pNode = new TNode(x,y,w,h);
	pNode->m_iIndex = g_iCount++;
	if (pParent != nullptr)
	{
		pNode->m_iDepth = pParent->m_iDepth + 1;
	}
	GenBoundingBox(pNode);	
	//SetIndexData(pNode,1);
	//CreateIndexBuffer(pNode);
	return pNode;
}
TBox	TQuadtree::GenBoundingBox(TNode* pNode)
{
	TVector3 v0, v4;
	v0 = m_pMap->m_VertexList[pNode->m_CornerList[0]].p; // 0
	v4 = m_pMap->m_VertexList[pNode->m_CornerList[3]].p; // 24
	pNode->m_Box.vMin.x = v0.x;
	pNode->m_Box.vMin.z = v4.z;
	pNode->m_Box.vMax.x = v4.x;
	pNode->m_Box.vMax.z = v0.z;
	TVector2 vHeight = GetHeightFromNode(
		pNode->m_CornerList[0],
		pNode->m_CornerList[1],	
		pNode->m_CornerList[2], 
		pNode->m_CornerList[3]);
	pNode->m_Box.vMin.y = vHeight.y;
	pNode->m_Box.vMax.y = vHeight.x;
	pNode->m_Box.vAxis[0] = TVector3(1, 0, 0);
	pNode->m_Box.vAxis[1] = TVector3(0, 1, 0);
	pNode->m_Box.vAxis[2] = TVector3(0, 0, 1);
	pNode->m_Box.size.x = (pNode->m_Box.vMax.x- pNode->m_Box.vMin.x) / 2.0f;
	pNode->m_Box.size.y = (pNode->m_Box.vMax.y - pNode->m_Box.vMin.y) / 2.0f;
	pNode->m_Box.size.z = (pNode->m_Box.vMax.z - pNode->m_Box.vMin.z) / 2.0f;
	pNode->m_Box.vCenter = (pNode->m_Box.vMax + pNode->m_Box.vMin);
	pNode->m_Box.vCenter /= 2.0f;

	return pNode->m_Box;
}
TVector2	TQuadtree::GetHeightFromNode(DWORD dwTL, DWORD dwTR, DWORD dwBL, DWORD dwBR)
{
	assert(m_pMap);

	DWORD dwStartRow = dwTL / m_iWidth;
	DWORD dwEndRow = dwBL / m_iWidth;

	DWORD dwStartCol = dwTL % m_iWidth;
	DWORD dwEndCol = dwTR % m_iWidth;

	TVector2 vHeight;
	vHeight.x = -999999.0f;
	vHeight.y = 999999.0f;

	// 0,  4, 
	// 20 ,24
	
	for (DWORD dwRow = dwStartRow; dwRow < dwEndRow; dwRow++)
	{
		for (DWORD dwCol = dwStartCol; dwCol < dwEndCol; dwCol++)
		{
			if (m_pMap->m_VertexList[dwRow * m_iWidth + dwCol].p.y > vHeight.x)
			{
				vHeight.x = m_pMap->m_VertexList[dwRow * m_iWidth + dwCol].p.y;
			}
			if (m_pMap->m_VertexList[dwRow * m_iWidth + dwCol].p.y < vHeight.y)
			{
				vHeight.y = m_pMap->m_VertexList[dwRow * m_iWidth + dwCol].p.y;
			}
		}
	}

	return vHeight;
}
void		TQuadtree::SetIndexData(TNode* pNode, int iLodLevel)
{
	// (0) 1    (2)  3  (4)
	// 5   6     7    8   9
	// (10) 11  (12) 13 (14)
	// 15 16    17   18  19
	// (20) 21  (22) 23 (24)
	// 16-> 4 -> 1
	assert(m_pMap);
	pNode->m_IndexList.resize(iLodLevel);

	for (int iLod = 0; iLod < iLodLevel; iLod++)
	{
		int iOffset = pow(2,iLod);//; 0 ->1, 1-> 2, 2->4
		DWORD dwStartRow = pNode->m_CornerList[0] / m_iWidth;
		DWORD dwEndRow = pNode->m_CornerList[2] / m_iWidth;

		DWORD dwStartCol = pNode->m_CornerList[0] % m_iWidth;
		DWORD dwEndCol = pNode->m_CornerList[1] % m_iWidth;

		DWORD dwCellWidth = (dwEndCol - dwStartCol);
		DWORD dwCellHeight = (dwEndRow - dwStartRow);
		// 0,  4, 
		// 20 ,24
		int iNumFace = (dwCellWidth * dwCellHeight * 2) /  pow(4,iLod);
		pNode->m_IndexList[iLod].resize(iNumFace * 3);
		UINT iIndex = 0;
		for (DWORD iRow = dwStartRow; iRow < dwEndRow; iRow+= iOffset)
		{
			for (DWORD iCol = dwStartCol; iCol < dwEndCol; iCol+= iOffset)
			{
				pNode->m_IndexList[iLod][iIndex + 0] = iRow * m_iWidth + iCol;
				pNode->m_IndexList[iLod][iIndex + 1] = (iRow * m_iWidth + iCol) + iOffset;
				pNode->m_IndexList[iLod][iIndex + 2] = (iRow + iOffset) * m_iWidth + iCol;

				pNode->m_IndexList[iLod][iIndex + 3] = pNode->m_IndexList[iLod][iIndex + 2];
				pNode->m_IndexList[iLod][iIndex + 4] = pNode->m_IndexList[iLod][iIndex + 1];
				pNode->m_IndexList[iLod][iIndex + 5] = pNode->m_IndexList[iLod][iIndex + 2] + iOffset;
				iIndex += 6;
			}
		}
	}
}
bool	TQuadtree::CreateIndexBuffer(TNode* pNode, int iLodLevel)
{
	HRESULT hr;
	pNode->m_pIndexBuffer.resize(iLodLevel);
	for (int iLod = 0; iLod < iLodLevel; iLod++)
	{
		if (pNode->m_IndexList[iLod].size() <= 0) return true;
		//gpu메모리에 버퍼 할당(원하는 할당 크기)
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
		bd.ByteWidth = sizeof(DWORD) * pNode->m_IndexList[iLod].size();
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
		sd.pSysMem = &pNode->m_IndexList[iLod].at(0);

		if (FAILED(hr = m_pMap->m_pd3dDevice->CreateBuffer(&bd, &sd,
			pNode->m_pIndexBuffer[iLod].GetAddressOf())))
		{
			return false;
		}
	}
	return true;
}
void		TQuadtree::Build(TMap* pMap, int iMaxDepth)
{
	HRESULT hr;
	m_pMap = pMap;
	m_IndexList.resize(m_pMap->m_IndexList.size());
	//gpu메모리에 버퍼 할당(원하는 할당 크기)
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(DWORD) * m_IndexList.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;	
	if (FAILED(hr = m_pMap->m_pd3dDevice->CreateBuffer(&bd, NULL,
						m_pIndexBuffer.GetAddressOf())))
	{
		return;
	}

	m_iLeafDepth = iMaxDepth;
	m_iWidth = pMap->m_iNumCols;
	m_iHeight = pMap->m_iNumRows;
	// 0   1  2  3  4
	// 5   6  7  8  9
	// 10 11 12 13 14
	// 15 16 17 18 19
	// 20 21 22 23 24
	m_pRootNode = CreateNode(nullptr,	0, 
										m_iWidth-1, 
										m_iWidth*(m_iHeight-1), 
										m_iWidth*m_iHeight-1);
	BuildTree(m_pRootNode);
	FindNeighborNode();

	for (auto node : g_pLeafNodes)
	{
		SetIndexData(node, m_iNumLOD);// *16;
		CreateIndexBuffer(node, m_iNumLOD);// *16;
	}
}
void TQuadtree::Build(int iWidth, int iHeight, int iMaxDepth)
{
	m_iLeafDepth = iMaxDepth;
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	// 0,0  -    a       100,0
	// -    0    -    1 
	// d    -    m       b
	//      3         2
	// 0,100     c    100,100
	m_pRootNode = CreateNode(nullptr, 
		0, 0, m_iWidth, m_iHeight);
	BuildTree(m_pRootNode);
}
void TQuadtree::BuildTree(TNode* pParent)
{
	if (pParent == nullptr )
	{
		return;
	}
	if ((pParent->m_CornerList[1] - pParent->m_CornerList[0]) == 1)
	{
		m_iMaxDepth = pParent->m_iDepth;
		m_iNumLOD = (m_iMaxDepth - m_iLeafLOD) + 1;
		return;
	}
	if (pParent->m_iDepth == m_iLeafDepth)
	{
		m_iLeafLOD = pParent->m_iDepth;
		pParent->m_bLeaf = true;

		g_pLeafNodes.push_back(pParent);
		//return;
	}
	// (0) 1    (2)  3  (4)
	// 5   6     7    8   9
	// (10) 11  (12) 13 (14)
	// 15 16    17   18  19
	// (20) 21  (22) 23 (24)
	int iT = (pParent->m_CornerList[0] + pParent->m_CornerList[1]) / 2; // 2
	int iB = (pParent->m_CornerList[2] + pParent->m_CornerList[3]) / 2; // 22
	int iL = (pParent->m_CornerList[0] + pParent->m_CornerList[2]) / 2; // 10
	int iR = (pParent->m_CornerList[1] + pParent->m_CornerList[3]) / 2; // 14
	int iC = (pParent->m_CornerList[0] + pParent->m_CornerList[3]) / 2; // 12
	// 0,2,10,12	
	pParent->m_pChild[0] = CreateNode(pParent,	pParent->m_CornerList[0],iT,iL,iC);
	// 2,4,12,14	
	pParent->m_pChild[1] = CreateNode(pParent,iT,pParent->m_CornerList[1],iC,iR);
	// 10,12, 20, 22	
	pParent->m_pChild[2] = CreateNode(pParent,iL,iC,pParent->m_CornerList[2],iB);
	// 12,14,22,24
	pParent->m_pChild[3] = CreateNode(pParent,iC, iR, iB, pParent->m_CornerList[3]);
	for (int iChild = 0; iChild < pParent->m_pChild.size(); iChild++)
	{
		BuildTree(pParent->m_pChild[iChild]);
	}
}
bool TQuadtree::AddObject(TMapObject* obj)
{
	TNode* pFindNode = 	FindNode(m_pRootNode, obj->box);
	if (pFindNode != nullptr)
	{
		pFindNode->AddObject(obj);
		//if (pFindNode->m_bLeaf == false)
		//{
		//	m_ObjectList.push_back(obj);
		//}
		return true;
	}
	return false;
}
void TQuadtree::RenderObject(TNode* pNode)
{
	if (pNode == nullptr) return;
	if (m_pCamera->ClassifyOBB(&pNode->m_Box) == TRUE)
	{
		for (auto obj : pNode->m_StaticObjectList)
		{
			m_ObjectList.push_back(obj);
		}
		for (int iNode = 0; iNode < pNode->m_pChild.size(); iNode++)
		{
			RenderObject(pNode->m_pChild[iNode]);
		}
	}
}
bool TQuadtree::AddDynamicObject(TMapObject* obj)
{
	TNode* pFindNode =
		FindNode(m_pRootNode, obj->box);
	if (pFindNode != nullptr)
	{
		//obj->m_iNodeIndex = pFindNode->m_iIndex;
		pFindNode->AddDynamicObject(obj);
		return true;
	}
	return false;
}
// aabb
bool   TQuadtree::CheckBox(TBox& a, TBox& b)
{
	if (a.vMin.x <= b.vMin.x && a.vMin.y <= b.vMin.y &&
		a.vMin.z <= b.vMin.z)
	{
		if (a.vMax.x >= b.vMax.x && a.vMax.y >= b.vMax.y &&
			a.vMax.z >= b.vMax.z)
		{
			return true;
		}
	}
	return false;
}
TNode* TQuadtree::FindNode(TNode* pNode, TBox& box)
{	
	do {		
		for (int iNode = 0; iNode < 4; iNode++)
		{
			if (pNode->m_pChild[iNode] != nullptr)
			{
				if (CheckBox(pNode->m_pChild[iNode]->m_Box,
							 box))
				{
					g_Queue.push(pNode->m_pChild[iNode]);
					break;
				}
			}
		}		
		if (g_Queue.empty()) break;
		pNode = g_Queue.front();
		g_Queue.pop();
	} while (pNode);
	return pNode;
}

void TQuadtree::PrintObjectList(TNode* pNode)
{
	if (pNode == nullptr) return;
	for (std::list<TMapObject*>::iterator iter = pNode->m_DynamicObjectList.begin();
		iter != pNode->m_DynamicObjectList.end();
		iter++)
	{
		TMapObject* pObj = *iter;
		std::cout << "[" << pNode->m_iIndex << "]" <<
			(int)pObj->vPos.x <<":"<< (int)pObj->vPos.y << " ";
	}
	std::cout << std::endl;
	for (int iNode = 0; iNode < 4; iNode++)
	{
		PrintObjectList(pNode->m_pChild[iNode]);
	}
}
int TQuadtree::UpdateIndexList(TNode* pNode, DWORD dwCurentIndex, DWORD dwNumLevel)
{
	if (m_IndexList.size() <=0) return false;
	int iNumFaces = 0;

	DWORD dwTL = pNode->m_CornerList[0];
	DWORD dwTR = pNode->m_CornerList[1];
	DWORD dwBL = pNode->m_CornerList[2];
	DWORD dwBR = pNode->m_CornerList[3];

	DWORD dwStartRow = dwTL / m_iWidth;
	DWORD dwEndRow = dwBL / m_iWidth;

	DWORD dwStartCol = dwTL % m_iWidth;
	DWORD dwEndCol = dwTR % m_iWidth;

	DWORD dwOffset = (DWORD)(pow(2.0f, (float)dwNumLevel));
	DWORD dwCountX = ((dwEndCol - dwStartCol) / dwOffset) - 1;
	DWORD dwCountY = ((dwEndRow - dwStartRow) / dwOffset) - 1;


	DWORD dwYCell = 0;
	for (DWORD dwRow = dwStartRow; dwRow < dwEndRow; dwRow += dwOffset, dwYCell++)
	{
		DWORD dwXCell = 0;
		for (DWORD dwCol = dwStartCol; dwCol < dwEndCol; dwCol += dwOffset, dwXCell++)
		{
			//1	2    2   
			//0	   5 4
			DWORD dwNextRow = dwRow + dwOffset;
			DWORD dwNextCol = dwCol + dwOffset;

			DWORD i0 = dwNextRow * m_iWidth + dwCol;    // 2
			DWORD i1 = dwRow * m_iWidth + dwCol;		// 0
			DWORD i2 = dwRow * m_iWidth + dwNextCol;	// 1	

			// 왼쪽, 상단
			if ((dwXCell == 0 && dwYCell == 0))
			{
				DWORD dwType = (pNode->m_dwLodType & 8) + (pNode->m_dwLodType & 1);
				iNumFaces += SetLodIndexBuffer(pNode, dwCurentIndex,
					i0,	// 2
					i1,		// 0
					i2,	// 1											
					dwType);
			}
			else if ((dwXCell == 0) && (pNode->m_dwLodType & 8))
			{
				iNumFaces += SetLodIndexBuffer(pNode, dwCurentIndex,
					i0,	// 2
					i1,		// 0
					i2,	// 1												
					8);
			}
			else if ((dwYCell == 0) && (pNode->m_dwLodType & 1))
			{
				iNumFaces += SetLodIndexBuffer(pNode, dwCurentIndex,
					i0,	// 2
					i1,		// 0
					i2,	// 1											
					1);
			}
			else
			{
				m_IndexList[dwCurentIndex + 0] = i0;
				m_IndexList[dwCurentIndex + 1] = i1;
				m_IndexList[dwCurentIndex + 2] = i2;
				iNumFaces += 1;
				dwCurentIndex += 3;
			}

			DWORD j0 = dwRow * m_iWidth + dwNextCol;		// 2
			DWORD j1 = dwNextRow * m_iWidth + dwNextCol;	// 3
			DWORD j2 = dwNextRow * m_iWidth + dwCol;		// 0			

			if ((dwXCell == dwCountX && dwYCell == dwCountY))
			{
				DWORD dwType = (pNode->m_dwLodType & 2) + (pNode->m_dwLodType & 4);
				iNumFaces += SetLodIndexBuffer(pNode, dwCurentIndex,
					j0,		// 2
					j1,	// 3
					j2,		// 0																					
					dwType);
			}
			else if ((dwXCell == dwCountX) && (pNode->m_dwLodType & 2))
			{
				iNumFaces += SetLodIndexBuffer(pNode, dwCurentIndex,
					j0,	// 2
					j1,	// 3
					j2,	// 0																					
					2);
			}
			else if ((dwYCell == dwCountY) && (pNode->m_dwLodType & 4))
			{
				iNumFaces += SetLodIndexBuffer(pNode, dwCurentIndex,
					j0,	// 2
					j1,	// 3
					j2,	// 0																						
					4);
			}
			else
			{
				m_IndexList[dwCurentIndex + 0] = j0;
				m_IndexList[dwCurentIndex + 1] = j1;
				m_IndexList[dwCurentIndex + 2] = j2;
				iNumFaces += 1;
				dwCurentIndex += 3;
			}
		}
	}
	return iNumFaces;
}

int  TQuadtree::SetLodIndexBuffer(TNode* pNode,
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
void TQuadtree::DrawDebugInit(ID3D11Device* pd3dDevice,
	ID3D11DeviceContext* pContext)
{
	m_BoxDebug.m_pColorTex = I_Texture.Load(L"../../data/charport.bmp");
	m_BoxDebug.m_pVShader = I_Shader.CreateVertexShader(
		pd3dDevice, L"Box.hlsl", "VSColor");
	m_BoxDebug.m_pPShader = I_Shader.CreatePixelShader(
		pd3dDevice, L"Box.hlsl", "PSColor");
	m_BoxDebug.SetPosition(T::TVector3(0.0f, 1.0f, 0.0f));
	if (!m_BoxDebug.Create(pd3dDevice, pContext))
	{
		return;
	}
}
void TQuadtree::DrawDebugRender(TBox*  pBox)
{
	pBox->vList[0] = T::TVector3(pBox->vMin.x,
		pBox->vMax.y,
		pBox->vMin.z);
	pBox->vList[1] = T::TVector3(pBox->vMax.x,
		pBox->vMax.y,
		pBox->vMin.z);
	pBox->vList[2] = T::TVector3(pBox->vMin.x,
		pBox->vMin.y,
		pBox->vMin.z);
	pBox->vList[3] = T::TVector3(pBox->vMax.x,
		pBox->vMin.y,
		pBox->vMin.z);

	pBox->vList[4] = T::TVector3(pBox->vMin.x,
		pBox->vMax.y,
		pBox->vMax.z);
	pBox->vList[5] = T::TVector3(pBox->vMax.x,
		pBox->vMax.y,
		pBox->vMax.z);
	pBox->vList[6] = T::TVector3(pBox->vMin.x,
		pBox->vMin.y,
		pBox->vMax.z);
	pBox->vList[7] = T::TVector3(pBox->vMax.x,
		pBox->vMin.y,
		pBox->vMax.z);
	// 4      5
	// 6      7

	// 0     1
	// 2     3
	int index = 0;
	// +z
	m_BoxDebug.m_VertexList[index].p = pBox->vList[5];
	m_BoxDebug.m_VertexList[index].n = T::TVector3(0.0f, 0.0f, 1.0f);
	m_BoxDebug.m_VertexList[index].c = T::TVector4(0.0f, 0.0f, 1.0f, 1.0f);
	m_BoxDebug.m_VertexList[index].t = T::TVector2(0.0f, 0.0f);

	m_BoxDebug.m_VertexList[++index].p = pBox->vList[4];
	m_BoxDebug.m_VertexList[index].n = T::TVector3(0.0f, 0.0f, 1.0f);
	m_BoxDebug.m_VertexList[index].c = T::TVector4(0.0f, 0.0f, 1.0f, 1.0f);
	m_BoxDebug.m_VertexList[index].t = T::TVector2(1.0f, 0.0f);

	m_BoxDebug.m_VertexList[++index].p = pBox->vList[7];
	m_BoxDebug.m_VertexList[index].n = T::TVector3(0.0f, 0.0f, 1.0f);
	m_BoxDebug.m_VertexList[index].c = T::TVector4(0.0f, 0.0f, 1.0f, 1.0f);
	m_BoxDebug.m_VertexList[index].t = T::TVector2(0.0f, 1.0f);

	m_BoxDebug.m_VertexList[++index].p = pBox->vList[6];
	m_BoxDebug.m_VertexList[index].n = T::TVector3(0.0f, 0.0f, 1.0f);
	m_BoxDebug.m_VertexList[index].c = T::TVector4(0.0f, 0.0f, 1.0f, 1.0f);
	m_BoxDebug.m_VertexList[index].t = T::TVector2(1.0f, 1.0f);
	// -Z plane
	m_BoxDebug.m_VertexList[++index].p = pBox->vList[0];
	m_BoxDebug.m_VertexList[index].n = T::TVector3(0.0f, 0.0f, -1.0f);
	m_BoxDebug.m_VertexList[index].c = T::TVector4(1.0f, 0.0f, 0.0f, 1.0f);
	m_BoxDebug.m_VertexList[index].t = T::TVector2(0.0f, 0.0f);

	m_BoxDebug.m_VertexList[++index].p = pBox->vList[1];
	m_BoxDebug.m_VertexList[index].n = T::TVector3(0.0f, 0.0f, -1.0f);
	m_BoxDebug.m_VertexList[index].c = T::TVector4(1.0f, 0.0f, 0.0f, 1.0f);
	m_BoxDebug.m_VertexList[index].t = T::TVector2(1.0f, 0.0f);

	m_BoxDebug.m_VertexList[++index].p = pBox->vList[2];
	m_BoxDebug.m_VertexList[index].n = T::TVector3(0.0f, 0.0f, -1.0f);
	m_BoxDebug.m_VertexList[index].c = T::TVector4(1.0f, 0.0f, 0.0f, 1.0f);
	m_BoxDebug.m_VertexList[index].t = T::TVector2(0.0f, 1.0f);

	m_BoxDebug.m_VertexList[++index].p = pBox->vList[3];
	m_BoxDebug.m_VertexList[index].n = T::TVector3(0.0f, 0.0f, -1.0f);
	m_BoxDebug.m_VertexList[index].c = T::TVector4(1.0f, 0.0f, 0.0f, 1.0f);
	m_BoxDebug.m_VertexList[index].t = T::TVector2(1.0f, 1.0f);

	// +X plane
	m_BoxDebug.m_VertexList[++index].p = pBox->vList[1];
	m_BoxDebug.m_VertexList[index].n = T::TVector3(1.0f, 0.0f, 0.0f);
	m_BoxDebug.m_VertexList[index].c = T::TVector4(1.0f, 1.0f, 0.0f, 1.0f);
	m_BoxDebug.m_VertexList[index].t = T::TVector2(0.0f, 0.0f);

	m_BoxDebug.m_VertexList[++index].p = pBox->vList[5];
	m_BoxDebug.m_VertexList[index].n = T::TVector3(1.0f, 0.0f, 0.0f);
	m_BoxDebug.m_VertexList[index].c = T::TVector4(1.0f, 1.0f, 0.0f, 1.0f);
	m_BoxDebug.m_VertexList[index].t = T::TVector2(1.0f, 0.0f);

	m_BoxDebug.m_VertexList[++index].p = pBox->vList[3];
	m_BoxDebug.m_VertexList[index].n = T::TVector3(1.0f, 0.0f, 0.0f);
	m_BoxDebug.m_VertexList[index].c = T::TVector4(1.0f, 1.0f, 0.0f, 1.0f);
	m_BoxDebug.m_VertexList[index].t = T::TVector2(0.0f, 1.0f);

	m_BoxDebug.m_VertexList[++index].p = pBox->vList[7];
	m_BoxDebug.m_VertexList[index].n = T::TVector3(1.0f, 0.0f, 0.0f);
	m_BoxDebug.m_VertexList[index].c = T::TVector4(1.0f, 1.0f, 0.0f, 1.0f);
	m_BoxDebug.m_VertexList[index].t = T::TVector2(1.0f, 1.0f);

	// -X plane
	m_BoxDebug.m_VertexList[++index].p = pBox->vList[4];
	m_BoxDebug.m_VertexList[index].n = T::TVector3(-1.0f, 0.0f, 0.0f);
	m_BoxDebug.m_VertexList[index].c = T::TVector4(0.0f, 1.0f, 1.0f, 1.0f);
	m_BoxDebug.m_VertexList[index].t = T::TVector2(0.0f, 0.0f);

	m_BoxDebug.m_VertexList[++index].p = pBox->vList[0];
	m_BoxDebug.m_VertexList[index].n = T::TVector3(-1.0f, 0.0f, 0.0f);
	m_BoxDebug.m_VertexList[index].c = T::TVector4(0.0f, 1.0f, 1.0f, 1.0f);
	m_BoxDebug.m_VertexList[index].t = T::TVector2(1.0f, 0.0f);

	m_BoxDebug.m_VertexList[++index].p = pBox->vList[6];
	m_BoxDebug.m_VertexList[index].n = T::TVector3(-1.0f, 0.0f, 0.0f);
	m_BoxDebug.m_VertexList[index].c = T::TVector4(0.0f, 1.0f, 1.0f, 1.0f);
	m_BoxDebug.m_VertexList[index].t = T::TVector2(0.0f, 1.0f);

	m_BoxDebug.m_VertexList[++index].p = pBox->vList[2];
	m_BoxDebug.m_VertexList[index].n = T::TVector3(-1.0f, 0.0f, 0.0f);
	m_BoxDebug.m_VertexList[index].c = T::TVector4(0.0f, 1.0f, 1.0f, 1.0f);
	m_BoxDebug.m_VertexList[index].t = T::TVector2(1.0f, 1.0f);

	// -y plane
	m_BoxDebug.m_VertexList[++index].p = pBox->vList[2];
	m_BoxDebug.m_VertexList[index].n = T::TVector3(0.0f, -1.0f, 0.0f);
	m_BoxDebug.m_VertexList[index].c = T::TVector4(0.0f, 0.0f, 0.0f, 1.0f);
	m_BoxDebug.m_VertexList[index].t = T::TVector2(0.0f, 0.0f);

	m_BoxDebug.m_VertexList[++index].p = pBox->vList[3];
	m_BoxDebug.m_VertexList[index].n = T::TVector3(0.0f, -1.0f, 0.0f);
	m_BoxDebug.m_VertexList[index].c = T::TVector4(0.0f, 0.0f, 0.0f, 1.0f);
	m_BoxDebug.m_VertexList[index].t = T::TVector2(1.0f, 0.0f);

	m_BoxDebug.m_VertexList[++index].p = pBox->vList[6];
	m_BoxDebug.m_VertexList[index].n = T::TVector3(0.0f, -1.0f, 0.0f);
	m_BoxDebug.m_VertexList[index].c = T::TVector4(0.0f, 0.0f, 0.0f, 1.0f);
	m_BoxDebug.m_VertexList[index].t = T::TVector2(0.0f, 1.0f);

	m_BoxDebug.m_VertexList[++index].p = pBox->vList[7];
	m_BoxDebug.m_VertexList[index].n = T::TVector3(0.0f, -1.0f, 0.0f);
	m_BoxDebug.m_VertexList[index].c = T::TVector4(0.0f, 0.0f, 0.0f, 1.0f);
	m_BoxDebug.m_VertexList[index].t = T::TVector2(1.0f, 1.0f);
	// +y plane
	m_BoxDebug.m_VertexList[++index].p = pBox->vList[4];
	m_BoxDebug.m_VertexList[index].n = T::TVector3(0.0f, 1.0f, 0.0f);
	m_BoxDebug.m_VertexList[index].c = T::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_BoxDebug.m_VertexList[index].t = T::TVector2(0.0f, 0.0f);

	m_BoxDebug.m_VertexList[++index].p = pBox->vList[5];
	m_BoxDebug.m_VertexList[index].n = T::TVector3(0.0f, 1.0f, 0.0f);
	m_BoxDebug.m_VertexList[index].c = T::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_BoxDebug.m_VertexList[index].t = T::TVector2(1.0f, 0.0f);

	m_BoxDebug.m_VertexList[++index].p = pBox->vList[0];
	m_BoxDebug.m_VertexList[index].n = T::TVector3(0.0f, 1.0f, 0.0f);
	m_BoxDebug.m_VertexList[index].c = T::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_BoxDebug.m_VertexList[index].t = T::TVector2(0.0f, 1.0f);

	m_BoxDebug.m_VertexList[++index].p = pBox->vList[1];
	m_BoxDebug.m_VertexList[index].n = T::TVector3(0.0f, 1.0f, 0.0f);
	m_BoxDebug.m_VertexList[index].c = T::TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_BoxDebug.m_VertexList[index].t = T::TVector2(1.0f, 1.0f);

	m_BoxDebug.SetMatrix(NULL, &m_pCamera->m_matView, &m_pCamera->m_matProj);
	m_BoxDebug.PreRender();
	m_BoxDebug.Draw();
	m_BoxDebug.m_pContext->UpdateSubresource(
		m_BoxDebug.m_pVertexBuffer, 0, NULL, &m_BoxDebug.m_VertexList.at(0), 0, 0);
	m_BoxDebug.PostRender();
}