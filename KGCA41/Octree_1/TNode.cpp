#include "TNode.h"
void   TNode::SetParent(TNode* pParent)
{
    if (pParent == nullptr) return;
    m_pParent = pParent;
    m_iDepth = pParent->m_iDepth + 1;
}

TNode::TNode(int id) : m_iIndex(id) {}
TNode::TNode() {}
TNode::~TNode()
{
    for (int iChild = 0; iChild < m_pChild.size(); iChild++)
    {
        delete m_pChild[iChild];
        m_pChild[iChild] = nullptr;
    }
}