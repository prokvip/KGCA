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
	SQLWCHAR		szColName[10];
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
	TCHAR m_szSelectName[10] = { 0, };
	/// <summary>
	/// 
	/// </summary>
	SQLHSTMT  g_hSelectAllStmt;
	TCHAR		m_szReadName[10] = { 0, };
	SQLINTEGER m_iDataLength;
	SQLLEN m_cbColumn;
	void		Load();
	/// <summary>
	/// 
	/// </summary>
	SQLHSTMT  g_hInsertStmt;
	TCHAR	   m_szInsertName[10] = { 0, };
	TCHAR	   m_szInsertPass[10] = { 0, };
	SQLINTEGER m_iSelectLevel;	
	/// <summary>
	/// 
	/// </summary>
	SQLHSTMT   g_hUpdateStmt;
	TCHAR	   m_szUpdateName[10] = { 0, };
	TCHAR	   m_szUpdatePass[10] = { 0, };
	SQLINTEGER m_iUpdateLevel;
	SQLINTEGER m_iUpdateSex;
	TIMESTAMP_STRUCT m_ts;

	/// <summary>
	/// 
	/// </summary>
	SQLHSTMT  g_hPassStmt;
	TCHAR	  m_szOutPass[10] = { 0, };

public:
	void Init();
	
	void ErrorMsg(SQLHSTMT  stmt);
	void Connect(std::wstring dbName);
	void ConnectMsSql(std::wstring dbName);
	void DisConnect();
	bool AddSQL(dbitem& record);
	bool UpdateSQL(dbitem& record, std::wstring selectName);
	bool ReadRecord(std::wstring selectName);
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
	TCHAR  retName[10] = { 0, };	SQLLEN  lName;
	TCHAR  retPass[10] = { 0, };	SQLLEN  lPass;
	int    retLevel;				SQLLEN  lLevel;
	int    retSex;					SQLLEN  lSex;
	TIMESTAMP_STRUCT accountTS;		SQLLEN  lAccount;
	TIMESTAMP_STRUCT loginTS;		SQLLEN  llogin;
	TIMESTAMP_STRUCT logoutTS;		SQLLEN  llogout;
};

