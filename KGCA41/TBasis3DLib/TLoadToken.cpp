#include "TLoadToken.h"
#include "TTimer.h"

template <typename OutputIterator>
void TLoadToken::Tokenize(const T_STR& text, const T_STR& delimiters, OutputIterator first)
{
	size_t start, stop, n = text.length();

	for (start = text.find_first_not_of(delimiters); 0 <= start && start < n;
	start = text.find_first_not_of(delimiters, stop + 1))
	{
		stop = text.find_first_of(delimiters, start);
		if (stop < 0 || stop > n)
		{
			stop = n;
		}
		*first++ = text.substr(start, stop - start);
	}
}

bool TLoadToken::BeginToken()
{
	m_dwTokenIndex = 0;
	m_dwMaxStringTable = 0;
	// back_inserter를 써서 내부적으로 sl.push_back()을 호출합니다. 
	T_STR		sentence;
	sentence = m_pwcTokenData;
	Tokenize(sentence, m_pwcToken, back_inserter(m_ListTokens));
	m_dwMaxStringTable = (DWORD)m_ListTokens.size();
	return true;
}
bool TLoadToken::LoadBuffer(const TCHAR* strFileName)
{
	CStopwatch stopwatch;
	TCHAR Drive[MAX_PATH];
	TCHAR Dir[MAX_PATH];
	TCHAR FName[MAX_PATH];
	TCHAR Ext[MAX_PATH];
	_tsplitpath(strFileName, Drive, Dir, FName, Ext);
	m_szDirName = Drive;
	m_szDirName += Dir;
	m_szName = FName;
	m_szName += Ext;

	LARGE_INTEGER FileSize;
	m_hHandle = CreateFile(strFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);
	if (m_hHandle != INVALID_HANDLE_VALUE)
	{
		GetFileSizeEx(m_hHandle, &FileSize);
		UINT cBytes = FileSize.LowPart;

		SAFE_NEW_ARRAY(m_pmbStaticMeshData, char, cBytes);
		if (!m_pmbStaticMeshData)
		{
			DeleteBuffer();
			return false;//E_OUTOFMEMORY;
		}

		DWORD dwBytesRead;
		if (!ReadFile(m_hHandle, m_pmbStaticMeshData, cBytes, &dwBytesRead, NULL))
		{
			DeleteBuffer();
			return false;
		}
		//SetFilePointer( m_hHandle, 0, 0, FILE_BEGIN );				
	}
	else
	{
		return false;
	}

	TCHAR* szMeshData = NULL;
	TCHAR* SearchString = NULL;
	size_t convertedChars = 0;
	SAFE_NEW_ARRAY(m_pwcStaticMeshData, TCHAR, FileSize.LowPart);
	if (mbstowcs_s(&convertedChars, m_pwcStaticMeshData, FileSize.LowPart, m_pmbStaticMeshData, _TRUNCATE) == 0)
	{
		DeleteBuffer();
		return false;
	}
	if (m_pwcStaticMeshData)
	{
		SAFE_DELETE_ARRAY(m_pmbStaticMeshData);
		m_pwcTokenData = m_pwcStaticMeshData;
		m_pwcSearchData = m_pwcStaticMeshData;
	}

	CloseHandle(m_hHandle);
	m_hHandle = 0;

	BeginToken();
	stopwatch.Output(L"TLoadToken::LoadBuffer");
	return true;
}
;
const TCHAR* TLoadToken::GetSearchString(const TCHAR* szSearchData, bool bLoop)
{
	const TCHAR* szSearchString = NULL;
	GetNextTokenString();
	if (!bLoop)
	{
		szSearchString = _tcsstr(m_pwcTokenData.c_str(), szSearchData);
		if (szSearchString == NULL)
		{
			--m_dwTokenIndex;
		}
	}
	else
	{
		szSearchString = _tcsstr(m_pwcTokenData.c_str(), szSearchData);
		while (szSearchString == NULL)
		{
			GetNextTokenString();
			szSearchString = _tcsstr(m_pwcTokenData.c_str(), szSearchData);
			if (_tcsstr(m_pwcTokenData.c_str(), szSearchData) != NULL)
			{
				break;
			}
		}
	}
	return szSearchString;
}
const TCHAR* TLoadToken::GetNextTokenString()
{
	if (m_dwTokenIndex + 1 >= m_dwMaxStringTable)
	{
		return NULL;
	}
	m_pwcTokenData = m_ListTokens[++m_dwTokenIndex].c_str();
	return m_pwcTokenData.c_str();
}
const TCHAR* TLoadToken::GetCurrentTokenString()
{
	return m_pwcTokenData.c_str();
}

// 버퍼스트링안에서 선단에 있는 오브젝트를 찾아 해당 인덱스를 넘겨준다.
INT TLoadToken::SearchTokenArray(INT NumObjType, const TCHAR** pObjectType, DWORD dwNextObjectIndex)
{
	INT iIndex = -1;
	bool IsLineCheck = true;

	while (IsLineCheck)
	{
		if (dwNextObjectIndex > 0 && dwNextObjectIndex - 1 <= m_dwTokenIndex)
		{
			break;
		}

		if (GetNextTokenString() == NULL) break;
		for (INT iType = 0; iType < NumObjType; iType++)
		{
			if (_tcsstr(m_pwcTokenData.c_str(), pObjectType[iType]) != NULL)
			{
				iIndex = iType;
				IsLineCheck = false;
				break;
			}
		}

	}
	return iIndex;
}
bool TLoadToken::DeleteBuffer()
{
	CloseHandle(m_hHandle);
	m_ListTokens.clear();
	SAFE_DELETE_ARRAY(m_pwcStaticMeshData);
	SAFE_DELETE_ARRAY(m_pmbStaticMeshData);
	return true;
}
TLoadToken::TLoadToken(void)
{
	ZeroMemory(m_pString, sizeof(TCHAR) * 256);
	m_pmbStaticMeshData = 0;
	m_pwcStaticMeshData = 0;
	m_pwcToken = _T("*");
	m_hHandle = 0;
}

TLoadToken::~TLoadToken(void)
{
}