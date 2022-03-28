#include "TNode.h"

void TNode::AddObject(TObject3D* obj)
{
	m_ObjectList.push_back(obj);
}
void TNode::AddDynamicObject(TObject3D* obj)
{
	m_DynamicObjectList.push_back(obj);
}
void TNode::DelDynamicObject(TObject3D* obj)
{
	m_DynamicObjectList.clear();
}
TNode::TNode() 
{
	m_pParent = nullptr;
	m_iDepth = 0;
	/*for ( auto obj : m_ObjectList)
	{*/
	for (std::list<TObject3D*>::iterator iter = m_ObjectList.begin();
		iter != m_ObjectList.end();
		iter++)
	{
		TObject3D* pObj = *iter;
		delete pObj;
	}
	m_ObjectList.clear();
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