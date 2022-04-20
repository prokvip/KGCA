#pragma once
#include "TStd.h"
#include <strsafe.h>

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

namespace T
{
	static void DisplayErrorBox(const WCHAR* lpszFunction)
	{
		LPVOID lpMsgBuf;
		LPVOID lpDisplayBuf;
		DWORD dw = GetLastError();

		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dw,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf,
			0, NULL);

		lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
			(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
		StringCchPrintf((LPTSTR)lpDisplayBuf,
			LocalSize(lpDisplayBuf) / sizeof(TCHAR),
			TEXT("%s failed with error %d: %s"),
			lpszFunction, dw, lpMsgBuf);
		MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

		LocalFree(lpMsgBuf);
		LocalFree(lpDisplayBuf);
	}
	static DWORD  LoadAllPath(const TCHAR* argv, std::vector<std::wstring>& list)
	{
		WIN32_FIND_DATA ffd;
		LARGE_INTEGER filesize;
		TCHAR szDir[MAX_PATH];
		size_t length_of_arg;
		HANDLE hFind = INVALID_HANDLE_VALUE;
		DWORD dwError = 0;

		StringCchLength(argv, MAX_PATH, &length_of_arg);
		if (length_of_arg > (MAX_PATH - 3))
		{
			//_tprintf(TEXT("\nDirectory path is too long.\n"));
			return (-1);
		}

		StringCchCopy(szDir, MAX_PATH, argv);
		StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

		// Find the first file in the directory.

		hFind = FindFirstFile(szDir, &ffd);

		if (INVALID_HANDLE_VALUE == hFind)
		{
			DisplayErrorBox(TEXT("FindFirstFile"));
			return dwError;
		}

		do
		{
			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				//_tprintf(TEXT("  %s   <DIR>\n"), ffd.cFileName);
			}
			else
			{
				filesize.LowPart = ffd.nFileSizeLow;
				filesize.HighPart = ffd.nFileSizeHigh;
				std::wstring path = argv;
				path += L"/";
				path += ffd.cFileName;
				list.push_back(path);
				//_tprintf(TEXT("  %s   %ld bytes\n"), ffd.cFileName, filesize.QuadPart);
			}
		} while (FindNextFile(hFind, &ffd) != 0);

		dwError = GetLastError();
		if (dwError != ERROR_NO_MORE_FILES)
		{
			DisplayErrorBox(TEXT("FindFirstFile"));
		}

		FindClose(hFind);
		return dwError;
	}
}