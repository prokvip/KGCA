#pragma once
#include <iostream>
#include <queue>
#include <stack>
#include <vector> // 배열(재할당 가능한)
#include <list>
#include "TObject.h"
using namespace std;
struct TRect
{
	float x1, y1; // x1,y1   -    w
	float x2, y2;
	float mx, my;//   -     m
	float w, h;   //  h           x2,y2	
};
class TNode
{
public:
	TNode* m_pParent;
	TRect  m_rt;
	int m_iDepth;
	TNode* pChild[4];
	std::list<TObject*> m_ObjectList;
	void   AddObject(int x, int y);
	bool   IsRect(int x, int y);
public:
	TNode();
	TNode(float x, float y, float w, float h);
	~TNode();
};
