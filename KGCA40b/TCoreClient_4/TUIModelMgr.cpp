#include "TUIModelMgr.h"
#include "TObjectMgr.h"
bool	TUIModelComposed::Frame()
{
	TUIModelComposed* pModel = new TUIModelComposed;
	std::list< TUIModel*>::iterator iter;
	for (iter = m_Components.begin(); iter != m_Components.end();
		iter++)
	{
		(*iter)->Frame();
	}
	return true;
}
bool	TUIModelComposed::Render()
{
	TUIModelComposed* pModel = new TUIModelComposed;
	std::list< TUIModel*>::iterator iter;
	for (iter = m_Components.begin(); iter != m_Components.end();
		iter++)
	{
		(*iter)->Render();
	}
	return true;
}
bool	TUIModelComposed::Release()
{
	TUIModelComposed* pModel = new TUIModelComposed;
	std::list< TUIModel*>::iterator iter;
	for (iter = m_Components.begin(); iter != m_Components.end();
		iter++)
	{
		(*iter)->Release();
	}
	return true;
}
void TUIModelComposed::Add(TUIModel* pObj)
{
	m_Components.push_back(pObj);
}
TUIModel* TUIModelComposed::Clone()
{
	TUIModelComposed* pModel = new TUIModelComposed;
	std::list< TUIModel*>::iterator iter;
	for (iter = m_Components.begin(); iter != m_Components.end();
		iter++)
	{
		pModel->Add((*iter)->Clone());

	}
	return pModel;
};
void  TUIModelComposed::UpdateData()
{
	std::list< TUIModel*>::iterator iter;
	for (iter = m_Components.begin(); iter != m_Components.end();
		iter++)
	{
		(*iter)->UpdateData();

	}
}