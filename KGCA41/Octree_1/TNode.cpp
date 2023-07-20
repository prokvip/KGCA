#include "TNode.h"
TNode::TNode(TNode* pParent,
    float x,
    float y,
    float fWidth,
    float fHeight) : m_pParent(pParent)
{
    m_rt.Set(x, y, fWidth, fHeight);
    SetParent(pParent);
}