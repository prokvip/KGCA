#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <tchar.h>
#include <string>
#include <time.h>
#include <ctime>
#include <vector>
//#pragma comment(lib, "legacy_stdio_definitions.lib")
//참고 Roaming::RoamingNotInitializedException C++ 예외 발생시 accessdatabaseengine_X64.exe 설치
using RECORD = std::vector<std::wstring>;

struct dbitem
{
	std::wstring name;
	std::wstring pass;
	int			level;	
	int			sex;
};

// 1개의 필드 정보
struct TColDescription
{
	SQLUSMALLINT    icol;
	SQLWCHAR		szColName[80];
	SQLSMALLINT     cchColNameMax;
	SQLSMALLINT     pcchColName;
	SQLSMALLINT     pfSqlType;
	SQLULEN			pcbColDef;
	SQLSMALLINT     pibScale;
	SQLSMALLINT     pfNullable;
	SQLWCHAR		bindData[80];
	SQLINTEGER      bindValue;
	SQLLEN			byteSize;
};

class TOdbc
{
public:
	SWORD sRet;
	SQLINTEGER cbRet;

	SQLHENV   g_hEnv;
	SQLHDBC	  g_hDbc;
	SQLHSTMT  g_hStmt;
	SQLHSTMT  g_hReadStmt;	
	SQLHSTMT  g_hDeleteStmt;

	std::vector<TColDescription>	m_ColumnList;
	std::vector<RECORD>				m_dbDataList;
	std::vector<dbitem>				m_dbList;

	/// <summary>
	/// 
	/// </summary>
	SQLHSTMT  g_hSelectAllStmt;
	TCHAR m_szSelectName[64] = { 0, };
	SQLINTEGER m_iDataLength;
	SQLLEN m_cbColumn;
	/// <summary>
	/// 
	/// </summary>
	SQLHSTMT  g_hInsertStmt;
	TCHAR	   m_szInsertName[64] = { 0, };
	TCHAR	   m_szInsertPass[64] = { 0, };
	SQLINTEGER m_iSelectLevel;	
	/// <summary>
	/// 
	/// </summary>
	SQLHSTMT   g_hUpdateStmt;
	TCHAR	   m_szUpdateName[64] = { 0, };
	TCHAR	   m_szUpdatePass[64] = { 0, };
	SQLINTEGER m_iUpdateLevel;
	SQLINTEGER m_iUpdateSex;
	TIMESTAMP_STRUCT m_ts;

	/// <summary>
	/// 
	/// </summary>
	SQLHSTMT  g_hPassStmt;
	TCHAR	  m_szOutPass[64] = { 0, };

public:
	void Init();
	
	void ErrorMsg(SQLHSTMT  stmt);
	void Connect(std::wstring dbName);
	void ConnectMsSql(std::wstring dbName);
	void DisConnect();
	bool AddSQL(dbitem& record);
	bool UpdateSQL(dbitem& record, std::wstring selectName);
	bool ReadRecord(const TCHAR* szName);
	bool DeleteAccount(std::wstring szName);
	bool UserPass(std::wstring szName);
public:
	bool CreatePrepare();
	bool CreateAlluserinfo();
	bool CreateUserinfo();
	bool CreatePassOut();
	bool CreateUpdate();
	bool CreateInsertAccount();
	bool CreateDeleteAccount();
public:
	int    retID;					SQLLEN  lID;
	TCHAR  retName[25] = { 0, };	SQLLEN  lName;
	TCHAR  retPass[25] = { 0, };	SQLLEN  lPass;
	int    retLevel;				SQLLEN  lLevel;
	int    retSex;					SQLLEN  lSex;
	TIMESTAMP_STRUCT accountTS;		SQLLEN  lAccount;
	TIMESTAMP_STRUCT loginTS;		SQLLEN  llogin;
	TIMESTAMP_STRUCT logoutTS;		SQLLEN  llogout;
};

