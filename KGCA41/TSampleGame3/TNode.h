#pragma once
#include "TObject3D.h"
#include "TMap.h"
class TNode
{
public:
	int						m_iDepth;
	bool					m_bLeaf;
	DWORD					m_Corner[4];
	TRect					m_rt;
	DWORD					m_dwFace;
	T_BOX					m_tBox;
	std::vector<DWORD>		m_IndexList;
	ID3D11Buffer*			m_pIndexBuffer;
	std::vector<TNode*>		m_pChild;
	std::vector<TObject3D*>	m_pStaticObjectlist;
	std::vector<TObject3D*>	m_pDynamicObjectlist;// ½Ç½Ã°£
public:
	void   CreateChildNode(TNode* pParent, TMap* pMap, DWORD dwNumCols,	DWORD dwNumRows	);
	void   CreateIndexData(TMap* pMap, DWORD dwNumRows, DWORD dNumCols);
	HRESULT   CreateIndexBuffer(TMap* pMap, DWORD dwNumRows,DWORD dNumCols);
public:
	TNode(TNode* pParent, TMap* pMap,
		DWORD dwLT, DWORD dwRT, DWORD dwRB, DWORD dwLB,
		DWORD dwRows, DWORD dwCols);
	virtual ~TNode();
};

