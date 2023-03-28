#pragma once
#include "TQuadTree.h"
#include "TMap.h"
typedef vector<DWORD>	DWORD_VECTOR;
class TStaticData
{
public:
	DWORD					dwLevel;
	vector<DWORD_VECTOR>	IndexList;
	vector<ComPtr<ID3D11Buffer>>	IndexBufferList;
public:
	TStaticData();
	~TStaticData();
};

class TQuadTreeIndex : public TQuadTree
{
public:
	TMap*					m_pMap;
	PNCT_VERTEX*			m_pVertexList;
	DWORD*					m_IndexList;
	vector<TStaticData*>	m_pdwLodIndexArray;
	int						m_iNumFace;
	int						m_iMinSplitDistance;	// 최소 분할 크기( 정점간 간격 )
	DWORD					m_dwWidth;
	DWORD					m_dwHeight;
	bool					m_bDynamicUpdateIB;
	bool					m_bUsedCrossTypeIndex;
public:	
	bool					Build(	TMap* pMap,	DWORD dwWidth, DWORD dwHeight);	
	TNode*					CreateNode( TNode* pParentNode, DWORD dwTL,DWORD dwTR,DWORD dwBL, DWORD dwBR );
	int						UpdateIndexList(TNode* pNode, DWORD dwCurentIndex, DWORD dwLod  );
	TVector2				GetHeightFromNode(	DWORD dwTL, DWORD dwTR, DWORD dwBL, DWORD dwBR );
	void					ComputeBoungigBox( TNode* pNode );
	void					UpdateBoundingBox(TNode* pNode);
	bool					SubDivide(TNode* pNode);
	bool					Frame();
	bool					Release();	
	bool					Render(ID3D11DeviceContext*    pContext);
	bool					ComputeStaticLodIndex( int iSize );
	int						UpdateStaticIndexList( TNode* pNode, DWORD dwCurentIndex, DWORD dwLod  );
	DWORD					GetIndex( DWORD dwIndex, DWORD dwTL, DWORD dwTR, DWORD dwBL, DWORD dwBR, DWORD dwLod  );
	int						SetLodIndexBuffer(	TNode* pNode, 
												DWORD& dwCurentIndex, 										
												DWORD dwA, DWORD dwB, DWORD dwC,
												DWORD dwType );
	HRESULT			CreateIndexBuffer( TStaticData*  pData );
	DWORD			GetIndexCounter( DWORD dwData, DWORD dwQuadSize );
	HRESULT			CreateVertexBuffer(TNode* pNode, DWORD dwCount,PNCT_VERTEX* pvHeightMap);
	bool			CreateVertexList(TNode* pNode);
public:
	TQuadTreeIndex(void);
	virtual ~TQuadTreeIndex(void);
};
