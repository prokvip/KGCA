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

	// °á°ú
	ret =SQLBindCol(g_hStmt, 1, SQL_UNICODE, Name, sizeof(Name), &lName);
	ret = SQLBindCol(g_hStmt, 2, SQL_C_ULONG, &Price, 0, &lPrice);
	ret = SQLBindCol(g_hStmt, 3, SQL_C_ULONG, &Korean, 0, &lKorean);

	TCHAR sql[MAX_PATH] = L"select * from tblCigar";
	ret = SQLExecDirect(g_hStmt, (SQLTCHAR*)&sql, SQL_NTS);

	while (SQLFetch(g_hStmt) != SQL_NO_DATA)
	{
		std::wcout << Name <<" " << Price << " " << Korean << std::endl;
	}

	SQLFreeHandle(SQL_HANDLE_STMT, g_hStmt);
	SQLDisconnect(g_hDbc);
	SQLFreeHandle(SQL_HANDLE_DBC, g_hDbc);
	SQLFreeHandle(SQL_HANDLE_ENV, g_hEnv);
}