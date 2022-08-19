#include "TNode.h"
TNode::TNode(TNode* pParent, TVector2D p, TVector2D s)
{
    m_rt.Set(p.x, p.y, s.x, s.y);
    m_iDepth = 0;
    m_pParent = nullptr;
    if (pParent != nullptr)
    {
        m_pParent = pParent;
        m_iDepth = pParent->m_iDepth + 1;
    }    
}
TNode::TNode()
{

}
TNode::TNode(TNode* pParent, TVector vPos,TVector vSize)
{
    m_Box.Set(vPos, vSize);
    m_iDepth = 0;
    m_pParent = nullptr;
    if (pParent != nullptr)
    {
        m_pParent = pParent;
        m_iDepth = pParent->m_iDepth + 1;
    }
}
TNode::~TNode()
{
    for (int iChild = 0; iChild < m_pChild.size(); iChild++)
    {
        delete m_pChild[iChild];
    }
}