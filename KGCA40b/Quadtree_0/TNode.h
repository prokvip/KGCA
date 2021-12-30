#pragma once
#include <iostream>
#include <queue>
#include <stack>
#include <vector> // �迭(���Ҵ� ������)
#include <list>
#include "TObject.h"
using namespace std;

class TNode
{
public:
	int    m_iIndex;
	TNode* m_pParent;
	TRect  m_rt;
	int m_iDepth;
	TNode* pChild[4];
	std::list<TObject*> m_ObjectList;
	std::list<TObject*> m_DynamicObjectList;
	void   AddObject(TObject* obj);
	void   AddDynamicObject(TObject* obj);
	void   DelDynamicObject(TObject* obj);
	
public:
	TNode();
	TNode(float x, float y, float w, float h);
	~TNode();
};
