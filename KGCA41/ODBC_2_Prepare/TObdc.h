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

// 1���� �ʵ� ����
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

class TObdc
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
	SQLINTEGER m_iSelectPrice;
	SQLINTEGER m_iSelectKorean;

	/// <summary>
	/// 
	/// </summary>
	SQLHSTMT  g_hUpdateStmt;
	
public:
	void Init();
	bool CreatePrepare();
	void ErrorMsg();
	void Connect();
	void DisConnect();
	bool AddSQL(dbitem& record);
	bool UpdateSQL(dbitem& record, std::wstring selectName);
	bool ReadRecord(const TCHAR* szName);
};
