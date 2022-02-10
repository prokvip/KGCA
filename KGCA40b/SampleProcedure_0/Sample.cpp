#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <iostream>
#include <tchar.h>

SQLHENV henv = SQL_NULL_HENV;
SQLHDBC hdbc = SQL_NULL_HDBC;
SQLHSTMT g_stmtAccount = SQL_NULL_HSTMT;
SQLHSTMT hstmt1 = SQL_NULL_HSTMT;
SQLRETURN retcode;

void Error(SQLHENV env, SQLHDBC dbc, SQLHSTMT stmt)
{
	SQLTCHAR buffer[SQL_MAX_MESSAGE_LENGTH + 1];
	SQLTCHAR sqlstate[SQL_SQLSTATE_SIZE + 1];
	SQLINTEGER sqlcode;
	SQLSMALLINT length;
	while (SQLError(env, dbc, stmt,
		sqlstate, &sqlcode,
		buffer, SQL_MAX_MESSAGE_LENGTH + 1,
		&length) == SQL_SUCCESS)
	{
		WCHAR szBuffer[SQL_MAX_MESSAGE_LENGTH + 1] = { 0, };
		memcpy(szBuffer, buffer, length*sizeof(WCHAR));
		std::wcout << szBuffer;
	}
}

SQLWCHAR id[10] = L"NONE";
SQLWCHAR ps[10] = L"NONE";

void CreatePrepare()
{
	retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &g_stmtAccount);
	SWORD sReturn = 0;
	SQLLEN cbRetParam = SQL_NTS;
	retcode = SQLBindParameter(g_stmtAccount, 1, SQL_PARAM_OUTPUT,
		SQL_C_SSHORT, SQL_INTEGER, 0, 0, &sReturn, 0, &cbRetParam);
	
	retcode = SQLBindParameter(g_stmtAccount, 2, SQL_PARAM_INPUT,
		SQL_C_WCHAR, SQL_WVARCHAR, sizeof(id), 0, id, sizeof(id), NULL);
	
	retcode = SQLBindParameter(g_stmtAccount, 3, SQL_PARAM_INPUT,
		SQL_C_WCHAR, SQL_WVARCHAR, sizeof(ps), 0, ps, sizeof(ps), NULL);

	TCHAR callsp[] = L"{?=call AccountCreate(?,?)}";
	retcode = SQLPrepare(g_stmtAccount, callsp, SQL_NTS);
	//retcode = SQLExecDirect(g_stmtAccount,callsp, SQL_NTS);
}
void ExecutePrepare(const TCHAR* szid, const TCHAR* szps)
{
	memcpy(id, szid, sizeof(id));
	memcpy(ps, szps, sizeof(ps));
	retcode = SQLExecute(g_stmtAccount);
	if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
	{
		Error(henv, hdbc, g_stmtAccount);
		return;
	}
	while (SQLMoreResults(g_stmtAccount) != SQL_NO_DATA);
	SQLFreeStmt(g_stmtAccount, SQL_CLOSE);
	SQLCloseCursor(g_stmtAccount);
}
void main()
{
	setlocale(LC_ALL, "korean");
	
	retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
	retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION,
		(SQLPOINTER)SQL_OV_ODBC3_80, 0);
	retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);


	SQLSMALLINT cbCon;
	SQLWCHAR connStrbuf[1024] = { 0, };
	retcode = SQLConnect(hdbc, (SQLWCHAR*)L"KGCA3d", SQL_NTS,
		(SQLWCHAR*)L"sa", SQL_NTS,
		(SQLWCHAR*)L"kgca!@34", SQL_NTS);

	/*retcode = SQLDriverConnect(hdbc, GetDesktopWindow(),
		(SQLWCHAR*)L"Driver={SQL Server}", SQL_NTS,
		(SQLWCHAR*)connStrbuf, _countof(connStrbuf),
		&cbCon, SQL_DRIVER_PROMPT);*/

	// ms sql -> odbc3_8(스트리밍) -> driver 확인
	WCHAR szVer[20] = { 0, };
	SQLSMALLINT cch = 0;
	retcode = SQLGetInfo(hdbc, SQL_DRIVER_ODBC_VER, szVer,
		_countof(szVer), &cch);
	int iOdbcMajor;
	int IOdbcMinor;
	if (SQL_SUCCEEDED(retcode))
	{

	}

	CreatePrepare();
	ExecutePrepare(L"3333", L"3333"); // g_stmtAccount	
	ExecutePrepare(L"4444", L"4444"); // 재사용


	
	/// <summary>
	/// ///
	retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt1);

	SWORD sReturn1 = 0;
	SQLLEN cbRetParam1 = SQL_NTS;
	retcode = SQLBindParameter(hstmt1, 1, SQL_PARAM_OUTPUT,
		SQL_C_SSHORT, SQL_INTEGER, 0, 0, &sReturn1, 0, &cbRetParam1);

	SQLWCHAR id1[10] = L"testuser";
	retcode = SQLBindParameter(hstmt1, 2, SQL_PARAM_INPUT,
		SQL_C_WCHAR, SQL_WVARCHAR, sizeof(id1), 0, id1, sizeof(id1), NULL);
	SQLWCHAR outps[10] = { 0, };
	retcode = SQLBindParameter(hstmt1, 3, SQL_PARAM_OUTPUT,
		SQL_C_WCHAR, SQL_WVARCHAR, sizeof(outps), 0, outps, sizeof(outps), NULL);

	TCHAR callsp1[] = L"{?=call CheckPaswordRet(?,?)}";
	retcode = SQLPrepare(hstmt1, callsp1, SQL_NTS);
	//retcode = SQLExecDirect(g_stmtAccount,callsp1, SQL_NTS);
	retcode = SQLExecute(hstmt1);
	if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
	{
		Error(henv, hdbc, g_stmtAccount);
		return;
	}
	/*while (SQLFetch(g_stmtAccount) != SQL_NO_DATA)
	{
	}*/
	while (SQLMoreResults(hstmt1) != SQL_NO_DATA);
	SQLFreeStmt(hstmt1, SQL_UNBIND);
	SQLFreeStmt(hstmt1, SQL_RESET_PARAMS);
	SQLCloseCursor(hstmt1);
	/// </summary>
	return;
}



