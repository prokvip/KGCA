#include "TNode.h"
//  0, 1
//  2, 3
TNode::TNode(TNode* pParent, TRect rt)
{		
	if (pParent != nullptr)
	{
		m_iDepth = pParent->m_iDepth + 1;
	}
	m_rt = rt;
	m_bLeaf = false;	
	m_pChild.resize(4);
}
void   TNode::CreateChildNode(TNode* pParent)
{
	TRect rt = pParent->m_rt;
	float halfW = rt.right / 2.0f;
	float halfH = rt.bottom / 2.0f;
	m_pChild[0] = new TNode(pParent, { rt.left, rt.top, halfW, halfH });
	m_pChild[1] = new TNode(pParent, { rt.left + halfW, rt.top, halfW, halfH });
	m_pChild[2] = new TNode(pParent, { rt.left, rt.top + halfH, halfW, halfH });
	m_pChild[3] = new TNode(pParent, { rt.left + halfW, rt.top + halfH, halfW, halfH });
}
TNode::~TNode()
{
	delete m_pChild[0];
	delete m_pChild[1];
	delete m_pChild[2];
	delete m_pChild[3];
	m_pChild[0] = nullptr;
	m_pChild[1] = nullptr;
	m_pChild[2] = nullptr;
	m_pChild[3] = nullptr;
}