#pragma once
#include "TObject2D.h"
#include "TFbxImporter.h"
using CollisionFunction = std::function<void(TBaseObject*, DWORD)>;
using SelectFunction = std::function<void(TBaseObject*, DWORD)>;

class TObjectMgr : public TBaseMgr<TFbxImporter, TObjectMgr>
{
	friend class TSingleton<TTextureMgr>;	
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
	bool  Release();
	void  CallRecursive(TBaseObject* pSrcObj, DWORD dwState);
private:
	TObjectMgr();
public:
	virtual ~TObjectMgr();
};
#define I_ObjectMgr   TObjectMgr::Get()
