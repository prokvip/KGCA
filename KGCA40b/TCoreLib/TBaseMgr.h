#pragma once
#include "TStd.h"
template <class T, class S>
class TBaseMgr : public TSingleton<S>
{
public:
	friend class TSingleton<TBaseMgr>;
public:
	int		m_iIndex;
	ID3D11Device* m_pd3dDevice;
	ID3D11DeviceContext* m_pContext;
	std::map<std::wstring, std::shared_ptr<T> >  m_list;
public:
	std::wstring Splitpath(std::wstring path, std::wstring entry);
	virtual void	Set(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext=nullptr)
	{
		m_pd3dDevice = pd3dDevice;
		m_pContext = pContext;
	}
	virtual T* Load(std::wstring filename);
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
std::wstring TBaseMgr<T, S>::Splitpath(	std::wstring path, 
										std::wstring entry)
{
	TCHAR szFileName[MAX_PATH] = { 0, };
	TCHAR Dirve[MAX_PATH] = { 0, };
	TCHAR Dir[MAX_PATH] = { 0, };
	TCHAR FileName[MAX_PATH] = { 0, };
	TCHAR FileExt[MAX_PATH] = { 0, };

	std::wstring fullpathname = path;
	_tsplitpath_s(fullpathname.c_str(), Dirve, Dir, FileName, FileExt);
	std::wstring name = FileName;
	name += FileExt;
	if (entry.empty() == false)
	{
		name += entry;
	}
	return name;
}
template<class T, class S>
T* TBaseMgr<T, S>::GetPtr(std::wstring key)
{
	auto iter = m_list.find(key);
	if (iter != m_list.end())
	{
		return (*iter).second.get();
	}
	return nullptr;
}
template<class T, class S>
T* TBaseMgr<T, S>::Load(std::wstring filename)
{
	std::wstring name = Splitpath(filename,L"");
	T* pData = GetPtr(name);
	if (pData != nullptr)
	{
		return pData;
	}
	std::shared_ptr<T> pNewData = std::make_shared<T>();
	if (pNewData->Load(m_pd3dDevice, filename) == false)
	{
		return nullptr;
	}
	pNewData->m_csName = name;
	m_list.insert(make_pair(pNewData->m_csName, pNewData));
	m_iIndex++;
	return pNewData.get();
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