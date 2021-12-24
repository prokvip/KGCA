#pragma once
#include <iostream>
#include <queue>
#include <stack>
#include <vector> // 배열(재할당 가능한)
#include <list>
#include "TObject.h"
using namespace std;

class TNode
{
public:
	TNode* m_pParent;
	TRect  m_rt;
	int m_iDepth;
	TNode* pChild[4];
	std::list<TObject*> m_ObjectList;
	void   AddObject(TObject* obj);
	
public:
	TNode();
	TNode(float x, float y, float w, float h);
	~TNode();
};
