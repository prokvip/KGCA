#include "TModelMgr.h"
TFbxObj* TModelMgr::GetPtr(W_STR key)
{
	auto iter = m_tFbxObjList.find(key);
	if (m_tFbxObjList.end() == iter)
	{
		return nullptr;
	}
	return iter->second.get();
}

TFbxObj* TModelMgr::Load(W_STR szPullfilePath)
{
	std::size_t found = szPullfilePath.find_last_of(L"/");
	std::wstring path = szPullfilePath.substr(0, found + 1);
	std::wstring key = szPullfilePath.substr(found + 1);
	TFbxObj* data = GetPtr(key);
	if (data != nullptr)
	{
		return data;
	}
	TFbxObj* ret = nullptr;
	m_pFbxImporter.Init();
	std::shared_ptr<TFbxObj> loadobj = std::make_shared<TFbxObj>();
	if (m_pFbxImporter.Load(szPullfilePath, loadobj.get()))
	{
		m_tFbxObjList.insert(std::make_pair(key, loadobj));
		 ret = loadobj.get();
	}
	m_pFbxImporter.Release();
	return ret;
}