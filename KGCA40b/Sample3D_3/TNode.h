#pragma once
#include <iostream>
#include <queue>
#include <stack>
#include <vector> // 배열(재할당 가능한)
#include <list>
#include "TObject3D.h"
using namespace std;

class TNode
{
public:
	int    m_iIndex;
	TNode* m_pParent;
	TBox   m_Box;
	int m_iDepth;
	bool   m_bLeaf = false;
	std::vector<TNode*> m_pChild;
	std::vector<int>   m_CornerList; // 정점인덱스
	std::list<TObject3D*> m_ObjectList;
	std::list<TObject3D*> m_DynamicObjectList;
	void   AddObject(TObject3D* obj);
	void   AddDynamicObject(TObject3D* obj);
	void   DelDynamicObject(TObject3D* obj);
public:
	std::vector<DWORD> m_IndexList;
	ComPtr<ID3D11Buffer> m_pIndexBuffer = nullptr;
public:
	TNode();
	TNode(float x, float y, float w, float h);
	~TNode();
};
