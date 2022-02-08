#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <iostream>
#include <tchar.h>

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
		printf("%s", buffer);
	}
}


void main()
{
	SQLHENV henv = SQL_NULL_HENV;
	SQLHDBC hdbc = SQL_NULL_HDBC;
	SQLHSTMT hstmt = SQL_NULL_HSTMT;
	SQLHSTMT hstmt1 = SQL_NULL_HSTMT;
	SQLRETURN retcode;
	retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
	retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION,
		(SQLPOINTER)SQL_OV_ODBC3_80, 0);
	retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);

	SQLSMALLINT cbCon;
	SQLWCHAR connStrbuf[1024] = { 0, };
	retcode = SQLDriverConnect(hdbc, GetDesktopWindow(),
		(SQLWCHAR*)L"Driver={SQL Server}", SQL_NTS,
		(SQLWCHAR*)connStrbuf, _countof(connStrbuf),
		&cbCon, SQL_DRIVER_PROMPT);
	retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

	SWORD sReturn = 0;
	SQLLEN cbRetParam = SQL_NTS;
	retcode = SQLBindParameter(hstmt, 1, SQL_PARAM_OUTPUT,
		SQL_C_SSHORT, SQL_INTEGER, 0, 0, &sReturn, 0, &cbRetParam);

	SQLWCHAR id[10] = L"testuser";
	retcode = SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT,
		SQL_C_WCHAR, SQL_WVARCHAR, sizeof(id), 0, id, sizeof(id), NULL);
	SQLWCHAR ps[10] = L"11111";
	retcode = SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT,
		SQL_C_WCHAR, SQL_WVARCHAR, sizeof(ps), 0, ps, sizeof(ps), NULL);

	TCHAR callsp[] = L"{?=call AccountCreate(?,?)}";
	retcode = SQLPrepare(hstmt, callsp, SQL_NTS);
	//retcode = SQLExecDirect(hstmt,callsp, SQL_NTS);
	retcode = SQLExecute(hstmt);
	if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
	{
		Error(henv, hdbc, hstmt);
		return;
	}
	/*while (SQLFetch(hstmt) != SQL_NO_DATA)
	{
	}*/
	while (SQLMoreResults(hstmt) != SQL_NO_DATA);
	SQLFreeStmt(hstmt, SQL_UNBIND);
	SQLFreeStmt(hstmt, SQL_RESET_PARAMS);
	SQLCloseCursor(hstmt);



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
	//retcode = SQLExecDirect(hstmt,callsp1, SQL_NTS);
	retcode = SQLExecute(hstmt1);
	if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
	{
		Error(henv, hdbc, hstmt);
		return;
	}
	/*while (SQLFetch(hstmt) != SQL_NO_DATA)
	{
	}*/
	while (SQLMoreResults(hstmt1) != SQL_NO_DATA);
	SQLFreeStmt(hstmt1, SQL_UNBIND);
	SQLFreeStmt(hstmt1, SQL_RESET_PARAMS);
	SQLCloseCursor(hstmt1);
	/// </summary>
	return;
}



