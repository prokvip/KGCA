#include "TQuadtree.h"
int TQuadtree::g_iCount = 0;
void TQuadtree::DynamicDeleteObject(TNode* pNode)
{
	if (pNode == nullptr) return;
	pNode->m_DynamicObjectList.clear();
	for (int iNode = 0; iNode < 4; iNode++)
	{
		DynamicDeleteObject(pNode->pChild[iNode]);
	}
}
TNode* TQuadtree::CreateNode(
	TNode* pParent, float x, float y, float w, float h)
{
	TNode* pNode = new TNode(x,y,w,h);
	pNode->m_iIndex = g_iCount++;
	if (pParent != nullptr)
	{
		pNode->m_iDepth = pParent->m_iDepth + 1;
	}
	return pNode;
}
void TQuadtree::Init(int iWidth, int iHeight, int iMaxDepth)
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
	if (pParent->m_iDepth == m_iMaxDepth) return;
	pParent->pChild[0] = CreateNode(pParent,
		pParent->m_rt.vMin.x,
		pParent->m_rt.vMin.y,
		pParent->m_rt.size.x / 2.0f,
		pParent->m_rt.size.y / 2.0f );
	BuildTree(pParent->pChild[0]);
	pParent->pChild[1] = CreateNode(pParent,
		pParent->m_rt.middle.x,
		pParent->m_rt.vMin.y,
		pParent->m_rt.size.x / 2.0f,
		pParent->m_rt.size.y / 2.0f);
	BuildTree(pParent->pChild[1]);
	pParent->pChild[2] = CreateNode(pParent,
		pParent->m_rt.middle.x,
		pParent->m_rt.middle.y,
		pParent->m_rt.size.x / 2.0f,
		pParent->m_rt.size.y / 2.0f);
	BuildTree(pParent->pChild[2]);
	pParent->pChild[3] = CreateNode(pParent,
		pParent->m_rt.vMin.x,
		pParent->m_rt.middle.y,
		pParent->m_rt.size.x / 2.0f,
		pParent->m_rt.size.y / 2.0f);
	BuildTree(pParent->pChild[3]);
}
bool TQuadtree::AddObject(TObject* obj)
{
	TNode* pFindNode = 
		FindNode(m_pRootNode, obj->m_rt);
	if (pFindNode != nullptr)
	{
		pFindNode->AddObject(obj);
		return true;
	}
	return false;
}
bool TQuadtree::AddDynamicObject(TObject* obj)
{
	TNode* pFindNode =
		FindNode(m_pRootNode, obj->m_rt);
	if (pFindNode != nullptr)
	{
		pFindNode->AddDynamicObject(obj);
		return true;
	}
	return false;
}
TNode* TQuadtree::FindNode(TNode* pNode, int x, int y)
{	
	do {		
		for (int iNode = 0; iNode < 4; iNode++)
		{
			if (pNode->pChild[iNode] != nullptr)
			{
				if (TCollision::RectToPoint(
					pNode->pChild[iNode]->m_rt,
					x,y))
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

TNode* TQuadtree::FindNode(TNode* pNode, TRect rt)
{
	do {
		for (int iNode = 0; iNode < 4; iNode++)
		{
			if (pNode->pChild[iNode] != nullptr)
			{
				TRect intersect;
				TCollisionType iRet = TCollision::RectToRect(
						pNode->pChild[iNode]->m_rt,
						rt);
				if( iRet > RECT_OUT)
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
void TQuadtree::PrintObjectList(TNode* pNode)
{
	if (pNode == nullptr) return;
	for (std::list<TObject*>::iterator iter = pNode->m_DynamicObjectList.begin();
		iter != pNode->m_DynamicObjectList.end();
		iter++)
	{
		TObject* pObj = *iter;
		std::cout << "[" << pNode->m_iIndex << "]" <<
			(int)pObj->m_vPos.x <<":"<< (int)pObj->m_vPos.y << " ";
	}
	std::cout << std::endl;
	for (int iNode = 0; iNode < 4; iNode++)
	{
		PrintObjectList(pNode->pChild[iNode]);
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
