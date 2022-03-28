#pragma once
#include "TNode.h"
#include "TMap.h"
#include "TCamera.h"
class TQuadtree
{
public:
	TMap*	m_pMap=nullptr;
	int		m_iWidth;
	int		m_iHeight;
	int		m_iMaxDepth;
	TNode*  m_pRootNode;
	static  int g_iCount;
public:
	std::vector<TNode*> g_pDrawLeafNodes;
	std::vector<TNode*> g_pLeafNodes;
	std::queue<TNode*> g_Queue;	
	int g_iValue = 0;
public:
	void		Init(int iWidth, int iHeight, int iMaxDepth);
	TNode*		CreateNode(TNode* pParent, float x, float y, float w, float h);
public:
	void		Init(TMap* pMap, int iMaxDepth);	
	void		BuildTree(TNode* pParent);
	TBox		GenBoundingBox(TNode* pNode);
	TVector2	GetHeightFromNode(DWORD dwTL, DWORD dwTR, DWORD dwBL, DWORD dwBR);
	bool		AddObject(TObject3D* obj);
	bool		AddDynamicObject(TObject3D* obj);

	void		DynamicDeleteObject(TNode* pNode);
	TNode*		FindNode(TNode* pNode, TBox& box);
public:
	void		SetIndexData(TNode* pNode);
	bool		CreateIndexBuffer(TNode* pNode);
	void		Update(TCamera* pCamera);
	bool		Render();

public:
	void PrintObjectList(TNode* pNode);
//	void BinaryNodePrintInOrder(TNode* pNode);
//	void BinaryNodePrintPostOrder(TNode* pNode);
//	void BinaryNodePrintLevelOrder(TNode* pNode);

public:
	TQuadtree() {};
	virtual ~TQuadtree()
	{
		delete m_pRootNode;
	}
};
