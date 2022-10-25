#pragma once
#include "TObject3D.h"
class TNode
{
public:
	int						m_iDepth;
	bool					m_bLeaf;
	DWORD					m_Corner[4];
	TRect					m_rt;
	std::vector<TNode*>		m_pChild;
	std::vector<TObject3D*>	m_pStaticObjectlist;
	std::vector<TObject3D*>	m_pDynamicObjectlist;// ½Ç½Ã°£
public:
	void   CreateChildNode(TNode* pParent);
public:
	TNode(TNode* pParent, DWORD dwLT, DWORD dwRT, DWORD dwRB, DWORD dwLB);
	virtual ~TNode();
};

