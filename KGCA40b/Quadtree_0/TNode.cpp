#include "TNode.h"
bool   TNode::IsRect(int x, int y)
{
	if (m_rt.p1.x <= x && m_rt.p2.x >= x &&
		m_rt.p1.y <= y && m_rt.p2.y >= y)
	{
		return true;
	}
	return false;
}
void TNode::AddObject(int fX, int fY)
{
	m_ObjectList.push_back(new TObject(fX, fY));
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
	m_rt.p1.x = x;
	m_rt.p1.y = y;
	m_rt.p2.x = x + w;
	m_rt.p2.y = y + h;
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