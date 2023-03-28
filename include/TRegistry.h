#pragma once
#include "TUtils.h"
class TRegistry : public TBASIS::TSingleton < TRegistry >
{
private:
	friend class TBASIS::TSingleton<TRegistry>;
// Attributes
public:
	operator HKEY() const;
	HKEY m_hKey;
	VOID	WritePathSettings(const TCHAR* strPath);
	HRESULT WriteStringRegKey(HKEY hKey, const TCHAR* strRegName, const TCHAR* strValue);
// Operations
public:
	LONG SetValue(DWORD dwValue, LPCTSTR lpszValueName);
	LONG QueryValue(DWORD& dwValue, LPCTSTR lpszValueName);
	LONG QueryValue(LPTSTR szValue, const LPCTSTR lpszValueName, DWORD* pdwCount);
	LONG SetValue(LPCTSTR lpszValue, LPCTSTR lpszValueName = NULL);

	LONG SetKeyValue(LPCTSTR lpszKeyName, LPCTSTR lpszValue, LPCTSTR lpszValueName = NULL);
	static LONG WINAPI SetValue(HKEY hKeyParent, LPCTSTR lpszKeyName,
		LPCTSTR lpszValue, LPCTSTR lpszValueName = NULL);

	LONG Create(HKEY hKeyParent, LPCTSTR lpszKeyName,
		LPTSTR lpszClass = REG_NONE, DWORD dwOptions = REG_OPTION_NON_VOLATILE,
		REGSAM samDesired = KEY_ALL_ACCESS,
		LPSECURITY_ATTRIBUTES lpSecAttr = NULL,
		LPDWORD lpdwDisposition = NULL);
	LONG Open(HKEY hKeyParent, LPCTSTR lpszKeyName,
		REGSAM samDesired = KEY_ALL_ACCESS);
	LONG Close();
	HKEY Detach();
	void Attach(HKEY hKey);
	LONG DeleteSubKey(LPCTSTR lpszSubKey);
	LONG RecurseDeleteKey(LPCTSTR lpszKey);
	LONG DeleteValue(LPCTSTR lpszValue);
public:
	bool SystemPathAdd(char* szPathData);
	bool SystemKeyAdd(const WCHAR* szKeyName, const WCHAR* data);
public:
	TRegistry(void);
	virtual ~TRegistry(void);
};
#define I_Reg TRegistry::GetInstance()