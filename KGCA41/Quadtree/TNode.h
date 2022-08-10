#pragma once
#include "TObject.h"
class TNode
{
public:
	int						m_iDepth;
	bool					m_bLeaf;
	TRect					m_rt;
	std::vector<TNode*>		m_pChild;
	std::vector<TObject*>	m_pStaticObjectlist;
	std::vector<TObject*>	m_pDynamicObjectlist;// ½Ç½Ã°£
public:
	void   CreateChildNode(TNode* pParent);
public:
	TNode(TNode* pParent, TRect rt);
	virtual ~TNode();
};

