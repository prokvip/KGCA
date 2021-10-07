#pragma once
#include "TNode.h"
#include <queue>

class TQuadtree
{
public:
	std::vector<DWORD>  m_IndexList;
	ID3D11Buffer* m_pIndexBuffer;
	TNode*		m_pRootNode;
	int			m_iNumCol;
	int			m_iNumRow;
	std::queue<TNode*>  m_Queue;
	std::vector<TNode*>  m_pLeafList;
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

	void	SetNeighborNode(TNode* pNode);
public:
	bool	Frame();
	bool	Render(ID3D11DeviceContext* pContext);
	bool    Release();
public:
	TQuadtree();
	virtual ~TQuadtree();
};

