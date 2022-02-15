#include "TObjectMgr.h"
void  TObjectMgr::AddCollisionExecute(TBaseObject* owner, CollisionFunction func)
{
	owner->m_iCollisionID = m_iExcueteCollisionID++;
	m_ObjectList.insert(std::make_pair(owner->m_iCollisionID, owner));
	m_fnCollisionExecute.insert(std::make_pair(owner->m_iCollisionID, func));
}
void  TObjectMgr::DeleteExecute(TBaseObject* owner)
{

}
bool  TObjectMgr::Init()
{
	return true;
}
bool  TObjectMgr::Frame()
{
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
	return true;
}
bool  TObjectMgr::Releae()
{
	return true;
}