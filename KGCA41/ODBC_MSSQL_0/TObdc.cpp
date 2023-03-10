#include "TOdbc.h"
bool TOdbc::CreatePrepare()
{
	SQLRETURN ret;
	/// <summary>
	/// g_hSelectAllStmt
	/// </summary>
	/// <returns></returns>
	std::wstring sql= L"select * from ACCOUNT";
	ret = SQLAllocHandle(SQL_HANDLE_STMT, g_hDbc, &g_hSelectAllStmt);	
	ret = SQLPrepare(g_hSelectAllStmt, (SQLTCHAR*)sql.c_str(), SQL_NTS);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg();
		return false;
	}
	/// <summary>
	/// g_hReadStmt
	/// </summary>
	/// <returns></returns>
	//std::wstring sql2 = L"select NAME,PASS,GLEVEL,ACCOUNT from ACCOUNT where NAME=?";
	std::wstring sql2 = L"select * from ACCOUNT where NAME=?";
	ret = SQLAllocHandle(SQL_HANDLE_STMT, g_hDbc, &g_hReadStmt);
	ret = SQLPrepare(g_hReadStmt, (SQLTCHAR*)sql2.c_str(), SQL_NTS);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg();
		return false;
	}

	m_iDataLength = sizeof(m_szSelectName);
	m_cbColumn = SQL_NTS;

	ret = SQLBindParameter(g_hReadStmt, 1, SQL_PARAM_INPUT, SQL_UNICODE, SQL_CHAR, 
		m_iDataLength, 0, m_szSelectName, 
		m_iDataLength, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg();
		return false;
	}

	/// <summary>
	/// g_hInsertStmt
	/// </summary>
	/// <returns></returns>
	TCHAR sql3[256] = L"insert into ACCOUNT (NAME,PASS,GlEVEL) values(?,?,?)";
	ret = SQLAllocHandle(SQL_HANDLE_STMT, g_hDbc, &g_hInsertStmt);
	ret = SQLPrepare(g_hInsertStmt, (SQLTCHAR*)sql3, SQL_NTS);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg();
		return false;
	}

	m_iDataLength = sizeof(m_szInsertName);
	m_cbColumn = SQL_NTS;

	ret = SQLBindParameter(g_hInsertStmt, 1, SQL_PARAM_INPUT, SQL_UNICODE, SQL_CHAR,
		m_iDataLength, 0, m_szInsertName,
		m_iDataLength, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg();
		return false;
	}
	ret = SQLBindParameter(g_hInsertStmt, 2, SQL_PARAM_INPUT, SQL_UNICODE, SQL_CHAR,
		m_iDataLength, 0, m_szInsertPass,
		m_iDataLength, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg();
		return false;
	}
	ret = SQLBindParameter(g_hInsertStmt, 3, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER,
		0, 0, &m_iSelectLevel,
		0, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg();
		return false;
	}
	

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	TCHAR sql4[256] = L"update ACCOUNT set NAME=?,PASS=?,GLEVEL=?,LOGIN=? where NAME=?";
	ret = SQLAllocHandle(SQL_HANDLE_STMT, g_hDbc, &g_hUpdateStmt);
	ret = SQLPrepare(g_hUpdateStmt, (SQLTCHAR*)sql4, SQL_NTS);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg();
		return false;
	}

	m_iDataLength = sizeof(m_szInsertName);
	m_cbColumn = SQL_NTS;
	::ZeroMemory(&m_ts, sizeof(m_ts));

	ret = SQLBindParameter(g_hUpdateStmt, 1, SQL_PARAM_INPUT, SQL_UNICODE, SQL_CHAR,
		m_iDataLength, 0, m_szInsertName,
		m_iDataLength, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg();
		return false;
	}
	ret = SQLBindParameter(g_hUpdateStmt, 2, SQL_PARAM_INPUT, SQL_UNICODE, SQL_CHAR,
		m_iDataLength, 0, m_szInsertPass,
		m_iDataLength, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg();
		return false;
	}
	ret = SQLBindParameter(g_hUpdateStmt, 3, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER,
		0, 0, &m_iSelectLevel,
		0, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg();
		return false;
	}
	ret = SQLBindParameter(g_hUpdateStmt, 4, SQL_PARAM_INPUT, SQL_TYPE_TIMESTAMP, SQL_TYPE_TIMESTAMP,
		SQL_TIMESTAMP_LEN, 0, &m_ts,
		sizeof(TIMESTAMP_STRUCT), &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg();
		return false;
	}
	//WCHAR szTime[] = L"2023-01-01 12:33:02.123456"; //밀리초(소수점6자리)
	//ret = SQLBindParameter(g_hUpdateStmt, 4, SQL_PARAM_INPUT, SQL_TYPE_TIMESTAMP, SQL_TYPE_TIMESTAMP,
	//	sizeof(szTime), 6, szTime,
	//	0, &m_cbColumn);
	//if (ret != SQL_SUCCESS)
	//{
	//	ErrorMsg();
	//	return false;
	//}

	ret = SQLBindParameter(g_hUpdateStmt, 5, SQL_PARAM_INPUT, SQL_UNICODE, SQL_CHAR,
		m_iDataLength, 0, m_szSelectName,
		m_iDataLength, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg();
		return false;
	}
	return true;
}
void TOdbc::Init()
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
	}
}
void TOdbc::Connect(std::wstring dbName)
{
	TCHAR  inConnect[255] = { 0, };
	TCHAR  outConnect[255] = { 0, };
	TCHAR  dir[MAX_PATH] = { 0, };
	GetCurrentDirectory(MAX_PATH, dir);
	_stprintf(inConnect, _T("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};DBQ=%s;"), dbName.c_str());

	SQLSMALLINT  cbOutCon;
	SQLRETURN hr = SQLDriverConnect(g_hDbc, NULL, inConnect, _countof(inConnect),
		outConnect, _countof(outConnect),
		&cbOutCon, SQL_DRIVER_NOPROMPT);
	if (hr != SQL_SUCCESS && hr != SQL_SUCCESS_WITH_INFO)
	{
		ErrorMsg();
		return;
	}

	//명령핸들(g_hStmt)
	SQLAllocHandle(SQL_HANDLE_STMT, g_hDbc, &g_hStmt);
	CreatePrepare();
}
void TOdbc::ConnectMsSql(std::wstring dbName)
{
	TCHAR  inConnect[255] = { 0, };
	TCHAR  outConnect[255] = { 0, };
	TCHAR  dir[MAX_PATH] = { 0, };
	GetCurrentDirectory(MAX_PATH, dir);
	//_stprintf(inConnect, _T("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};DBQ=%s;"), dbName.c_str());
	/*TCHAR dsn[] = L"Driver={SQL Server};Server=shader.kr;Address=192.168.0.12,1433;Network=dbmssocn;Database=KGCATest;Uid=sa;Pwd=kgca!@34;";
	SQLSMALLINT  cbOutCon;
	SQLRETURN hr = SQLDriverConnect(g_hDbc, NULL, dsn, _countof(dsn),
		outConnect, _countof(outConnect),
		&cbOutCon, SQL_DRIVER_NOPROMPT);
	if (hr != SQL_SUCCESS && hr != SQL_SUCCESS_WITH_INFO)
	{
		ErrorMsg();
		return;
	}*/
	/*_stprintf(inConnect, _T("FileDsn=%s;"), dbName.c_str());	
	SQLSMALLINT  cbOutCon;
	SQLRETURN hr = SQLDriverConnect(g_hDbc, NULL, inConnect, _countof(inConnect),
		outConnect, _countof(outConnect),
		&cbOutCon, SQL_DRIVER_NOPROMPT);
	if (hr != SQL_SUCCESS && hr != SQL_SUCCESS_WITH_INFO)
	{
		ErrorMsg();
		return;
	}*/

	// KGCATest, sa, kgca!@34
	SQLRETURN hr = SQLConnect(g_hDbc, (SQLTCHAR*)L"KGCATest", SQL_NTS,
									  (SQLTCHAR*)L"sa", SQL_NTS, 
									  (SQLTCHAR*)L"kgca!@34", SQL_NTS);
	if (hr != SQL_SUCCESS && hr != SQL_SUCCESS_WITH_INFO)
	{
		ErrorMsg();
		return;
	}
	//명령핸들(g_hStmt)
	SQLAllocHandle(SQL_HANDLE_STMT, g_hDbc, &g_hStmt);
	CreatePrepare();
}
void TOdbc::DisConnect()
{	
	if (g_hStmt) SQLFreeHandle(SQL_HANDLE_STMT, g_hStmt);
	if (g_hSelectAllStmt) SQLFreeHandle(SQL_HANDLE_STMT, g_hSelectAllStmt);
	if (g_hInsertStmt) SQLFreeHandle(SQL_HANDLE_STMT, g_hInsertStmt);
	if (g_hUpdateStmt) SQLFreeHandle(SQL_HANDLE_STMT, g_hUpdateStmt);
	if (g_hDeleteStmt) SQLFreeHandle(SQL_HANDLE_STMT, g_hDeleteStmt);

	if (g_hDbc) SQLFreeHandle(SQL_HANDLE_DBC, g_hDbc);
	if (g_hEnv) SQLFreeHandle(SQL_HANDLE_ENV, g_hEnv);
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
	ZeroMemory(m_szInsertName, sizeof(m_szInsertName));
	CopyMemory(m_szInsertName, record.name.c_str(), record.name.size()*sizeof(TCHAR));
	ZeroMemory(m_szInsertPass, sizeof(m_szInsertPass));
	CopyMemory(m_szInsertPass, record.pass.c_str(), record.pass.size() * sizeof(TCHAR));

	m_iSelectLevel = record.level;

	SQLRETURN hr = SQLExecute(g_hInsertStmt);
	if (hr != SQL_SUCCESS)
	{
		ErrorMsg();
		return false;
	}
	if (g_hInsertStmt) SQLCloseCursor(g_hInsertStmt);
	SQLFreeStmt(g_hInsertStmt, SQL_CLOSE);
	return true;
}
bool TOdbc::UpdateSQL(dbitem& record, std::wstring selectName)
{
	ZeroMemory(m_szSelectName, sizeof(m_szSelectName));
	CopyMemory(m_szSelectName, selectName.c_str(), selectName.size() * sizeof(TCHAR));

	ZeroMemory(m_szInsertName, sizeof(m_szInsertName));
	CopyMemory(m_szInsertName, record.name.c_str(), record.name.size() * sizeof(TCHAR));

	ZeroMemory(m_szInsertPass, sizeof(m_szInsertPass));
	CopyMemory(m_szInsertPass, record.pass.c_str(), record.pass.size() * sizeof(TCHAR));

	m_iSelectLevel = record.level;

	std::time_t now = std::time(NULL); // 1970,01,01, 0시
	std::tm* ptm = std::localtime(&now);
	
	m_ts.year =ptm->tm_year + 1900;
	m_ts.month = ptm->tm_mon + 1;
	m_ts.day = ptm->tm_mday;
	m_ts.hour =  ptm->tm_hour;
	m_ts.minute = ptm->tm_min;
	m_ts.second = ptm->tm_sec;
	m_ts.fraction = 0;

	SQLRETURN hr = SQLExecute(g_hUpdateStmt);

	if (hr != SQL_SUCCESS)
	{
		ErrorMsg();
		return false;
	}
	if (g_hUpdateStmt) SQLCloseCursor(g_hUpdateStmt);
	SQLFreeStmt(g_hUpdateStmt, SQL_CLOSE);
	return true;
}
bool TOdbc::ReadRecord(const TCHAR* szName)
{
	if (szName != nullptr)
	{
		ZeroMemory(m_szSelectName, sizeof(TCHAR)*64);
		CopyMemory(m_szSelectName, szName, _tcslen(szName));

		SQLRETURN hr = SQLExecute(g_hReadStmt);

		if (hr == SQL_SUCCESS)
		{
			if (SQLFetch(g_hReadStmt) != SQL_NO_DATA)
			{
				if (g_hReadStmt) SQLCloseCursor(g_hReadStmt);
				SQLFreeStmt(g_hReadStmt, SQL_CLOSE);
				return true;
			}			
		}
	}
	ErrorMsg();
	if (g_hReadStmt) SQLCloseCursor(g_hReadStmt);
	SQLFreeStmt(g_hReadStmt, SQL_CLOSE);
	return false;
}