#include "TWorld.h"
TWorld* TWorld::m_pWorld = nullptr;

bool	TWorld::Load(std::wstring saveFile)
{
	return true;
}
bool	TWorld::Release()
{
	for (auto obj : m_UIObj)
	{
		obj.second->Release();
		delete obj.second;
	}
	for (auto obj : m_ItemObj)
	{
		obj.second->Release();
		delete obj.second;
	}
	for (auto obj : m_NpcObj)
	{
		obj.second->Release();
		delete obj.second;
	}
	for (auto obj : m_MapObj)
	{
		obj.second->Release();
		delete obj.second;
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

}