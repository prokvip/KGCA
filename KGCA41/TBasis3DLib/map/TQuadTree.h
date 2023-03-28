#pragma once
#include "TMapLOD.h"
#include <queue>

class TQuadTree : public TMapLOD
{
public:
	enum QUADTREECORNER{ tTL = 0, tTR = 1, tBL = 2, tBR = 3, };
	int				m_iRenderDepth;
	void			SetRenderDepth( DWORD dwDepth );
	TNode*					m_pRootNode;
	queue<TNode*>			m_QuadTreeQueue;
	vector<TBaseObj*>		m_DrawObjList;
	bool			m_bUsedIndexList;		// 정점 인덱스 사용하여 공간분할	
	int				m_iMaxDepthLimit;		// 노드 분할의 최대 한도
	void			SetMaxDepthLimit( int iMaxDepth ) { m_iMaxDepthLimit=iMaxDepth; }
	int				m_iMaxDepth;			// 현 리프 노드의 레벨( 깊이 ) 개수
	float			m_fWidth;
	float			m_fHeight;
	// 최소	분할 크기
	float			m_fMinDivideSize;
	void			SetMinDivideSize( int iMinDivideSize ) { m_fMinDivideSize=(float)iMinDivideSize; }
public:
	bool			Build(float fWidth, float fHeight);
	virtual bool	BuildTree( TNode* pNode);
	virtual bool	SubDivide(TNode* pNode);
	virtual bool	Release();
	virtual bool	PreFrame();
	virtual bool	Frame();	
	TNode*			CreateNode( TNode* pParentNode, float fTopLeft,float fTopRight,float fBottomLeft, float fBottomRight );
	
	int				AddObject( TBaseObj* pObj );
	TNode*			FindNode( TNode* pNode,TBaseObj* pObj );
	int				CheckRect( TNode* pNode, TBaseObj* pObj );

	void			VisibleNode( TNode* pNode );
	void			VisibleObject( TNode* pNode );
	virtual void	DrawFindNode(TNode* pNode);	

	DWORD			CheckSize(DWORD dwSize);
	void			SetNeighborNode(TNode* pNode);
	void			FindNeighborNode(TNode* pNode);
public:
	TQuadTree(void);
	virtual ~TQuadTree(void);
};
