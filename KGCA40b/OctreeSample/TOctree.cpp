#include "TOctree.h"
int	TOctree::m_iNodeCount = 0;
bool TOctree::AddObject(TObject* obj)
{
	TNode* pFindNode = FindNode(m_pRootNode, obj->m_rt);
	if (pFindNode != nullptr)
	{
		pFindNode->AddObject(obj);
		return true;
	}
	return false;
}
void TOctree::DynamicDeleteObject(TNode* pNode)
{
	if (pNode == nullptr) return;
	pNode->m_DynamicObjectList.clear();
	for (int iNode = 0; iNode < 8; iNode++)
	{
		DynamicDeleteObject(pNode->pChild[iNode]);
	}
}
bool TOctree::AddDynamicObject(TObject* obj)
{
	TNode* pFindNode = FindNode(m_pRootNode, obj->m_rt);
	if (pFindNode != nullptr)
	{
		obj->m_iNodeIndex = pFindNode->m_iIndex;
		pFindNode->AddDynamicObject(obj);
		return true;
	}
	return false;
}
TNode* TOctree::CreateNode(
	TNode* pParent, float x, float y, float z,
	float w, float h, float q)
{
	TNode* pNode = new TNode(x,y,z, w,h,q);
	pNode->m_iIndex = m_iNodeCount++;
	if (pParent != nullptr)
	{
		pNode->m_iDepth = pParent->m_iDepth + 1;
	}
	return pNode;
}
void TOctree::Init(int iWidth, int iHeight, 
	int iDepthint, int  iMaxDepth)
{
	m_iMaxDepth = iMaxDepth;
	m_iSizeX = iWidth;
	m_iSizeY = iHeight;
	m_iSizeZ = iHeight;
	// 0,0  -    a       100,0
	// -    0    -    1 
	// d    -    m       b
	//      3         2
	// 0,100     c    100,100
	m_pRootNode = CreateNode(nullptr, 
		0, 0, 0, 
		m_iSizeX, m_iSizeY, m_iSizeZ);
	BuildTree(m_pRootNode);
}
void TOctree::BuildTree(TNode* pParent)
{
	if (pParent->m_iDepth == m_iMaxDepth) return;
	pParent->pChild[0] = CreateNode(pParent,
		pParent->m_rt.vMin.x,
		pParent->m_rt.vMin.y,
		pParent->m_rt.vMin.z,
		pParent->m_rt.size.x / 2.0f,
		pParent->m_rt.size.y / 2.0f,
		pParent->m_rt.size.z / 2.0f);
	BuildTree(pParent->pChild[0]);
	pParent->pChild[1] = CreateNode(pParent,
		pParent->m_rt.vMiddle.x,
		pParent->m_rt.vMin.y,
		pParent->m_rt.vMin.z,
		pParent->m_rt.size.x / 2.0f,
		pParent->m_rt.size.y / 2.0f,
		pParent->m_rt.size.z / 2.0f);
	BuildTree(pParent->pChild[1]);
	pParent->pChild[2] = CreateNode(pParent,
		pParent->m_rt.vMiddle.x,
		pParent->m_rt.vMiddle.y,
		pParent->m_rt.vMin.z,
		pParent->m_rt.size.x / 2.0f,
		pParent->m_rt.size.y / 2.0f,
		pParent->m_rt.size.z / 2.0f);
	BuildTree(pParent->pChild[2]);
	pParent->pChild[3] = CreateNode(pParent,
		pParent->m_rt.vMin.x,
		pParent->m_rt.vMiddle.y,
		pParent->m_rt.vMin.z,
		pParent->m_rt.size.x / 2.0f,
		pParent->m_rt.size.y / 2.0f,
		pParent->m_rt.size.z / 2.0f);
	BuildTree(pParent->pChild[3]);


	//
	pParent->pChild[4] = CreateNode(pParent,
		pParent->m_rt.vMin.x,
		pParent->m_rt.vMin.y,
		pParent->m_rt.vMiddle.z,
		pParent->m_rt.size.x / 2.0f,
		pParent->m_rt.size.y / 2.0f,
		pParent->m_rt.size.z / 2.0f);
	BuildTree(pParent->pChild[4]);
	pParent->pChild[5] = CreateNode(pParent,
		pParent->m_rt.vMiddle.x,
		pParent->m_rt.vMin.y,
		pParent->m_rt.vMiddle.z,
		pParent->m_rt.size.x / 2.0f,
		pParent->m_rt.size.y / 2.0f,
		pParent->m_rt.size.z / 2.0f);
	BuildTree(pParent->pChild[5]);
	pParent->pChild[6] = CreateNode(pParent,
		pParent->m_rt.vMiddle.x,
		pParent->m_rt.vMiddle.y,
		pParent->m_rt.vMiddle.z,
		pParent->m_rt.size.x / 2.0f,
		pParent->m_rt.size.y / 2.0f,
		pParent->m_rt.size.z / 2.0f);
	BuildTree(pParent->pChild[6]);
	pParent->pChild[7] = CreateNode(pParent,
		pParent->m_rt.vMin.x,
		pParent->m_rt.vMiddle.y,
		pParent->m_rt.vMiddle.z,
		pParent->m_rt.size.x / 2.0f,
		pParent->m_rt.size.y / 2.0f,
		pParent->m_rt.size.z / 2.0f);
	BuildTree(pParent->pChild[7]);
}


TNode* TOctree::FindNode(TNode* pNode, TBox rt)
{
	do {
		for (int iNode = 0; iNode < 4; iNode++)
		{
			if (pNode->pChild[iNode] != nullptr)
			{
				TRect intersect;
				TCollisionType iRet = TCollision::BoxToBox(
						pNode->pChild[iNode]->m_rt,
						rt);
				if( iRet == RECT_OVERLAP)
				{
					g_Queue.push(pNode->pChild[iNode]);
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
void TOctree::PrintObjectList(TNode* pNode)
{
	if (pNode == nullptr) return;
	for (std::list<TObject*>::iterator iter = pNode->m_DynamicObjectList.begin();
		iter != pNode->m_DynamicObjectList.end();
		iter++)
	{
		TObject* pObj = *iter;
		std::cout << "[" << pNode->m_iIndex << "]" <<
			pObj->m_vPos.x <<":"<< pObj->m_vPos.y << ":" << pObj->m_vPos.z << " ";
	}
	std::cout << std::endl;
	for (int iNode = 0; iNode < 4; iNode++)
	{
		PrintObjectList(pNode->pChild[iNode]);
	}
}

//void TOctree::BinaryNodePrintInOrder(TNode* pNode)
//{
//	if (pNode == nullptr) return;
//	BinaryNodePrintInOrder(pNode->pChild[0]);
//	std::cout << pNode->iValue;
//	BinaryNodePrintInOrder(pNode->pChild[1]);
//}
//void TOctree::BinaryNodePrintPostOrder(TNode* pNode)
//{
//	if (pNode == nullptr) return;
//	BinaryNodePrintPostOrder(pNode->pChild[0]);
//	BinaryNodePrintPostOrder(pNode->pChild[1]);
//	std::cout << pNode->iValue;
//}
//void TOctree::BinaryNodePrintLevelOrder(TNode* pNode)
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
