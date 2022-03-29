#pragma once
#include "TNode.h"
#include "TMap.h"
#include "TCamera.h"
class TQuadtree
{
public:
	TMap*	m_pMap=nullptr;
	TCamera* m_pCamera = nullptr;
	int		m_iWidth;
	int		m_iHeight;
	TNode*  m_pRootNode;
	int		m_iLeafDepth=0;
	int		m_iMaxDepth = 0;
	int		m_iNumLOD = 1;
	int		m_iLeafLOD = 1;
	static  int g_iCount;
public:
	std::list<TMapObject*> m_ObjectList;
	std::vector<TNode*> g_pDrawLeafNodes;
	std::vector<TNode*> g_pLeafNodes;
	std::queue<TNode*> g_Queue;	
	std::vector<DWORD>   m_IndexList;
	ComPtr<ID3D11Buffer> m_pIndexBuffer = nullptr;
	int					 m_iNumFace;
	int g_iValue = 0;
public:
	void		Build(int iWidth, int iHeight, int iMaxDepth);
	TNode*		CreateNode(TNode* pParent, float x, float y, float w, float h);
public:
	void		Build(TMap* pMap, int iMaxDepth);
	void		BuildTree(TNode* pParent);
	TBox		GenBoundingBox(TNode* pNode);
	TVector2	GetHeightFromNode(DWORD dwTL, DWORD dwTR, DWORD dwBL, DWORD dwBR);
	bool		AddObject(TMapObject* obj);
	bool		AddDynamicObject(TMapObject* obj);

	void		DynamicDeleteObject(TNode* pNode);
	TNode*		FindNode(TNode* pNode, TBox& box);
	bool		CheckBox(TBox& a, TBox& b);
public:
	void		SetIndexData(TNode* pNode, int iLodLevel);
	bool		CreateIndexBuffer(TNode* pNode, int iLodLevel);
	void		Update(TCamera* pCamera);
	bool		Render();
	void		RenderObject(TNode* pNode);
	void		RenderTile(TNode* pNode);
public:
	void		FindNeighborNode();
	TNode*		CheckBoxtoPoint(T::TVector3 p);
	void		GetRatio(TNode* pNode);
	int			GetLodType(TNode* pNode);
	int			UpdateIndexList(TNode* pNode, DWORD dwCurentIndex, DWORD dwNumLevel);
	int			SetLodIndexBuffer(TNode* pNode,DWORD& dwCurentIndex,
				DWORD dwA, DWORD dwB, DWORD dwC,DWORD dwType);
#ifdef _DEBUG
public:
	TBoxObj		m_BoxDebug;
	void		DrawDebugInit(ID3D11Device* pd3dDevice,
							  ID3D11DeviceContext* pContext);
	void		DrawDebugRender(TBox* pBox);
#endif
public:
	void PrintObjectList(TNode* pNode);


public:
	TQuadtree() {};
	virtual ~TQuadtree()
	{
		m_BoxDebug.Release();
		delete m_pRootNode;
	}
};
