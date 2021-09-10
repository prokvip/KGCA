#include "TQuadtree.h"
bool    TQuadtree::Init(	int iWidth, 
							int iHeight)
{
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_pRootNode = new TNode(0, 0, iWidth, iHeight);	
	TNode::g_iNewCounter++;
	Buildtree(m_pRootNode);
	return true;
}

void TQuadtree::Buildtree(TNode* pNode)
{
	if (pNode->m_tRect.wh.x >= 30.0f && 
		pNode->m_tRect.wh.y >= 30.0f)
	{
		pNode->m_pChild[0] =
			new TNode(pNode->m_tRect.p0.x,
				pNode->m_tRect.p0.y,
				pNode->m_tRect.wh.x / 2.0f,
				pNode->m_tRect.wh.y / 2.0f);
		pNode->m_pChild[0]->m_iDepth = pNode->m_iDepth + 1;
		TNode::g_iNewCounter++;
		Buildtree(pNode->m_pChild[0]);

		pNode->m_pChild[1] =
			new TNode(
				pNode->m_pChild[0]->m_tRect.p1.x,
				pNode->m_pChild[0]->m_tRect.p0.y,
				pNode->m_pChild[0]->m_tRect.wh.x,
				pNode->m_pChild[0]->m_tRect.wh.y);
		pNode->m_pChild[1]->m_iDepth = pNode->m_iDepth + 1;
		TNode::g_iNewCounter++;
		Buildtree(pNode->m_pChild[1]);
		pNode->m_pChild[2] =
			new TNode(
				pNode->m_pChild[0]->m_tRect.p1.x,
				pNode->m_pChild[0]->m_tRect.p1.y,
				pNode->m_pChild[0]->m_tRect.wh.x,
				pNode->m_pChild[0]->m_tRect.wh.y);
		pNode->m_pChild[2]->m_iDepth = pNode->m_iDepth + 1;
		TNode::g_iNewCounter++;
		Buildtree(pNode->m_pChild[2]);
		pNode->m_pChild[3] =
			new TNode(
				pNode->m_pChild[0]->m_tRect.p0.x,
				pNode->m_pChild[0]->m_tRect.p1.y,
				pNode->m_pChild[0]->m_tRect.wh.x,
				pNode->m_pChild[0]->m_tRect.wh.y);
		pNode->m_pChild[3]->m_iDepth = pNode->m_iDepth + 1;
		TNode::g_iNewCounter++;
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
void TQuadtree::Release()
{
	delete m_pRootNode;
	m_pRootNode = nullptr;
}
TQuadtree::TQuadtree()
{
	m_pRootNode = nullptr;
}

TQuadtree::~TQuadtree()
{
}
