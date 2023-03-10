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
	SQLHSTMT  g_hUpdateStmt;
	TIMESTAMP_STRUCT m_ts;

public:
	void Init();
	bool CreatePrepare();
	void ErrorMsg();
	void Connect(std::wstring dbName);
	void DisConnect();
	bool AddSQL(dbitem& record);
	bool UpdateSQL(dbitem& record, std::wstring selectName);
	bool ReadRecord(const TCHAR* szName);
};

