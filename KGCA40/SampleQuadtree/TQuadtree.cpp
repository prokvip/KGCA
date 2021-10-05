#include "TQuadtree.h"

void	TQuadtree::Frame(float time)
{
	//player.sumForces.x = 10.0f;
	//player.sumForces.y = 0.0f;
	//// F = MA, A =  F / M
	//player.Acceleration.x = player.sumForces.x / player.mass;
	//player.Acceleration.y = player.sumForces.y / player.mass;
	//player.Velocity += player.Acceleration * time;
	//player.pos += player.Velocity * time;
	//TNode* pFind = FindPlayerNode(player.pos);
	//if (pFind != nullptr)
	//{
	//	cout << pFind->m_iIndex << " ";
	//}
}
void    TQuadtree::Build(TMap* pMap)
{
	m_pMap = pMap;
	m_iNumCol = pMap->m_info.m_iNumCol;
	m_iNumRow = pMap->m_info.m_iNumRow;
	m_pRootNode = CreateNode(nullptr, 0, m_iNumCol-1, 
		(m_iNumRow-1)* m_iNumCol, m_iNumRow * m_iNumCol -1);
	Buildtree(m_pRootNode);
}
bool    TQuadtree::Init()
{	
	
	return true;
}

void TQuadtree::Buildtree(TNode* pNode)
{
	if (pNode->m_iDepth < 3 && (pNode->m_CornerList[1]- pNode->m_CornerList[0]) > 1)
	{
		// 0           2            4
		//                         
		// 10          12           14
		// 
		// 20          22           24 		
		int center = (pNode->m_CornerList[3] + pNode->m_CornerList[0]) / 2;
		int mt = (pNode->m_CornerList[0] + pNode->m_CornerList[1]) / 2;
		int ml = (pNode->m_CornerList[2] + pNode->m_CornerList[0]) / 2;
		int mr = (pNode->m_CornerList[3] + pNode->m_CornerList[1]) / 2;
		int mb = (pNode->m_CornerList[2] + pNode->m_CornerList[3]) / 2;

		pNode->m_pChild[0] = CreateNode(pNode, pNode->m_CornerList[0],
			mt,
			ml,
			center);
		Buildtree(pNode->m_pChild[0]);
		
		pNode->m_pChild[1] = CreateNode(pNode, mt,
			pNode->m_CornerList[1],
			center,
			mr);
		Buildtree(pNode->m_pChild[1]);

		pNode->m_pChild[2] = CreateNode(pNode, ml,
			center,
			pNode->m_CornerList[2],
			mb);
		Buildtree(pNode->m_pChild[2]);
		
		pNode->m_pChild[3] = CreateNode(pNode, center,
			mr,
			mb,
			pNode->m_CornerList[3]);
		Buildtree(pNode->m_pChild[3]);		
	}
}
TNode* TQuadtree::FindNode(TNode* pNode, TVector2 pos)
{
	do {
		for (int iNode = 0; iNode < 4; iNode++)
		{
			if (pNode->m_pChild[iNode] != nullptr &&
				pNode->m_pChild[iNode]->IsRect(pos))
			{
				m_Queue.push(pNode->m_pChild[iNode]);
				break;
			}
		}
		if (m_Queue.empty())break;
		pNode = m_Queue.front();
		m_Queue.pop();
	} while (pNode);
	return pNode;
}
bool    TQuadtree::AddObject(TVector2 pos)
{
	TNode* pFindNode = FindNode(m_pRootNode, pos);
	if (pFindNode != nullptr)
	{
		pFindNode->AddObject(pos);
		return true;
	}
	return false;
}
TNode*    TQuadtree::FindPlayerNode(TVector2 pos)
{
	TNode* pFindNode = FindNode(m_pRootNode, pos);
	if (pFindNode != nullptr)
	{
		return pFindNode;
	}
	return nullptr;
}
void TQuadtree::Release()
{
	delete m_pRootNode;
	m_pRootNode = nullptr;
}
TNode* TQuadtree::CreateNode(TNode* pParent, float x, float y, float w, float h)
{
	TNode* pNode =	new TNode(x,y,w,h);
	if (pParent != nullptr)
	{
		pNode->m_iDepth = pParent->m_iDepth + 1;
		pNode->m_pParent = pParent;
	}	
	pNode->m_iIndex = TNode::g_iNewCounter;
	TNode::g_iNewCounter++;
	return pNode;
}
TQuadtree::TQuadtree()
{
	m_pRootNode = nullptr;
}

TQuadtree::~TQuadtree()
{
}
