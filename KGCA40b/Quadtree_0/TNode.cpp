#include "TNode.h"
bool   TNode::IsRect(int x, int y)
{
	if (m_rt.x1 <= x && m_rt.x2 >= x &&
		m_rt.y1 <= y && m_rt.y2 >= y)
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
	m_rt.x1 = x;
	m_rt.y1 = y;
	m_rt.x2 = x + w;
	m_rt.y2 = y + h;
	m_rt.mx = x + (w / 2.0f);
	m_rt.my = y + (h / 2.0f);
	m_rt.w = w;
	m_rt.h = h;
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