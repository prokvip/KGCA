#pragma once
#include "TObject2D.h"
using CollisionFunction = std::function<void(TBaseObject*, DWORD)>;
using SelectFunction = std::function<void(TBaseObject*, DWORD)>;
class TObjectMgr : public TSingleton< TObjectMgr>
{
private:
	int			m_iExcueteCollisionID;
	int			m_iExcueteSelectID;
	std::map<int, TBaseObject*>  m_ObjectList;
	std::map<int, TBaseObject*>  m_SelectList;
public:
	friend TSingleton< TObjectMgr>;
public:	
	typedef std::map<int, CollisionFunction>::iterator FuncionIterator;
	std::map<int, CollisionFunction> m_fnCollisionExecute;
	typedef std::map<int, SelectFunction>::iterator FuncionIterator;
	std::map<int, SelectFunction> m_fnSelectExecute;
public:
	void  AddCollisionExecute(TBaseObject* owner, CollisionFunction func);
	void  DeleteCollisionExecute(TBaseObject* owner);
	void  AddSelectExecute(TBaseObject* owner, CollisionFunction func);
	void  DeleteSelectExecute(TBaseObject* owner);
	bool  Init();
	bool  Frame();
	bool  Releae();
private:
	TObjectMgr() 
	{
		m_iExcueteCollisionID = 0; 
		m_iExcueteSelectID = 0;
	};
public:
	virtual ~TObjectMgr() {};
};
#define I_ObjectMgr   TObjectMgr::Get()
