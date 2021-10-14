#pragma once
#include "TNode.h"
#include <queue>

class TQuadtree : public TModel
{
public:
	UINT		m_iMaxDepth;
	UINT		m_iNumCell;
	UINT		m_iNumPatch;
	std::vector<TLodPatch>   m_LodPatchList;
public:
	TNode*		m_pRootNode;
	int			m_iNumCol;
	int			m_iNumRow;
	std::queue<TNode*>  m_Queue;
	std::map<int, TNode*>  m_pLeafList;
	TMap*		m_pMap;
public:
	bool	UpdateIndexList(TNode* pNode);
	HRESULT CreateIndexBuffer(TNode* pNode);
	bool	UpdateVertexList(TNode* pNode);
	HRESULT CreateVertexBuffer(TNode* pNode);
public:
	void    Build(TMap* pMap);
	bool    Init();
	TNode*  CreateNode(TNode* pParent, float x, float y, float w, float h);
	void	Buildtree(TNode*);
	bool    AddObject(TVector2 pos);
	TNode*  FindNode(TNode* pNode, TVector2 pos);
	
	TNode*  FindPlayerNode(TVector2 pos);
	bool	SubDivide(TNode* pNode);

	void	SetNeighborNode();
	bool    LoadObject(std::wstring filename);
public:
	bool	Frame();
	bool	Render(ID3D11DeviceContext* pContext, TVector3 vCamera);
	bool    Release();
	bool    ComputeStaticLodIndex(int iMaxCells);
	HRESULT CreateIndexBuffer(TLodPatch& patch, int iCode);
	template <typename OutputIterator>
	void	Tokenize(const std::wstring& text, const std::wstring& delimiters, OutputIterator first);
public:
	TQuadtree();
	virtual ~TQuadtree();
};

