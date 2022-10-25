#include "TNode.h"
//  0, 1
//  2, 3
TNode::TNode(TNode* pParent, DWORD dwLT, DWORD dwRT, DWORD dwRB, DWORD dwLB )
{		
	if (pParent != nullptr)
	{
		m_iDepth = pParent->m_iDepth + 1;
	}
	m_Corner[0] = dwLT;
	m_Corner[1] = dwRT;
	m_Corner[2] = dwRB;
	m_Corner[3] = dwLB;
	m_bLeaf = false;	
	m_pChild.resize(4);
}
void   TNode::CreateChildNode(TNode* pParent)
{
	// 0  1   2   3   4
	// 5  6   7   8   9
	// 10 11  12  13  14
	// 15 16  17  18  19
	// 20  21 22  23  24
	TRect rt = pParent->m_rt;
	DWORD dwLT = pParent->m_Corner[0]; // 0
	DWORD dwRT = pParent->m_Corner[1]; // 512
	DWORD dwLB = pParent->m_Corner[2];
	DWORD dwRB = pParent->m_Corner[3];
	DWORD dwTopCenter = (dwLT + dwRT) / 2;
	DWORD dwRightCenter = (dwRB + dwRT) / 2;
	DWORD dwBottomCenter = (dwLB + dwRB) / 2;
	DWORD dwLeftCenter = (dwLB + dwLT) / 2;
	DWORD dwCenter = (dwRT + dwLT+ dwRB+dwLB ) / 4;
	m_pChild[0] = new TNode(pParent, dwLT, dwTopCenter, dwLeftCenter, dwCenter);
	m_pChild[1] = new TNode(pParent, dwTopCenter, dwRT, dwCenter, dwRightCenter);
	m_pChild[2] = new TNode(pParent, dwLeftCenter, dwCenter, dwLB, dwBottomCenter);
	m_pChild[3] = new TNode(pParent, dwCenter, dwRightCenter, dwBottomCenter, dwRB);
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