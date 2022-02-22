#pragma once
#include "TStd.h"
template <class T, class S>
class TBaseMgr : public TSingleton<S>
{
public:
	friend class TSingleton<S>;
public:
	int		m_iIndex;
	ID3D11Device* m_pd3dDevice;
	std::map<std::wstring, T* >  m_list;
public:
	T* CheckLoad(std::wstring filename, std::wstring entry);
	virtual void	Set(ID3D11Device* pd3dDevice)
	{
		m_pd3dDevice = pd3dDevice;
	}
	T* Load(std::wstring filename);
	T* GetPtr(std::wstring key);
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
public:
	TBaseMgr();
public:
	~TBaseMgr();
};
template<class T, class S>
T* TBaseMgr<T,S>::CheckLoad(std::wstring filename, std::wstring entry)
{
	TCHAR szFileName[MAX_PATH] = { 0, };
	TCHAR Dirve[MAX_PATH] = { 0, };
	TCHAR Dir[MAX_PATH] = { 0, };
	TCHAR FileName[MAX_PATH] = { 0, };
	TCHAR FileExt[MAX_PATH] = { 0, };

	std::wstring fullpathname = filename;
	_tsplitpath_s(fullpathname.c_str(), Dirve, Dir, FileName, FileExt);
	std::wstring name = FileName;
	name += FileExt;
	if (entry.empty() == false)
	{
		name += entry;
	}
	for (auto data : m_list)
	{
		if (data.second->m_csName == name)
		{
			return data.second;
		}
	}
	return nullptr;
}
template<class T, class S>
T* TBaseMgr<T, S>::GetPtr(std::wstring key)
{
	auto iter = m_list.find(key);
	if (iter != m_list.end())
	{
		return (*iter).second;
	}
	return nullptr;
}
template<class T, class S>
T* TBaseMgr<T, S>::Load(std::wstring filename)
{
	T* pData = CheckLoad(filename, L"");
	if (pData != nullptr)
	{
		return pData;
	}
	pData = new T;
	if (pData->Load(m_pd3dDevice, filename) == false)
	{
		delete pData;
		return nullptr;
	}
	TCHAR szFileName[MAX_PATH] = { 0, };
	TCHAR Dirve[MAX_PATH] = { 0, };
	TCHAR Dir[MAX_PATH] = { 0, };
	TCHAR FileName[MAX_PATH] = { 0, };
	TCHAR FileExt[MAX_PATH] = { 0, };
	std::wstring fullpathname = filename;
	_tsplitpath_s(fullpathname.c_str(), Dirve, Dir, FileName, FileExt);
	pData->m_csName = FileName;
	pData->m_csName += FileExt;
	m_list.insert(make_pair(pData->m_csName, pData));
	m_iIndex++;
	return pData;
}
template<class T, class S>
bool	TBaseMgr<T, S>::Init()
{
	return true;
}
template<class T, class S>
bool	TBaseMgr<T, S>::Frame()
{
	return true;
}
template<class T, class S>
bool	TBaseMgr<T, S>::Render()
{
	return true;
}
template<class T, class S>
bool	TBaseMgr<T, S>::Release()
{
	for (auto data : m_list)
	{
		data.second->Release();
		delete data.second;
	}
	m_list.clear();
	return true;
}
template<class T, class S>
TBaseMgr<T, S>::TBaseMgr()
{
	m_iIndex = 0;
}
template<class T, class S>
TBaseMgr<T,S>::~TBaseMgr()
{
	Release();
}