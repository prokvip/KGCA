#include "TRegistry.h"

HRESULT TRegistry::WriteStringRegKey(HKEY hKey, const TCHAR* strRegName,const TCHAR* strValue)
{
	if (NULL == strValue)
		return E_INVALIDARG;

	DWORD cbValue = ((DWORD)_tcslen(strValue) + 1) * sizeof(TCHAR);

	if (ERROR_SUCCESS != RegSetValueEx(hKey, strRegName, 0, REG_SZ,
		(BYTE*)strValue, cbValue))
		return E_FAIL;

	return S_OK;
}
VOID TRegistry::WritePathSettings(const TCHAR* strPath)
{
	HKEY hkey;
	HRESULT hr;
	if (ERROR_SUCCESS == RegCreateKeyEx(HKEY_CURRENT_USER, BASIS_APP_KEY,
		0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, NULL))
	{
		if (SUCCEEDED(hr=WriteStringRegKey(hkey, _T("BASIS_INFO"), strPath)))
		{
			INT KKK = 0;
		}
		RegCloseKey(hkey);
	}
}
TRegistry::TRegistry()
{
	m_hKey = NULL;
}

TRegistry::~TRegistry()
{
	Close();
}

TRegistry::operator HKEY() const
{
	return m_hKey;
}

HKEY TRegistry::Detach()
{
	HKEY hKey = m_hKey;
	m_hKey = NULL;
	return hKey;
}

void TRegistry::Attach(HKEY hKey)
{
	_ASSERT(m_hKey == NULL);
	m_hKey = hKey;
}

LONG TRegistry::DeleteSubKey(LPCTSTR lpszSubKey)
{
	_ASSERT(m_hKey != NULL);
	return RegDeleteKey(m_hKey, lpszSubKey);
}

LONG TRegistry::DeleteValue(LPCTSTR lpszValue)
{
	_ASSERT(m_hKey != NULL);
	return RegDeleteValue(m_hKey, (LPTSTR)lpszValue);
}

LONG TRegistry::Close()
{
	LONG lRes = ERROR_SUCCESS;
	if (m_hKey != NULL)
	{
		lRes = RegCloseKey(m_hKey);
		m_hKey = NULL;
	}
	return lRes;
}

LONG TRegistry::Create(HKEY hKeyParent, LPCTSTR lpszKeyName,
	LPTSTR lpszClass, DWORD dwOptions, REGSAM samDesired,
	LPSECURITY_ATTRIBUTES lpSecAttr, LPDWORD lpdwDisposition)
{
	_ASSERT(hKeyParent != NULL);
	DWORD dw;
	HKEY hKey = NULL;
	LONG lRes = RegCreateKeyEx(hKeyParent, lpszKeyName, 0,
		lpszClass, dwOptions, samDesired, lpSecAttr, &hKey, &dw);
	if (lpdwDisposition != NULL)
		*lpdwDisposition = dw;
	if (lRes == ERROR_SUCCESS)
	{
		lRes = Close();
		m_hKey = hKey;
	}
	return lRes;
}

LONG TRegistry::Open(HKEY hKeyParent, LPCTSTR lpszKeyName, REGSAM samDesired)
{
	_ASSERT(hKeyParent != NULL);
	HKEY hKey = NULL;
	LONG lRes = RegOpenKeyEx(hKeyParent, lpszKeyName, 0, samDesired, &hKey);
	if (lRes == ERROR_SUCCESS)
	{
		lRes = Close();
		_ASSERT(lRes == ERROR_SUCCESS);
		m_hKey = hKey;
	}
	return lRes;
}

LONG TRegistry::QueryValue(DWORD& dwValue, LPCTSTR lpszValueName)
{
	DWORD dwType = NULL;
	DWORD dwCount = sizeof(DWORD);
	LONG lRes = RegQueryValueEx(m_hKey, (LPTSTR)lpszValueName, NULL, &dwType,
		(LPBYTE)&dwValue, &dwCount);
	_ASSERT((lRes!=ERROR_SUCCESS) || (dwType == REG_DWORD));
	_ASSERT((lRes!=ERROR_SUCCESS) || (dwCount == sizeof(DWORD)));
	return lRes;
}

LONG TRegistry::QueryValue(LPTSTR szValue, LPCTSTR lpszValueName, DWORD* pdwCount)
{
	_ASSERT(pdwCount != NULL);
	DWORD dwType = NULL;
	LONG lRes = RegQueryValueEx(m_hKey, (LPTSTR)lpszValueName, NULL, &dwType,
		(LPBYTE)szValue, pdwCount);
	_ASSERT((lRes!=ERROR_SUCCESS) || (dwType == REG_SZ) ||
			 (dwType == REG_MULTI_SZ) || (dwType == REG_EXPAND_SZ));
	return lRes;
}

LONG WINAPI TRegistry::SetValue(HKEY hKeyParent, LPCTSTR lpszKeyName, LPCTSTR lpszValue, LPCTSTR lpszValueName)
{
	_ASSERT(lpszValue != NULL);
	TRegistry key;
	LONG lRes = key.Create(hKeyParent, lpszKeyName);
	if (lRes == ERROR_SUCCESS)
		lRes = key.SetValue(lpszValue, lpszValueName);
	return lRes;
}

