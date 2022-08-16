#include "TNode.h"
TNode::TNode(TNode* pParent, float x, float y, float w, float h)
{
    m_rt.Set(x, y, w, h);
    m_iDepth = 0;
    m_pParent = nullptr;
    if (pParent != nullptr)
    {
        m_pParent = pParent;
        m_iDepth = pParent->m_iDepth + 1;
    }
    m_pChild.resize(4);
}
TNode::~TNode()
{
    delete m_pChild[0];
    delete m_pChild[1];
    delete m_pChild[2];
    delete m_pChild[3];
}