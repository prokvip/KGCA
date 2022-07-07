#include "TWorld.h"
#include "TObjectMgr.h"
TWorld* TWorld::m_pWorld = nullptr;

//bool	TWorld::WorldChange()
//{
//	I_ObjectMgr.Release();
//	//m_ZoneWorld.m_pd3dDevice = m_pd3dDevice;
//	//m_ZoneWorld.m_pContext = m_pImmediateContext;
//	m_ZoneWorld.Load(L"zone.txt");
//	m_pWorld = &m_ZoneWorld;
//}
bool	TWorld::Load(std::wstring saveFile)
{
	return true;
}
bool	TWorld::Init()
{
	return true;
}
bool	TWorld::Frame()
{
	for (auto obj : m_UIObj)
	{
		TObject2D* pObj = obj.get();
		if (pObj != nullptr)
		{
			pObj->Frame();
		}
	}
	for (auto obj : m_NpcObj)
	{
		TObject2D* pObj = obj.get();
		if (pObj != nullptr)
		{
			pObj->Frame();
		}
	}
	return true;
}
bool	TWorld::Render()
{
	for (auto obj : m_UIObj)
	{
		TObject2D* pObj = obj.get();
		if (pObj != nullptr)
		{
			pObj->Render();
		}
	}
	for (auto obj : m_NpcObj)
	{
		TObject2D* pObj = obj.get();
		if (pObj->m_bDead == false)
		{
			pObj->Render();
		}
	}
	return true;
}
bool	TWorld::Release()
{
	for (auto obj : m_UIObj)
	{
		if (obj != nullptr)
		{
			obj->Release();
			obj = nullptr;
		}
		
	}
	for (auto obj : m_ItemObj)
	{
		if (obj != nullptr)
		{
			obj->Release();
			obj = nullptr;
		}
	}
	for (auto obj : m_NpcObj)
	{
		if (obj != nullptr)
		{
			obj->Release();
			obj = nullptr;
		}
	}
	for (auto obj : m_MapObj)
	{
		if (obj != nullptr)
		{
			obj->Release();
			obj = nullptr;
		}
	}
	m_ItemObj.clear();
	m_UIObj.clear();
	m_NpcObj.clear();
	m_MapObj.clear();
	return true;
}
TWorld::TWorld()
{

}

TWorld::~TWorld()
{
	Release();
}