LONG TRegistry::SetKeyValue(LPCTSTR lpszKeyName, LPCTSTR lpszValue, LPCTSTR lpszValueName)
{
	_ASSERT(lpszValue != NULL);
	TRegistry key;
	LONG lRes = key.Create(m_hKey, lpszKeyName);
	if (lRes == ERROR_SUCCESS)
		lRes = key.SetValue(lpszValue, lpszValueName);
	return lRes;
}

LONG TRegistry::SetValue(DWORD dwValue, LPCTSTR lpszValueName)
{
	_ASSERT(m_hKey != NULL);
	return RegSetValueEx(m_hKey, lpszValueName, NULL, REG_DWORD,
		(BYTE * const)&dwValue, sizeof(DWORD));
}

LONG TRegistry::SetValue(LPCTSTR lpszValue, LPCTSTR lpszValueName)
{
	_ASSERT(lpszValue != NULL);
	_ASSERT(m_hKey != NULL);
	return RegSetValueEx(m_hKey, lpszValueName, NULL, REG_SZ,
		(BYTE * const)lpszValue, (lstrlen(lpszValue)+1)*sizeof(TCHAR));
}

LONG TRegistry::RecurseDeleteKey(LPCTSTR lpszKey)
{
	TRegistry key;
	LONG lRes = key.Open(m_hKey, lpszKey, KEY_READ | KEY_WRITE);
	if (lRes != ERROR_SUCCESS)
		return lRes;
	FILETIME time;
	DWORD dwSize = 256;
	TCHAR szBuffer[256];
	while (RegEnumKeyEx(key.m_hKey, 0, szBuffer, &dwSize, NULL, NULL, NULL,
		&time)==ERROR_SUCCESS)
	{
		lRes = key.RecurseDeleteKey(szBuffer);
		if (lRes != ERROR_SUCCESS)
			return lRes;
		dwSize = 256;
	}
	key.Close();
	return DeleteSubKey(lpszKey);
}


bool TRegistry::SystemPathAdd(char* szPathData)
{
	const char* PATH = "Path";
	const HKEY hkey = HKEY_LOCAL_MACHINE;
	const char* sub_key_name = "System\\CurrentControlSet\\Control\\Session Manager\\Environment";
	HKEY sub_hkey;
	bool ret = true;
	do {
		LSTATUS hr = RegOpenKeyExA(hkey, sub_key_name, 0, KEY_ALL_ACCESS/*KEY_QUERY_VALUE | KEY_SET_VALUE*/, &sub_hkey);
		if ( ERROR_SUCCESS !=  hr)
		{
			break;
		}
		DWORD size;
		std::string value;
		if (RegQueryValueExA(sub_hkey, PATH, 0, NULL, NULL, &size) != ERROR_SUCCESS) {
			ret = false;
			break;
		}
		char* data = new char[size];
		RegQueryValueExA(sub_hkey, PATH, 0, NULL, (LPBYTE)data, &size);
		value.assign(data);
		delete[] data;

		std::string::size_type pos = value.find(szPathData, 0);
		while (pos != std::string::npos) {
			if ((0 == pos || (';' == value[pos - 1])) && ((pos + strlen(szPathData) == value.length()) || (';' == value[pos + strlen(szPathData)]))) {
				pos = 0;
				break;
			}
			pos = value.find(szPathData, pos + 1);
		}
		if (pos == 0 || value.length() == 0) {
			ret = false;
			break;
		}
		value = value + ';' + szPathData;
		RegOpenKeyExA(hkey, sub_key_name, 0, KEY_QUERY_VALUE | KEY_SET_VALUE, &sub_hkey);
		RegSetValueExA(sub_hkey, PATH, 0, REG_EXPAND_SZ, reinterpret_cast<BYTE const*>(value.c_str()), value.length() + 1);
		DWORD_PTR result;
		SendMessageTimeoutA(HWND_BROADCAST, WM_SETTINGCHANGE, NULL, reinterpret_cast<LPARAM>("Environment"), SMTO_NORMAL, 100U, &result);
	} while (0);
	RegCloseKey(hkey);
	return ret;
}

bool TRegistry::SystemKeyAdd(const WCHAR* szKeyName, const WCHAR* data)
{
	const WCHAR* PATH = L"Path";
	const HKEY hkey = HKEY_LOCAL_MACHINE;
	const WCHAR* sub_key_name = L"System\\CurrentControlSet\\Control\\Session Manager\\Environment";
	HKEY sub_hkey;
	bool ret = true;
	LSTATUS hr = RegOpenKeyEx(hkey, sub_key_name, 0, KEY_ALL_ACCESS/* KEY_QUERY_VALUE | KEY_SET_VALUE*/, &sub_hkey);
	if ( ERROR_SUCCESS !=  hr)
	{
		RegCloseKey(hkey);
		return false;
	}
	if (ERROR_SUCCESS != WriteStringRegKey(sub_hkey, szKeyName, data))
	{
		RegCloseKey(hkey);
		return false;
	}	
	DWORD_PTR result;
	SendMessageTimeout(HWND_BROADCAST, WM_SETTINGCHANGE, NULL, 
		reinterpret_cast<LPARAM>(L"Environment"), SMTO_NORMAL, 100U, &result);
	RegCloseKey(hkey);
	m_hKey = sub_hkey;
	return true;
}