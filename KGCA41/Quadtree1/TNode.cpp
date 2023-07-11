#include "TNode.h"
void   TNode::SetParent(TNode* pParent)
{
    if (pParent == nullptr) return;
    m_pParent = pParent;
    m_iDepth = pParent->m_iDepth + 1;
}
//TNode::TNode(int i) : m_iValue(i)
//{
//    for (int i = 0; i < m_pChild.size(); i++)
//    {
//        m_pChild[i] = nullptr;
//    }
//}