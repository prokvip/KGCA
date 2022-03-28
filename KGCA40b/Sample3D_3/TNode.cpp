#include "TNode.h"

void TNode::AddObject(TMapObject* obj)
{
	m_StaticObjectList.push_back(obj);
}
void TNode::AddDynamicObject(TMapObject* obj)
{
	m_DynamicObjectList.push_back(obj);
}
void TNode::DelDynamicObject(TMapObject* obj)
{
	m_DynamicObjectList.clear();
}
TNode::TNode() 
{
	m_pParent = nullptr;
	m_iDepth = 0;
	for (std::list<TMapObject*>::iterator iter = m_StaticObjectList.begin();
		iter != m_StaticObjectList.end();
		iter++)
	{
		TMapObject* pObj = *iter;
		delete pObj;
	}
	m_StaticObjectList.clear();
};

// 0, 4, 20 ,24
TNode::TNode(float x, float y, float w, float h) : TNode()
{
	m_CornerList.push_back(x);
	m_CornerList.push_back(y);
	m_CornerList.push_back(w);
	m_CornerList.push_back(h);

	m_pChild.resize(4);
	m_pChild[0] = nullptr;
	m_pChild[1] = nullptr;
	m_pChild[2] = nullptr;
	m_pChild[3] = nullptr;
}
TNode::~TNode()
{
	delete m_pChild[0];
	delete m_pChild[1];
	delete m_pChild[2];
	delete m_pChild[3];
}