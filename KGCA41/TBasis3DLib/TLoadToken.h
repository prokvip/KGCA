#pragma once
#include "TParser.h"
#include <iterator>
class TLoadToken
{
public:
	T_STR		m_szDirName;
	T_STR		m_szName;
	HANDLE		m_hHandle;
	CHAR*		m_pmbStaticMeshData;
	TCHAR*		m_pwcStaticMeshData;
	vector<T_STR>	m_ListTokens;
	T_STR		m_pwcToken;
	TCHAR		m_pString[256];
	TCHAR*		m_pwcSearchData;
	T_STR		m_pwcTokenData;
	INT			m_iData;
	DWORD		m_dwTokenIndex;
	DWORD		m_dwMaxStringTable;
public:
	void			SetToken(T_STR szToken) { m_pwcToken = szToken; }
	bool			BeginToken();
	const TCHAR*	GetNextTokenString();
	const TCHAR*	GetCurrentTokenString();
	bool			LoadBuffer(const TCHAR* strFileName);
	INT				SearchTokenArray(INT NumObjType, const TCHAR** pObjectType, DWORD dwNextObjectIndex = 0);
	const TCHAR*	GetSearchString(const TCHAR* szSearchData, bool bLoop = true);
	bool			DeleteBuffer();
public:
	template <typename OutputIterator>
	void			Tokenize(const T_STR& text, const T_STR& delimiters, OutputIterator first);
public:
	TLoadToken(void);
	virtual ~TLoadToken(void);
};