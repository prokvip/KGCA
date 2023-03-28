#pragma once
#include "TTemplate.h"
#include "TAseObj.h"
#include "TTbsObj.h"
#include "TSkinObj.h"
#include "TBoneObj.h"

class TObjMgr : public TTemplateMap< TActor >, public TSingleton<TObjMgr>
{
private:
	friend class TSingleton<TObjMgr>;
public:

	int			Load(	ID3D11Device* pd3dDevice,
						const TCHAR* strFileName,
						const TCHAR* strShaderName, bool bThread = false);
	int			Add(TActor* pNode);
	TActor* 	GetFileTypeID(const TCHAR* pszFileName);
	void		SetMatrix(TMatrix* pWorld, TMatrix* pView, TMatrix* pProj);
private:
	TObjMgr(void);
	virtual ~TObjMgr(void);
};

static TAseObj* IsAseObjType(TActor* pModel)
{
	const std::type_info& r1 = typeid(*pModel);
	const std::type_info& r2 = typeid(TAseObj);
	TAseObj* pObj = nullptr;
	if (&r1 == &r2)
	{
		pObj = dynamic_cast<TAseObj*>(pModel);
	}	
	return pObj;
}
static TTbsObj* IsTbsObjType(TActor* pModel)
{
	const std::type_info& r1 = typeid(*pModel);
	const std::type_info& r2 = typeid(TTbsObj);
	TTbsObj* pObj = nullptr;
	if (&r1 == &r2)
	{
		pObj = dynamic_cast<TTbsObj*>(pModel);
	}
	return pObj;
}
#define I_ObjMgr TObjMgr::GetInstance()

//template<typename T, typename F>
//auto execute(T* value, F func) -> decltype(func(value)) 
//{
//	return func(value);
//}
template<typename T>
auto CreateDefaultSubobject(const TCHAR* szName) 
{
	shared_ptr<T> pNode = make_shared<T>();
	I_ObjMgr.Add(pNode);
	pNode->m_szName = szName;
	return pNode;
}

