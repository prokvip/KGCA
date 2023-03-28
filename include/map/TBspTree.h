#pragma once
#include "TBspParser.h"
#include "TBspRender.h"

class TBspTree : public TSpaceDivision
{
public:		
	TBspParser			m_BspParser;
	TBspRender			m_BspRender;

	ID3D11Device*		GetDevice(){ return m_BspRender.m_pd3dDevice; }

	TPolygon			*m_pCollisionPoly;	// Collision Polygon Data
	TNode				*m_pCollisionNode;	// Collision Node
	TNode				*m_pRootNode;		// BSP tree root node

	queue< TNode* >		m_QuadTreeQueue;
	vector< TNode* >	m_DrawNodeList;
	vector< TBaseObj* >	m_DrawObjList;

	TVector3			m_vCurrentPosition;

public:		
	void				SetCurrentPosition( TVector3 vPos ) { m_vCurrentPosition = vPos; };
	bool				Build(ID3D11Device*   pd3dDevice, TPolygon* pPolyList,  TCHAR*	pLoadShaderString );
	bool				Load( ID3D11Device*  pd3dDevice, TCHAR* szFileName, bool Binary = false);
	bool				Init();
	bool				Frame();			
	bool				Release();	
	bool				Render(ID3D11DeviceContext*		pContext);
	
	void				BuildBSPTree( TNode* , TPolygon*  );
	void				SetPolygon( TPolygon* pSplitter, TPolygon* pPoly,vector<PNCT_VERTEX>&	VertexList);
	void				SetMatrix( TMatrix* pWorld, TMatrix* pView, TMatrix* pProj );
	TPolygon*			SelectBestSplitter( TPolygon* pPolyList );
	T_POSITION			ClassifyPoint( TPolygon* pPlane, TVector3* pPos );
	T_POSITION			ClassifyPoly( TPolygon* pPlane, TPolygon* pPoly );
	void				SplitterPolygon(	TPolygon* pPoly, 
												TPolygon* pPlane, 
												TPolygon* pFrontSplitter,
												TPolygon* pBackSplitter );
	bool				GetIntersection(	TVector3& vStart, 	
												TVector3& vEnd , 
												TVector3& vPlaneVertex, 
												TVector3& vPlaneNormal, 
												TVector3& vIntersection,	float	&fPercentage );	

	TPolygon*			GetCollisionPoly( TPolygon* pPoly );
	void				ReleaseBsp(TNode *pBspNode);
	void				ReleaseNode(TNode *pBspNode);
	int				AddObject( TBaseObj* pObj );
	TNode*			FindNode( TNode* pNode,TBaseObj* pObj );
	int				CheckRect( TNode* pNode, TBaseObj* pObj );

	void			VisibleNode( TNode* pNode );
public:
	TBspTree(void);
	virtual ~TBspTree(void);

#ifdef _DEBUG
public:
	bool				m_bDebugBspView;
	void				SetDebugBspView( bool bView = true ) { m_bDebugBspView = bView; };
	bool				GetDebugBspView() { return m_bDebugBspView; };
#endif
};
