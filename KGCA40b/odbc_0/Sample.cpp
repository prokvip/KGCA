#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <iostream>
#include <tchar.h>
#include <string>
#include <sql.h>
#include <sqlext.h>

SQLHENV		g_hEnv;
SQLHDBC		g_hDbc;
SQLHSTMT	g_hStmt;

void Check()
{
	SQLTCHAR szSQLState[SQL_SQLSTATE_SIZE + 1];
	SQLTCHAR errorBuffer[SQL_MAX_MESSAGE_LENGTH + 1];
	SQLINTEGER iSQLCode;
	SQLSMALLINT length;
	SQLError(g_hEnv, g_hDbc,
		g_hStmt,
		szSQLState,
		&iSQLCode,
		errorBuffer,
		sizeof(errorBuffer),
		&length);
	MessageBox(NULL, errorBuffer, szSQLState, MB_OK);
}
void main()
{
	setlocale(LC_ALL, "korean");
	if (SQLAllocHandle(SQL_HANDLE_ENV,	SQL_NULL_HANDLE, &g_hEnv) != SQL_SUCCESS)
	{
		return;
	}
	if (SQLSetEnvAttr(g_hEnv, SQL_ATTR_ODBC_VERSION,
		(SQLPOINTER)SQL_OV_ODBC3_80, SQL_IS_INTEGER)!= SQL_SUCCESS)
	{
		return;
	}
	if (SQLAllocHandle(SQL_HANDLE_DBC,	g_hEnv, &g_hDbc) != SQL_SUCCESS)
	{
		return;
	}
	SQLWCHAR dir[MAX_PATH] = { 0, };
	GetCurrentDirectory(MAX_PATH, dir);
	std::wstring dbpath = dir;
	dbpath += L"\\cigarette.accdb";

	TCHAR InCon[256] = { 0, };
	_stprintf(InCon, 
		_T("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};DBQ=%s;"), dbpath.c_str());
	SQLSMALLINT cbOutLen;
	SQLRETURN ret = SQLDriverConnect(	g_hDbc, NULL, InCon, _countof(InCon),
						NULL, 0, 
						&cbOutLen, SQL_DRIVER_NOPROMPT);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		Check();
		return;
	}

	if (SQLAllocHandle(SQL_HANDLE_STMT,	g_hDbc, &g_hStmt) != SQL_SUCCESS)
	{
		return;
	}

	SQLLEN lName, lPrice, lKorean;
	TCHAR Name[21];	
	int Price;
	BOOL Korean;

	// 결과
	ret =SQLBindCol(g_hStmt, 1, SQL_UNICODE, Name, sizeof(Name), &lName);
	ret = SQLBindCol(g_hStmt, 2, SQL_C_ULONG, &Price, 0, &lPrice);
	ret = SQLBindCol(g_hStmt, 3, SQL_C_ULONG, &Korean, 0, &lKorean);

	TCHAR sql[MAX_PATH] = L"select * from tblCigar";
	ret = SQLExecDirect(g_hStmt, (SQLTCHAR*)&sql, SQL_NTS);

	while (SQLFetch(g_hStmt) != SQL_NO_DATA)
	{
		std::wcout << Name <<" " << Price << " " << Korean << std::endl;
	}
	SQLCloseCursor(g_hStmt);


	ret = SQLBindCol(g_hStmt, 1, SQL_C_ULONG, &Price, 0, &lPrice);
	TCHAR sql2[MAX_PATH] = { 0, };// L"select name,price,korean from tblCigar='%s'";
	wsprintf(sql2, L"select price from tblCigar where  name='%s'", L"장미");
	ret = SQLExecDirect(g_hStmt, (SQLTCHAR*)&sql2, SQL_NTS);

	while (SQLFetch(g_hStmt) != SQL_NO_DATA)
	{
		std::wcout << Price << std::endl;
	}
	SQLCloseCursor(g_hStmt);

	//TCHAR sql3[MAX_PATH] = { 0, };// L"select name,price,korean from tblCigar='%s'";
	//wsprintf(sql3, L"insert into tblCigar (name,price, korean) values ('%s',%d,%d)",
	//				L"디스플러스", 4100, 1);
	//ret = SQLExecDirect(g_hStmt, (SQLTCHAR*)&sql3, SQL_NTS);
	//if (ret != SQL_SUCCESS )
	//{
	//	Check();
	//	return;
	//}	
	//SQLCloseCursor(g_hStmt);

	//TCHAR sql3[MAX_PATH] = { 0, };// L"select name,price,korean from tblCigar='%s'";
	//wsprintf(sql3, L"delete from tblCigar where name='%s'",
	//	L"xxxxxx");
	//ret = SQLExecDirect(g_hStmt, (SQLTCHAR*)&sql3, SQL_NTS);
	//if (ret != SQL_SUCCESS)
	//{
	//	Check();
	//	return;
	//}
	//SQLCloseCursor(g_hStmt);

	TCHAR sql4[MAX_PATH] = { 0, };// L"select name,price,korean from tblCigar='%s'";
	wsprintf(sql4, L"update tblCigar set name='%s' where name='%s'",
		L"코로나", L"88 Light");
	ret = SQLExecDirect(g_hStmt, (SQLTCHAR*)&sql4, SQL_NTS);
	if (ret != SQL_SUCCESS)
	{
		Check();
		return;
	}
	SQLCloseCursor(g_hStmt);
	SQLFreeHandle(SQL_HANDLE_STMT, g_hStmt);
	SQLDisconnect(g_hDbc);
	SQLFreeHandle(SQL_HANDLE_DBC, g_hDbc);
	SQLFreeHandle(SQL_HANDLE_ENV, g_hEnv);
}