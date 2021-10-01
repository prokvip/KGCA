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

	player.Velocity.x = 10.0f;
	player.Velocity.y = 0.0f;
	player.pos += player.Velocity * time;
	TNode* pFind = FindPlayerNode(player.pos);
	if (pFind != nullptr)
	{
		cout << pFind->m_iIndex << " ";
	}
}
bool    TQuadtree::Init(	int iWidth, 
							int iHeight)
{	
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_pRootNode = CreateNode(nullptr, 0, 0, iWidth, iHeight);
	Buildtree(m_pRootNode);
	return true;
}

void TQuadtree::Buildtree(TNode* pNode)
{
	if (pNode->m_tRect.wh.x >= 30.0f && 
		pNode->m_tRect.wh.y >= 30.0f)
	{
		pNode->m_pChild[0] = CreateNode(pNode, pNode->m_tRect.p0.x,
			pNode->m_tRect.p0.y,
			pNode->m_tRect.wh.x / 2.0f,
			pNode->m_tRect.wh.y / 2.0f);		
		Buildtree(pNode->m_pChild[0]);
		
		pNode->m_pChild[1] = CreateNode(pNode, pNode->m_pChild[0]->m_tRect.p1.x,
			pNode->m_pChild[0]->m_tRect.p0.y,
			pNode->m_pChild[0]->m_tRect.wh.x,
			pNode->m_pChild[0]->m_tRect.wh.y);
		Buildtree(pNode->m_pChild[1]);

		pNode->m_pChild[2] = CreateNode(pNode, pNode->m_pChild[0]->m_tRect.p1.x,
			pNode->m_pChild[0]->m_tRect.p1.y,
			pNode->m_pChild[0]->m_tRect.wh.x,
			pNode->m_pChild[0]->m_tRect.wh.y);
		Buildtree(pNode->m_pChild[2]);
		
		pNode->m_pChild[3] = CreateNode(pNode, pNode->m_pChild[0]->m_tRect.p0.x,
			pNode->m_pChild[0]->m_tRect.p1.y,
			pNode->m_pChild[0]->m_tRect.wh.x,
			pNode->m_pChild[0]->m_tRect.wh.y);
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
	cout << pNode->m_iIndex << " ";
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
