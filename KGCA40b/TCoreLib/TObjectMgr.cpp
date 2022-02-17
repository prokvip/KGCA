#include "TObjectMgr.h"
void  TObjectMgr::AddCollisionExecute(TBaseObject* owner, CollisionFunction func)
{
	owner->m_iCollisionID = m_iExcueteCollisionID++;
	m_ObjectList.insert(std::make_pair(owner->m_iCollisionID, owner));
	m_fnCollisionExecute.insert(std::make_pair(owner->m_iCollisionID, func));
}
void  TObjectMgr::DeleteCollisionExecute(TBaseObject* owner)
{
	std::map<int, TBaseObject*>::iterator objiter;
	objiter = m_ObjectList.find(owner->m_iCollisionID);
	if (objiter != m_ObjectList.end())
	{
		m_ObjectList.erase(objiter);
	}

	FuncionIterator colliter = m_fnCollisionExecute.find(owner->m_iCollisionID);
	if (colliter != m_fnCollisionExecute.end())
	{
		m_fnCollisionExecute.erase(colliter);
	}
}
void  TObjectMgr::AddSelectExecute(TBaseObject* owner, CollisionFunction func)
{
	owner->m_iSelectID = m_iExcueteSelectID++;
	m_SelectList.insert(std::make_pair(owner->m_iSelectID, owner));
	m_fnSelectExecute.insert(std::make_pair(owner->m_iSelectID, func));
}
void  TObjectMgr::DeleteSelectExecute(TBaseObject* owner)
{
	std::map<int, TBaseObject*>::iterator objiter;
	objiter = m_SelectList.find(owner->m_iCollisionID);
	if (objiter != m_SelectList.end())
	{
		m_SelectList.erase(objiter);
	}

	FuncionIterator colliter = m_fnSelectExecute.find(owner->m_iCollisionID);
	if (colliter != m_fnSelectExecute.end())
	{
		m_fnSelectExecute.erase(colliter);
	}
}
bool  TObjectMgr::Init()
{
	return true;
}
bool  TObjectMgr::Frame()
{
	// collision
	for (auto src : m_ObjectList)
	{
		TBaseObject* pObjSrc = (TBaseObject*)src.second;
		if (pObjSrc->m_dwCollisonType == TCollisionType::Ignore) continue;
		DWORD dwState= TCollisionType::Overlap;
		for (auto dest : m_ObjectList)
		{
			TBaseObject* pObjDest = (TBaseObject*)dest.second;
			if (pObjSrc == pObjDest) continue;
			if (TCollision::ToRect(pObjSrc->m_rtCollision, pObjDest->m_rtCollision))
			{
				FuncionIterator colliter = m_fnCollisionExecute.find(pObjSrc->m_iCollisionID);
				if (colliter != m_fnCollisionExecute.end())
				{
					CollisionFunction call = colliter->second;
					call(pObjDest, dwState);
				}
			}
		}
	}

	// mouse select
	for (auto src : m_SelectList)
	{
		TBaseObject* pObjSrc = (TBaseObject*)src.second;
		if (pObjSrc->m_dwSelectType == TCollisionType::Ignore) continue;
		DWORD dwState = TCollisionType::Overlap;		
		if (TCollision::RectToPoint(
			pObjSrc->m_rtCollision, (float)g_ptMouse.x, (float)g_ptMouse.y))
		{
			FuncionIterator colliter = m_fnSelectExecute.find(pObjSrc->m_iSelectID);
			if (colliter != m_fnSelectExecute.end())
			{
				CollisionFunction call = colliter->second;
				call(pObjSrc, dwState);
			}
		}
		
	}
	return true;
}
bool  TObjectMgr::Releae()
{
	return true;
}