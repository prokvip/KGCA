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
	TFbxImport	m_pFbxImporter;
	TFbxObj* Load(W_STR filename);
public:
	std::map<W_STR, std::shared_ptr<TFbxObj>>  m_tFbxObjList;
	TFbxObj* GetPtr(W_STR key);
};


