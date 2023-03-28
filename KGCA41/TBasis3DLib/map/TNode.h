#pragma once
#include "../TBaseObj.h"
#include "../TCamera.h"

struct NodeMtrlInfo
{
	int		iMtrl;	// 텍스쳐 인덱스
	int		iNumFaces; // 페이스 갯수
	int		iNumVertex;// 정점의 갯수
};

struct TPolygon
{
	int				iNumVertices;		// 정점 개수
	int				iNumIndices;		// 인덱스 개수	
	int				iTexIndex;			// 텍스쳐 인덱스
	bool			bBeenUsedAsSplitter;// 분할면 사용여부 판단  	
	TVector3		vNormal;			// 평면 노말	
	PNCT_VERTEX*	pVertexList;		// 정점 데이터 
	TCHAR			strTexName[30];
	DWORD			*pIndices;			// 인덱스 데이터	
	TPolygon		*pNext;				// 연결 리스트	
	virtual ~TPolygon()
	{
		SAFE_DEL(pIndices);
		SAFE_DEL(pVertexList);
	}
};

class TNode 
{	
public:
	DWORD				m_dwDepth;	
	BOOL				m_isLeaf;
	BOOL				m_isTile;
	DWORD				m_dwLodLevel;
	DWORD				m_dwLodType;
	// 바운딩 박스
	T_BOX				m_tBox;
	vector<DWORD>		m_CornerIndex;	
	// 노드의 코너 정점 인덱스 및 위치
	vector<TVector3>	m_CornerList;	
	vector<TNode*>		m_ChildList;	
	vector<TNode*>		m_NeighborList;	// 북(0)남(1)서(2)동(3)
	vector<TBaseObj*>	m_ObjectList;
	// 추가
	DWORD				m_dwPositionIndex[2];// X,Y	

	D3D11_BOX			m_BoxVB;
	D3D11_BOX			m_BoxIB;
	UINT				m_iBeginVB;
	UINT				m_iBeginIB;
	vector<DWORD>		m_IndexList;
	vector <PNCT_VERTEX>	m_VertexList;
	ComPtr<ID3D11Buffer>	m_pVertexBuffer;
	ComPtr<ID3D11Buffer>	m_pIndexBuffer;

	// bsp
	DWORD				m_dwIndex;
	vector<ComPtr<ID3D11Buffer>>	m_pVBList;
	vector<ComPtr<ID3D11Buffer>>	m_pIBList;
	int					iNumPolygon;
	int					iNumMtrl;	// 리프노드상태인 경우 사용하는 총 텍스쳐의 갯수
	vector<NodeMtrlInfo> m_MtrlList;
	TPolygon			*m_pSplitter; // Splitter poly for this node
public:
	TNode();
	virtual ~TNode();
};