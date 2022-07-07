#include "TQuadtree.h"
int TQuadtree::g_iCount = 0;
void		TQuadtree::Update(TCamera* pCamera)
{
	g_pDrawLeafNodes.clear();
	m_ObjectList.clear();
	RenderTile(m_pRootNode);
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

	for (int iNode = 0; iNode < g_pDrawLeafNodes.size(); iNode++)
	{
		m_pMap->m_ConstantList.Color = T::TVector4(1, 1, 0, 1);
		m_pMap->m_pContext->UpdateSubresource(
			m_pMap->m_pConstantBuffer, 0, NULL, &m_pMap->m_ConstantList, 0, 0);

		m_pMap->m_pContext->IASetIndexBuffer(
			g_pDrawLeafNodes[iNode]->m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		m_pMap->m_pContext->DrawIndexed(g_pDrawLeafNodes[iNode]->m_IndexList.size(), 0, 0);
	}
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
	//SetIndexData(pNode);
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
void		TQuadtree::SetIndexData(TNode* pNode)
{
	// (0) 1    (2)  3  (4)
	// 5   6     7    8   9
	// (10) 11  (12) 13 (14)
	// 15 16    17   18  19
	// (20) 21  (22) 23 (24)
	assert(m_pMap);

	DWORD dwStartRow = pNode->m_CornerList[0] / m_iWidth;
	DWORD dwEndRow = pNode->m_CornerList[2] / m_iWidth;

	DWORD dwStartCol = pNode->m_CornerList[0] % m_iWidth;
	DWORD dwEndCol = pNode->m_CornerList[1] % m_iWidth;

	DWORD dwCellWidth = (dwEndCol - dwStartCol);
	DWORD dwCellHeight = (dwEndRow - dwStartRow);
	// 0,  4, 
	// 20 ,24
	int iNumFace = dwCellWidth * dwCellHeight * 2 ;
	pNode->m_IndexList.resize(iNumFace * 3);
	UINT iIndex = 0;
	for (DWORD iRow = dwStartRow; iRow < dwEndRow; iRow++)
	{
		for (DWORD iCol = dwStartCol; iCol < dwEndCol; iCol++)
		{
			pNode->m_IndexList[iIndex + 0] = iRow * m_iWidth + iCol;
			pNode->m_IndexList[iIndex + 1] = (iRow * m_iWidth + iCol) + 1;
			pNode->m_IndexList[iIndex + 2] = (iRow + 1) * m_iWidth + iCol;

			pNode->m_IndexList[iIndex + 3] = pNode->m_IndexList[iIndex + 2];
			pNode->m_IndexList[iIndex + 4] = pNode->m_IndexList[iIndex + 1];
			pNode->m_IndexList[iIndex + 5] = pNode->m_IndexList[iIndex + 2] + 1;
			iIndex += 6;
		}
	}	
}
bool	TQuadtree::CreateIndexBuffer(TNode* pNode)
{
	HRESULT hr;
	if (pNode->m_IndexList.size() <= 0) return true;
	//gpu메모리에 버퍼 할당(원하는 할당 크기)
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(DWORD) * pNode->m_IndexList.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = &pNode->m_IndexList.at(0);

	if (FAILED(hr = m_pMap->m_pd3dDevice->CreateBuffer(&bd, &sd, 
		pNode->m_pIndexBuffer.GetAddressOf())))
	{
		return false;
	}
	return true;
}
void		TQuadtree::Build(TMap* pMap, int iMaxDepth)
{
	m_pMap = pMap;
	m_iMaxDepth = iMaxDepth;
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
}
void TQuadtree::Build(int iWidth, int iHeight, int iMaxDepth)
{
	m_iMaxDepth = iMaxDepth;
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
	if (pParent == nullptr)
	{
		return;
	}
	if (pParent->m_iDepth == m_iMaxDepth)
	{
		pParent->m_bLeaf = true;
		SetIndexData(pParent);
		CreateIndexBuffer(pParent);
		g_pLeafNodes.push_back(pParent);
		return;
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

//void TQuadtree::BinaryNodePrintInOrder(TNode* pNode)
//{
//	if (pNode == nullptr) return;
//	BinaryNodePrintInOrder(pNode->pChild[0]);
//	std::cout << pNode->iValue;
//	BinaryNodePrintInOrder(pNode->pChild[1]);
//}
//void TQuadtree::BinaryNodePrintPostOrder(TNode* pNode)
//{
//	if (pNode == nullptr) return;
//	BinaryNodePrintPostOrder(pNode->pChild[0]);
//	BinaryNodePrintPostOrder(pNode->pChild[1]);
//	std::cout << pNode->iValue;
//}
//void TQuadtree::BinaryNodePrintLevelOrder(TNode* pNode)
//{
//	std::cout << pNode->iValue;
//	do {
//		if (pNode->pChild[0] != nullptr)
//			g_Queue.push(pNode->pChild[0]);
//		if (pNode->pChild[1] != nullptr)
//			g_Queue.push(pNode->pChild[1]);
//
//		if (g_Queue.empty()) break;
//		pNode = g_Queue.front();
//		std::cout << pNode->iValue;
//		g_Queue.pop();
//	} while (pNode);
//}
