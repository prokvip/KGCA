#pragma once
#include "TFbxImport.h"

class TModelMgr
{
public:
	static TModelMgr& Get()
	{
		static TModelMgr mgr;
		return mgr;
	}
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;
	void Set(ID3D11Device* pDevice,ID3D11DeviceContext* pContext)
	{
		m_pDevice = pDevice;
		m_pContext = pContext;
	}
	TFbxImport	m_pFbxImporter;
	TFbxObj* Load(W_STR filename);
public:
	std::map<W_STR, std::shared_ptr<TFbxObj>>  m_tFbxObjList;
	TFbxObj* GetPtr(W_STR key);
};


