#pragma once
#include "TNode.h"

struct PolyTexture
{
	int				iTexIndex;
	T_STR	szName;
};
class TPolygonGen
{
public:
	TPolygon*				m_pPolyList;
	vector<TPolygon*>		m_pPolyArray;
	vector<PolyTexture>		m_TextureArray;
	float					m_fHeight;
	int						m_iNumPolygon;
	typedef vector <PNCT_VERTEX>			VertexList;
	typedef VertexList::iterator			VertexListItor;
public:
	bool					LoadTexture();
	// Type=0 => �ΰ��� �÷����� ����
	// Type=1 => ���� �ǿ����� ����
	TPolygon*			CreateMap(DWORD dwType = 0);
	// ��ȯ�Ǵ� ������ ����Ʈ, ������ ���� ��� ��, ����ũ��, ����ũ��, ���� 
	bool				CreateBitMap(int *pMap, int XCnt, int YCnt, float fUnit);
	void				SetBitVertex(PNCT_VERTEX* pVertex, float x, float y, float z,
		TVector3 vNormal,
		float tu, float tv,
		float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f);
	void				UnionPolygon(TPolygon* pPolygon);
	void				UnionAddPolygon(TPolygon* pSrc, TPolygon* pDesc);
	int					IsEqulVertexList(VertexList& CollectMap, PNCT_VERTEX* pTsVertex);

	int					MergePolygon(int *pMap, int iStartX, int iStartZ, int iMaxX, int iMazZ);
public:
	TPolygonGen(void);
	virtual ~TPolygonGen(void);
};

