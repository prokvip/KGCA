#pragma once
#include "TNode.h"

class TOctree
{
public:
	static int	m_iNodeCount;
	int		m_iSizeX;
	int		m_iSizeY;
	int		m_iSizeZ;
	int		m_iMaxDepth;
	TNode*  m_pRootNode;	
public:
	std::vector<TNode*> g_ppArray;
	//TNode** g_ppArray = nullptr;
	std::queue<TNode*> g_Queue;	
	int g_iValue = 0;
public:
	void		Init(	int iXSize, // 가로사이즈
						int iYsize, 
						int iZSize, 
						int iMaxDepth);
	TNode*		CreateNode(TNode* pParent, 
		float x, float y, float z, 
		float w, float h, float q);
	void		BuildTree(TNode* pParent);
	bool		AddObject(TObject* obj);	
	TNode*		FindNode(TNode* pNode, TBox rt);
	bool		AddDynamicObject(TObject* obj);
	void		DynamicDeleteObject(TNode* pNode);
public:
	void PrintObjectList(TNode* pNode);
//	void BinaryNodePrintInOrder(TNode* pNode);
//	void BinaryNodePrintPostOrder(TNode* pNode);
//	void BinaryNodePrintLevelOrder(TNode* pNode);
};
