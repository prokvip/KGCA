#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <tchar.h>
#include <string>
#include <vector>

using RECORD = std::vector<std::wstring>;

struct dbitem
{
	std::wstring name;
	int			price;
	bool		korean;
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
	std::vector<TColDescription>	m_ColumnList;
	std::vector<RECORD>				m_dbDataList;
	std::vector<dbitem>				m_dbList;
public:
	void ErrorMsg();
	void Connect();
	void DisConnect();
	bool AddSQL(dbitem& record);
	bool UpdateSQL(dbitem& record, const TCHAR* name);
	bool ReadRecord(const TCHAR* szName);
};

