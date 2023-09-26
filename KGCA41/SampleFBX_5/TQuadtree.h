#pragma once
#include "TMap.h"

class TNode
{
public:
	bool     m_bLeaf;
	DWORD    m_dwDepth;
	T_BOX    m_tBox;
	std::vector<DWORD>   m_Corner;
	TNode* m_pParent;
	std::vector<TNode*>  m_pChild;
	virtual ~TNode()
	{
		m_Corner.clear();
	}
};
class TQuadtree
{
public:
	TMap*		m_pMap;
	TNode*		m_pRootNode;
	bool   Build(TMap* map);
	TNode* CreateNode(TNode* pParent,
		DWORD dwTL, DWORD dwTR,
		DWORD dwBL, DWORD dwBR);
	void ComputeBoundingBox(TNode* pNode);
	TVector2 GetHeightFormNode(TNode* pNode);
};

