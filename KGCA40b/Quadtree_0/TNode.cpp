#include "TNode.h"

void TNode::AddObject(TObject* obj)
{
	m_ObjectList.push_back(obj);
}
void TNode::AddDynamicObject(TObject* obj)
{
	m_DynamicObjectList.push_back(obj);
}
void TNode::DelDynamicObject(TObject* obj)
{
	m_DynamicObjectList.clear();
}
TNode::TNode() {
	m_pParent = nullptr;
	m_iDepth = 0;
	pChild[0] = nullptr;
	pChild[1] = nullptr;
	pChild[2] = nullptr;
	pChild[3] = nullptr;
	/*for ( auto obj : m_ObjectList)
	{*/
	for (std::list<TObject*>::iterator iter = m_ObjectList.begin();
		iter != m_ObjectList.end();
		iter++)
	{
		TObject* pObj = *iter;
		delete pObj;
	}
	m_ObjectList.clear();
};
TNode::TNode(float x, float y, float w, float h) : TNode()
{
	m_rt.vMin.x = x;
	m_rt.vMin.y = y;
	m_rt.vMax.x = x + w;
	m_rt.vMax.y = y + h;
	m_rt.middle.x = x + (w / 2.0f);
	m_rt.middle.y = y + (h / 2.0f);
	m_rt.size.x = w;
	m_rt.size.y = h;
	pChild[0] = nullptr;
	pChild[1] = nullptr;
	pChild[2] = nullptr;
	pChild[3] = nullptr;
}
TNode::~TNode()
{
	delete pChild[0];
	delete pChild[1];
	delete pChild[2];
	delete pChild[3];
}