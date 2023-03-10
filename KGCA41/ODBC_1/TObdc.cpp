#include "TOdbc.h"
void TOdbc::Connect()
{
	// 환경핸들(g_hEnv), 연결핸들(g_hDbc), 명령핸들(g_hStmt)
	SQLRETURN hr = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &g_hEnv);
	if (hr != SQL_SUCCESS)
	{
		return;
	}
	if (SQLSetEnvAttr(g_hEnv, SQL_ATTR_ODBC_VERSION,
		(SQLPOINTER)SQL_OV_ODBC3_80,
		SQL_IS_INTEGER) != SQL_SUCCESS)
	{
		ErrorMsg();
		return;
	}

	if (hr == SQL_SUCCESS)
	{
		//연결핸들(g_hDbc)
		hr = SQLAllocHandle(SQL_HANDLE_DBC, g_hEnv, &g_hDbc);
		if (hr != SQL_SUCCESS)
		{
			ErrorMsg();
			return;
		}


		TCHAR  inConnect[255] = { 0, };
		TCHAR  outConnect[255] = { 0, };
		TCHAR  dir[MAX_PATH] = { 0, };
		GetCurrentDirectory(MAX_PATH, dir);
		_stprintf(inConnect, _T("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};DBQ=%s\\cigarette.accdb;"), dir);

		SQLSMALLINT  cbOutCon;
		hr = SQLDriverConnect(g_hDbc, NULL, inConnect, _countof(inConnect),
			outConnect, _countof(outConnect),
			&cbOutCon, SQL_DRIVER_NOPROMPT);
		if (hr != SQL_SUCCESS && hr != SQL_SUCCESS_WITH_INFO)
		{
			ErrorMsg();
			return;
		}

		//명령핸들(g_hStmt)
		SQLAllocHandle(SQL_HANDLE_STMT, g_hDbc, &g_hStmt);
	}
}
void TOdbc::DisConnect()
{
	if (g_hEnv) SQLFreeHandle(SQL_HANDLE_STMT, g_hEnv);
	if (g_hDbc) SQLFreeHandle(SQL_HANDLE_DBC, g_hDbc);
	if (g_hStmt) SQLFreeHandle(SQL_HANDLE_ENV, g_hStmt);
}

void TOdbc::ErrorMsg()
{
	int value = -1;
	SQLTCHAR sqlState[10] = { 0, };
	SQLTCHAR msg[SQL_MAX_MESSAGE_LENGTH + 1] = { 0, };
	SQLTCHAR errorMsg[SQL_MAX_MESSAGE_LENGTH + 1] = { 0, };
	SQLSMALLINT msgLen;
	SQLINTEGER nativeError = 0;

	SQLRETURN hr;
	// 복합에러
	/*while (hr = SQLGetDiagRec(SQL_HANDLE_STMT, g_hStmt, value, sqlState, &nativeError, msg,
		_countof(msg), &msgLen) != SQL_NO_DATA)*/
		//단순에러
	SQLError(g_hEnv, g_hDbc, g_hStmt,
		sqlState, &nativeError, msg, SQL_MAX_MESSAGE_LENGTH + 1, &msgLen);
	{
		_stprintf(errorMsg, L"SQLSTATE:%s, 진단정보:%s, 에러코드:%d",
			sqlState, msg, nativeError);
		::MessageBox(NULL, errorMsg, L"진단정보", 0);
	}
}
bool TOdbc::AddSQL(dbitem& record)
{
	TCHAR sql[256] = { 0, };
	_stprintf(sql, L"insert into tblCigar (name,price,korean) values('%s',%d,%d)",
		record.name.c_str(), record.price, (int)record.korean);
	SQLRETURN hr = SQLExecDirect(g_hStmt, sql, SQL_NTS);
	if (hr != SQL_SUCCESS)
	{
		ErrorMsg();
		return false;
	}
	if (g_hStmt) SQLCloseCursor(g_hStmt);
	return true;
}
bool TOdbc::UpdateSQL(dbitem& record, const TCHAR* name)
{
	TCHAR sql[256] = { 0, };
	_stprintf(sql, L"update tblCigar set name='%s',price=%d,korean=%d where name='%s'",
		record.name.c_str(), record.price, (int)record.korean, name);
	SQLRETURN hr = SQLExecDirect(g_hStmt, sql, SQL_NTS);
	if (hr != SQL_SUCCESS)
	{
		ErrorMsg();
		return false;
	}
	if (g_hStmt) SQLCloseCursor(g_hStmt);
	return true;
}
bool TOdbc::ReadRecord(const TCHAR* szName)
{
	if (szName != nullptr)
	{
		std::wstring sqlString = L"select ";
		sqlString += m_ColumnList[0].szColName;
		for (int iData = 1; iData < m_ColumnList.size(); iData++)
		{
			sqlString += L",";
			sqlString += m_ColumnList[iData].szColName;
		}
		sqlString += L" from tblCigar where name = '%s'";

		TCHAR sql[256] = { 0, };
		//_stprintf(sql, L"select name, price, korean from tblCigar where name='%s'", szName);
		_stprintf(sql, sqlString.c_str(), szName);

		SQLRETURN hr = SQLExecDirect(g_hStmt, sql, SQL_NTS);
		if (hr != SQL_SUCCESS)
		{
			ErrorMsg();
			if (g_hStmt) SQLCloseCursor(g_hStmt);
			return false;
		}
		if (SQLFetch(g_hStmt) != SQL_NO_DATA)
		{
			if (g_hStmt) SQLCloseCursor(g_hStmt);
			return true;
		}		
		if (g_hStmt) SQLCloseCursor(g_hStmt);
	}
	return false;
}