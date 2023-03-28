#pragma once

#include "TActor.h"


using namespace DX;

template< class T > struct TTri
{
	T						vVertex[3];
	TVector3  vNormal;
	int						iSubIndex;
	TTri(int Index) : iSubIndex(Index) {}
	TTri() : iSubIndex(-1) {}
};
template< class T > struct DescendingTriSort
{
	bool operator()(TTri<T>& rpStart, TTri<T>& rpEnd)
	{
		return rpStart.iSubIndex < rpEnd.iSubIndex;
	}
};
template< class T > struct IsTriangleSame
{
	TTri<T>			m_Tri;
	IsTriangleSame(TTri<T> data) : m_Tri(data) {}
	bool operator()(TTri<T>& value)
	{
		return value.iSubIndex == m_Tri.iSubIndex;
	}
};

template <class T > class TData
{
public:
	int					m_iNumFace;
	vector<T>			m_VertexArray;
	vector<DWORD>		m_IndexArray;
	vector<TTri<T>>		m_TriList;
	vector<shared_ptr<TData>>		m_pSubMesh;
	vector<TData*>		m_pChildMesh;
	T*					m_pDrawVertex;
	TMatrix				m_matInverse;
public:
	int IsEqulVertexList(vector<T>&  VertexArray, T& Vertex);
	int SetUniqueBuffer(vector <TTri<T>>& TriList, int iMtrl, int iStartTri);
	TData(void)
	{
		m_pDrawVertex = NULL;
		m_iNumFace = 0;
	}
};

class TMesh : public TActor
{
public:
	int						m_iIndex;
	T_STR					m_strNodeName;
	T_STR					m_strParentName;
	OBJECTCLASSTYPE			m_ClassType;
	TMatrix				m_matXFormToWorld;
	TMatrix				m_matWorld;
	TMatrix				m_matInverse;
	TMatrix				m_matWorldTrans;
	TMatrix				m_matCalculation;
	TMatrix				m_matWorldRotate;
	TMatrix				m_matWorldScale;
	bool					m_bUsed;
	int						m_iNumFace;
	int						m_iDiffuseTex;
	T_BOX					m_BBox;
	TMesh*					m_pParent;
	vector<TMatrix>		m_pMatrixList;
	vector<shared_ptr<TMesh>>		m_pSubMesh;
	vector<TMesh*>		m_pChildMesh;
	vector<shared_ptr<TAnimTrack>>		m_pPosTrack;
	vector<shared_ptr<TAnimTrack>>		m_pRotTrack;
	vector<shared_ptr<TAnimTrack>>		m_pSclTrack;
	vector<shared_ptr<TAnimTrack>>		m_pVisTrack;
	float					m_fVisibility;
	
	
	//TBS
	int   m_iNumTrack[4];
	int   m_iMtrlRef;

	// skm
	// 텍스쳐 정보( 텍스쳐 맵 단위 )	
	int						m_iNumMtrl;
	int						m_iNumVertexs;

	int						m_iNumTex;
	int						m_iTexType[13];
	TCHAR					m_szTexName[13][30];

	ComPtr<ID3D11CommandList>      m_pd3dCommandList;
public:
	TMesh(void)
	{
		m_iNumFace = 0;
		m_iDiffuseTex = -1;
		m_iIndex = -1;
		m_pParent = 0;
		m_iNumVertexs = 0;
		m_iNumMtrl = 0;
		m_iMtrlRef = -1;
		m_bUsed = true;
		D3DXMatrixIdentity(&m_matWorld);
		D3DXMatrixIdentity(&m_matWorldTrans);
		D3DXMatrixIdentity(&m_matWorldRotate);
		D3DXMatrixIdentity(&m_matWorldScale);
		D3DXMatrixIdentity(&m_matCalculation);
		D3DXMatrixIdentity(&m_matXFormToWorld);		
	};;
	virtual ~TMesh(void) {};
};

template <class T >
int TData<T>::IsEqulVertexList(vector<T>&  VertexArray, T& Vertex)
{
	for (int iVertex = 0; iVertex < VertexArray.size(); iVertex++)
	{
		if (VertexArray[iVertex] == Vertex)
		{
			return iVertex;
		}
	}
	return -1;
}

template <class T >
int TData<T>::SetUniqueBuffer(vector <TTri<T>>& TriList, int iMtrl, int iStartTri)
{
	int iNumFaces = (int)TriList.size();
	m_VertexArray.reserve(iNumFaces * 3);
	m_IndexArray.reserve(iNumFaces * 3);

	TTri<T>  triSame(iMtrl);
	if (iMtrl >= 0)
	{
		iNumFaces = (int)(count_if(TriList.begin(), TriList.end(), IsTriangleSame<T>(triSame)));
	}

	for (int iFace = 0; iFace < iNumFaces; iFace++)
	{
		for (int iCnt = 0; iCnt < 3; iCnt++)
		{
			// 삼각형리스트 무조건 추가 + 인덱스로 1씩 증가됨.(정점버퍼만으로도 랜더가능함.)
			m_VertexArray.push_back(TriList[iStartTri + iFace].vVertex[iCnt]);
			m_IndexArray.push_back((DWORD)(m_VertexArray.size()-1));

			// 정점 최적화 :  시간이 많이 소모되기 때문에 주석 처리함. 
			/* 시작 -->
			int iPosReturn = IsEqulVertexList(m_VertexArray, TriList[iStartTri + iFace].vVertex[iCnt]);
			if (iPosReturn < 0)
			{
				m_VertexArray.push_back(TriList[iStartTri + iFace].vVertex[iCnt]);
				iPosReturn = m_VertexArray.size() - 1;
			}
			m_IndexArray.push_back(iPosReturn);
			<-- 마지막
			*/
		}
	}

	return iNumFaces;
}
