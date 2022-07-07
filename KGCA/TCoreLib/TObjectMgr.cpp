#include "TObjectMgr.h"
#include "TInput.h"
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
void  TObjectMgr::CallRecursive(TBaseObject* pSrcObj,DWORD dwState)
{
	if (pSrcObj->m_pParent == nullptr)
	{		
		return;
	}
	CallRecursive(pSrcObj->m_pParent, dwState);
	pSrcObj->HitSelect(pSrcObj, dwState);
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
		//if (pObjSrc->m_dwSelectType == TSelectType::Select_Ignore) continue;
		DWORD dwState = TSelectState::T_DEFAULT;

		if (pObjSrc->m_dwSelectType != TSelectType::Select_Ignore &&
			TCollision::RectToPoint(
			pObjSrc->m_rtCollision, (float)g_ptMouse.x, (float)g_ptMouse.y))
		{
			DWORD dwKeyState = TInput::Get().m_dwMouseState[0];
			pObjSrc->m_dwSelectState = TSelectState::T_HOVER;
			if (dwKeyState == KEY_PUSH)
			{
				pObjSrc->m_dwSelectState = TSelectState::T_ACTIVE;
			}
			if (dwKeyState == KEY_HOLD)
			{
				pObjSrc->m_dwSelectState = TSelectState::T_FOCUS;
			}
			if (dwKeyState == KEY_UP)
			{
				pObjSrc->m_dwSelectState = TSelectState::T_SELECTED;
			}

			CallRecursive(pObjSrc, dwState);
			FuncionIterator colliter = m_fnSelectExecute.find(pObjSrc->m_iSelectID);
			if (colliter != m_fnSelectExecute.end())
			{			
				CollisionFunction call = colliter->second;
				call(pObjSrc, dwState);					
			}
		}
		else
		{
			if (pObjSrc->m_dwSelectState != TSelectState::T_DEFAULT)
			{
				pObjSrc->m_dwSelectState = TSelectState::T_DEFAULT;
				FuncionIterator colliter = m_fnSelectExecute.find(pObjSrc->m_iSelectID);
				if (colliter != m_fnSelectExecute.end())
				{
					CollisionFunction call = colliter->second;
					call(pObjSrc, dwState);
				}
			}
		}		
	}	
	return true;
}
bool  TObjectMgr::Release()
{
	m_ObjectList.clear();
	m_SelectList.clear();
	return true;
}

TObjectMgr::TObjectMgr()
{
	m_iExcueteCollisionID = 0;
	m_iExcueteSelectID = 0;
};
TObjectMgr::~TObjectMgr() 
{
	Release();
};
