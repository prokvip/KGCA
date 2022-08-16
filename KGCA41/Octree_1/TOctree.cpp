#include "TOctree.h"
void    TOctree::Create(TVector vPos, TVector vSize)
{
	m_pRootNode = CreateNode(nullptr,vPos, vSize);
	Buildtree(m_pRootNode);
}
TNode* TOctree::CreateNode(
	TNode* pParent,
	TVector vPos,
	TVector vSize)
{
	TNode* pNode = new TNode(pParent, vPos, vSize);
    pNode->m_pChild.resize(g_iMaxOctreeChild);
	return pNode;
}
void TOctree::Buildtree(TNode* pNode)
{
    if (pNode->m_iDepth >= 3) return;
    if (pNode == nullptr) return;

    TVector pos;
    TVector size;
    pos = pNode->m_Box.vMin;
    size = pNode->m_Box.vSize / 2.0f;
    pNode->m_pChild[0] = CreateNode(pNode, pos, size);
    pos.x = pNode->m_Box.vMin.x + size.x;
    pNode->m_pChild[1] = CreateNode(pNode, pos, size);
    pos.x = pNode->m_Box.vMin.x;
    pos.y = pNode->m_Box.vMin.y + size.y;
    pNode->m_pChild[2] = CreateNode(pNode, pos, size);
    pos.x = pNode->m_Box.vMin.x + size.x;
    pos.y = pNode->m_Box.vMin.y + size.y;
    pNode->m_pChild[3] = CreateNode(pNode, pos, size);

    pos.x = pNode->m_Box.vMin.x;
    pos.y = pNode->m_Box.vMin.y;
    pos.z = pNode->m_Box.vMin.z + size.z;
    pNode->m_pChild[4] = CreateNode(pNode, pos, size);
    pos.x = pNode->m_Box.vMin.x + size.x;
    pos.y = pNode->m_Box.vMin.y;
    pNode->m_pChild[5] = CreateNode(pNode, pos, size);
    pos.x = pNode->m_Box.vMin.x;
    pos.y = pNode->m_Box.vMin.y + size.y;
    pNode->m_pChild[6] = CreateNode(pNode, pos, size);
    pos.x = pNode->m_Box.vMin.x + size.x;
    pos.y = pNode->m_Box.vMin.y + size.y;
    pNode->m_pChild[7] = CreateNode(pNode, pos, size);

    for (int iChild = 0; iChild < pNode->m_pChild.size(); iChild++)
    {
        Buildtree(pNode->m_pChild[iChild]);
    }
}

bool   TOctree::IsNodeInObject(TNode* pNode, TObject* pObj)
{
    return TCollision::BoxToInBox( pNode->m_Box,
                            pObj->m_Box);
}