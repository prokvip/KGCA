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
	// Type=0 => 두개의 플랜지형 생성
	// Type=1 => 직각 실외지형 생성
	TPolygon*			CreateMap(DWORD dwType = 0);
	// 반환되는 폴리곤 리스트, 폴리곤 생성 대상 맵, 가로크기, 세로크기, 단위 
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

