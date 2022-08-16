#include "TOctree.h"
void    TOctree::Create(
	float fWidth,
	float fHeight,
	float fDepth)
{
	m_pRootNode = CreateNode(nullptr,
		0, 0,0, fWidth, fHeight, fDepth);
	Buildtree(m_pRootNode);
}
TNode* TOctree::CreateNode(
	TNode* pParent,
	float x, float y, float z,
	float w, float h, float d)
{
	TNode* pNode = new TNode(pParent, x, y,z, w, h, d);
    pNode->m_pChild.resize(8);
	return pNode;
}
void TOctree::Buildtree(TNode* pNode)
{
    if (pNode->m_iDepth >= 3) return;
    if (pNode == nullptr) return;

    float x, y, z, w, h, d;
    x = pNode->m_Box.x1;
    y = pNode->m_Box.y1;
    z = pNode->m_Box.z1;
    w = pNode->m_Box.w / 2.0f;
    h = pNode->m_Box.h / 2.0f;
    d = pNode->m_Box.d / 2.0f;
    pNode->m_pChild[0] = CreateNode(pNode, x, y,z, w, h,d);
    x = pNode->m_Box.x1 + w;
    y = pNode->m_Box.y1;
    pNode->m_pChild[1] = CreateNode(pNode, x, y, z, w, h, d);
    x = pNode->m_Box.x1;
    y = pNode->m_Box.y1 + h;
    pNode->m_pChild[2] = CreateNode(pNode, x, y, z, w, h, d);
    x = pNode->m_Box.x1 + w;
    y = pNode->m_Box.y1 + h;
    pNode->m_pChild[3] = CreateNode(pNode, x, y, z, w, h, d);

    x = pNode->m_Box.x1;
    y = pNode->m_Box.y1;
    z = pNode->m_Box.z1 + d;
    pNode->m_pChild[4] = CreateNode(pNode, x, y, z, w, h, d);
    x = pNode->m_Box.x1 + w;
    y = pNode->m_Box.y1;
    pNode->m_pChild[5] = CreateNode(pNode, x, y, z, w, h, d);
    x = pNode->m_Box.x1;
    y = pNode->m_Box.y1 + h;
    pNode->m_pChild[6] = CreateNode(pNode, x, y, z, w, h, d);
    x = pNode->m_Box.x1 + w;
    y = pNode->m_Box.y1 + h;
    pNode->m_pChild[7] = CreateNode(pNode, x, y, z, w, h, d);

    for (int iChild = 0; iChild < pNode->m_pChild.size(); iChild++)
    {
        Buildtree(pNode->m_pChild[iChild]);
    }
}