#include "TOdbc.h"
bool TOdbc::CreateInsertAccount()
{
	SQLRETURN ret;
	std::wstring sql;
	sql = L"{?=CALL usp_InsertAccount(?,?)}";
	ret = SQLAllocHandle(SQL_HANDLE_STMT, g_hDbc, &g_hInsertStmt);
	ret = SQLPrepare(g_hInsertStmt, (SQLTCHAR*)sql.c_str(), SQL_NTS);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hInsertStmt);
		return false;
	}

	/*m_iDataLength = sizeof(m_szInsertName);
	m_cbColumn = SQL_NTS;

	ret = SQLBindParameter(g_hInsertStmt, 1, SQL_PARAM_OUTPUT, SQL_C_SHORT, SQL_SMALLINT,
		0, 0, &sRet,
		0, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hInsertStmt);
		return false;
	}

	ret = SQLBindParameter(g_hInsertStmt, 2, SQL_PARAM_INPUT, SQL_UNICODE, SQL_CHAR,
		m_iDataLength, 0, m_szInsertName,
		m_iDataLength, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hInsertStmt);
		return false;
	}
	ret = SQLBindParameter(g_hInsertStmt, 3, SQL_PARAM_INPUT, SQL_UNICODE, SQL_CHAR,
		m_iDataLength, 0, m_szInsertPass,
		m_iDataLength, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hInsertStmt);
		return false;
	}*/
	return true;
}
bool TOdbc::CreateDeleteAccount()
{
	SQLRETURN ret;
	std::wstring sql;
	sql = L"{?=CALL usp_DeleteAccount(?)}";
	ret = SQLAllocHandle(SQL_HANDLE_STMT, g_hDbc, &g_hDeleteStmt);
	ret = SQLPrepare(g_hDeleteStmt, (SQLTCHAR*)sql.c_str(), SQL_NTS);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hDeleteStmt);
		return false;
	}

	//m_iDataLength = sizeof(m_szSelectName);
	//m_cbColumn = SQL_NTS;


	//ret = SQLBindParameter(g_hDeleteStmt, 1, SQL_PARAM_OUTPUT, SQL_C_SHORT, SQL_SMALLINT,
	//	0, 0, &sRet,
	//	0, &m_cbColumn);
	//if (ret != SQL_SUCCESS)
	//{
	//	ErrorMsg(g_hDeleteStmt);
	//	return false;
	//}

	//ret = SQLBindParameter(g_hDeleteStmt, 2, SQL_PARAM_INPUT, SQL_UNICODE, SQL_CHAR,
	//	m_iDataLength, 0, m_szSelectName,
	//	m_iDataLength, &m_cbColumn);
	//if (ret != SQL_SUCCESS)
	//{
	//	ErrorMsg(g_hDeleteStmt);
	//	return false;
	//}
	return true;
}
bool TOdbc::CreateUserinfo()
{
	/// <summary>
	/// g_hReadStmt
	/// </summary>
	/// <returns></returns>
	//std::wstring sql2 = L"select NAME,PASS,GLEVEL,ACCOUNT from ACCOUNT where NAME=?";
	//std::wstring sql2 = L"select * from ACCOUNT where NAME=?";
	SQLRETURN ret;
	std::wstring sql;
	sql = L"{?=CALL usp_Userinfo(?)}";
	ret = SQLAllocHandle(SQL_HANDLE_STMT, g_hDbc, &g_hReadStmt);
	ret = SQLPrepare(g_hReadStmt, (SQLTCHAR*)sql.c_str(), SQL_NTS);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hReadStmt);
		return false;
	}

	/*m_iDataLength = sizeof(m_szReadName);
	m_cbColumn = SQL_NTS;


	ret = SQLBindParameter(g_hReadStmt, 1, SQL_PARAM_OUTPUT, SQL_C_SHORT, SQL_SMALLINT,
		0, 0, &sRet,
		0, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hReadStmt);
		return false;
	}

	ret = SQLBindParameter(g_hReadStmt, 2, SQL_PARAM_INPUT, SQL_UNICODE, SQL_UNICODE,
		m_iDataLength, 0, m_szReadName,
		m_iDataLength, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hReadStmt);
		return false;
	}	

	SQLBindCol(g_hReadStmt, 1, SQL_INTEGER, &retID, 0, &lID);
	SQLBindCol(g_hReadStmt, 2, SQL_UNICODE, retName, _countof(retName), &lName);
	SQLBindCol(g_hReadStmt, 3, SQL_UNICODE, retPass, _countof(retPass), &lPass);
	SQLBindCol(g_hReadStmt, 4, SQL_INTEGER, &retLevel, 0, &lLevel);
	SQLBindCol(g_hReadStmt, 5, SQL_INTEGER, &retSex, 0, &lSex);
	SQLBindCol(g_hReadStmt, 6, SQL_TYPE_TIMESTAMP, &accountTS, sizeof(accountTS), &lAccount);
	SQLBindCol(g_hReadStmt, 7, SQL_TYPE_TIMESTAMP, &loginTS, sizeof(loginTS), &llogin);
	SQLBindCol(g_hReadStmt, 8, SQL_TYPE_TIMESTAMP, &logoutTS, sizeof(logoutTS), &llogout);*/
	return true;
}
bool TOdbc::CreateAlluserinfo()
{
	SQLRETURN ret;
	/// <summary>
	/// g_hSelectAllStmt
	/// </summary>
	/// <returns></returns>
	//std::wstring sql= L"select * from ACCOUNT";
	std::wstring sql = L"{CALL usp_Alluserinfo}";
	ret = SQLAllocHandle(SQL_HANDLE_STMT, g_hDbc, &g_hSelectAllStmt);
	ret = SQLPrepare(g_hSelectAllStmt, (SQLTCHAR*)sql.c_str(), SQL_NTS);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hSelectAllStmt);
		return false;
	}
	return true;
}
bool TOdbc::CreatePassOut()
{
	TCHAR sql5[] = L"{?=CALL dbo.usp_passout(?,?)}";
	SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, g_hDbc, &g_hPassStmt);
	ret = SQLPrepare(g_hPassStmt, (SQLTCHAR*)sql5, SQL_NTS);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hPassStmt);
		return false;
	}

	m_iDataLength = sizeof(m_szSelectName);
	m_cbColumn = SQL_NTS;


	ret = SQLBindParameter(g_hPassStmt, 1, SQL_PARAM_OUTPUT, SQL_C_SHORT, SQL_SMALLINT,
		0, 0, &sRet,
		0, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hPassStmt);
		return false;
	}

	ret = SQLBindParameter(g_hPassStmt, 2, SQL_PARAM_INPUT, SQL_UNICODE, SQL_CHAR,
		m_iDataLength, 0, m_szSelectName,
		m_iDataLength, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hPassStmt);
		return false;
	}

	ret = SQLBindParameter(g_hPassStmt, 3, SQL_PARAM_OUTPUT, SQL_UNICODE, SQL_UNICODE,
		m_iDataLength, 0, m_szOutPass,
		m_iDataLength, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hPassStmt);
		return false;
	}
	return true;
}
bool TOdbc::CreateUpdate()
{
	//TCHAR sql4[256] = L"update ACCOUNT set NAME=?,PASS=?,GLEVEL=?,LOGIN=? where NAME=?";
	//std::wstring sql = L"update ACCOUNT set NAME=?,PASS=?,GLEVEL=?,SEX=? where NAME=?";
	std::wstring sql = L"{?=CALL dbo.usp_UpdateAccount(?,?,?,?,?)}";
	SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, g_hDbc, &g_hUpdateStmt);
	ret = SQLPrepare(g_hUpdateStmt, (SQLTCHAR*)sql.c_str(), SQL_NTS);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hUpdateStmt);
		return false;
	}

	m_iDataLength = sizeof(m_szUpdateName);
	m_cbColumn = SQL_NTS;
	::ZeroMemory(&m_ts, sizeof(m_ts));

	ret = SQLBindParameter(g_hUpdateStmt, 1, SQL_PARAM_OUTPUT, SQL_C_SHORT, SQL_SMALLINT,
		0, 0, &sRet,
		0, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hUpdateStmt);
		return false;
	}

	ret = SQLBindParameter(g_hUpdateStmt, 2, SQL_PARAM_INPUT, SQL_UNICODE, SQL_CHAR,
		m_iDataLength, 0, m_szSelectName,
		m_iDataLength, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hUpdateStmt);
		return false;
	}
	ret = SQLBindParameter(g_hUpdateStmt, 3, SQL_PARAM_INPUT, SQL_UNICODE, SQL_CHAR,
		m_iDataLength, 0, m_szUpdateName,
		m_iDataLength, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hUpdateStmt);
		return false;
	}
	ret = SQLBindParameter(g_hUpdateStmt, 4, SQL_PARAM_INPUT, SQL_UNICODE, SQL_CHAR,
		m_iDataLength, 0, m_szUpdatePass,
		m_iDataLength, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hUpdateStmt);
		return false;
	}
	ret = SQLBindParameter(g_hUpdateStmt, 5, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER,
		0, 0, &m_iUpdateLevel,
		0, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hUpdateStmt);
		return false;
	}

	ret = SQLBindParameter(g_hUpdateStmt, 6, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER,
		0, 0, &m_iUpdateSex,
		0, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hUpdateStmt);
		return false;
	}

	/*ret = SQLBindParameter(g_hUpdateStmt, 5, SQL_PARAM_INPUT, SQL_TYPE_TIMESTAMP, SQL_TYPE_TIMESTAMP,
		SQL_TIMESTAMP_LEN, 0, &m_ts,
		sizeof(TIMESTAMP_STRUCT), &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hUpdateStmt);
		return false;
	}*/
	//WCHAR szTime[] = L"2023-01-01 12:33:02.123456"; //밀리초(소수점6자리)
	//ret = SQLBindParameter(g_hUpdateStmt, 4, SQL_PARAM_INPUT, SQL_TYPE_TIMESTAMP, SQL_TYPE_TIMESTAMP,
	//	sizeof(szTime), 6, szTime,
	//	0, &m_cbColumn);
	//if (ret != SQL_SUCCESS)
	//{
	//	ErrorMsg();
	//	return false;
	//}

	//ret = SQLBindParameter(g_hUpdateStmt, 5, SQL_PARAM_INPUT, SQL_UNICODE, SQL_CHAR,
	//	m_iDataLength, 0, m_szSelectName,
	//	m_iDataLength, &m_cbColumn);
	//if (ret != SQL_SUCCESS)
	//{
	//	ErrorMsg(g_hUpdateStmt);
	//	return false;
	//}
	return true;
}
bool TOdbc::CreatePrepare()
{
	SQLRETURN ret;
	/// <summary>
	/// g_hSelectAllStmt
	/// </summary>
	/// <returns></returns>
	CreateAlluserinfo();
	/// <summary>
	/// g_hReadStmt
	/// </summary>
	/// <returns></returns>
	CreateUserinfo();
	/// <summary>
	/// g_hUpdateStmt
	/// </summary>
	/// <returns></returns>
	CreateUpdate();
	/// <summary>
	/// g_hPassStmt
	/// </summary>
	/// <returns></returns>
	CreatePassOut();
	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	CreateInsertAccount();
	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	CreateDeleteAccount();

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
		ErrorMsg(NULL);
		return;
	}

	if (hr == SQL_SUCCESS)
	{
		//연결핸들(g_hDbc)
		hr = SQLAllocHandle(SQL_HANDLE_DBC, g_hEnv, &g_hDbc);
		if (hr != SQL_SUCCESS)
		{
			ErrorMsg(NULL);
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
		ErrorMsg(NULL);
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
		ErrorMsg();ㅐㅣ
		return;
	}*/

	// KGCATest, sa, kgca!@34
	SQLRETURN hr = SQLConnect(g_hDbc, (SQLTCHAR*)L"KGCATest", SQL_NTS,
									  (SQLTCHAR*)L"sa", SQL_NTS, 
									  (SQLTCHAR*)L"kgca!@34", SQL_NTS);
	if (hr != SQL_SUCCESS && hr != SQL_SUCCESS_WITH_INFO)
	{
		ErrorMsg(NULL);
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
bool TOdbc::DeleteAccount(std::wstring szName)
{
	ZeroMemory(m_szSelectName, sizeof(TCHAR)* 10);
	CopyMemory(m_szSelectName, szName.c_str(), szName.size() * sizeof(TCHAR));
	m_iDataLength = sizeof(m_szSelectName);
	m_cbColumn = SQL_NTS;


	SQLRETURN ret = SQLBindParameter(g_hDeleteStmt, 1, SQL_PARAM_OUTPUT, SQL_C_SHORT, SQL_SMALLINT,
		0, 0, &sRet,
		0, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hDeleteStmt);
		return false;
	}

	ret = SQLBindParameter(g_hDeleteStmt, 2, SQL_PARAM_INPUT, SQL_UNICODE, SQL_CHAR,
		m_iDataLength, 0, m_szSelectName,
		m_iDataLength, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hDeleteStmt);
		return false;
	}
	SQLRETURN hr = SQLExecute(g_hDeleteStmt);
	if (hr != SQL_SUCCESS || hr == SQL_SUCCESS_WITH_INFO)
	{
		ErrorMsg(g_hDeleteStmt);
		return false;
	}
	if (g_hDeleteStmt) SQLCloseCursor(g_hDeleteStmt);
	SQLFreeStmt(g_hReadStmt, SQL_RESET_PARAMS);
	SQLFreeStmt(g_hDeleteStmt, SQL_CLOSE);
	return true;
}
void TOdbc::ErrorMsg(SQLHSTMT  stmt)
{
	int value = -1;
	SQLTCHAR sqlState[10] = { 0, };
	SQLTCHAR msg[SQL_MAX_MESSAGE_LENGTH + 1] = { 0, };
	SQLTCHAR errorMsg[SQL_MAX_MESSAGE_LENGTH + 1] = { 0, };
	SQLSMALLINT msgLen;
	SQLINTEGER nativeError = 0;

	SQLRETURN hr;
	// 복합에러
	/*while (hr = SQLGetDiagRec(SQL_HANDLE_STMT, stmt, value, sqlState, &nativeError, msg,
		_countof(msg), &msgLen) != SQL_NO_DATA)*/
		//단순에러
	SQLError(g_hEnv, g_hDbc, stmt,
		sqlState, &nativeError, msg, SQL_MAX_MESSAGE_LENGTH + 1, &msgLen);
	{
		_stprintf(errorMsg, L"SQLSTATE:%s, 진단정보:%s, 에러코드:%d",
			sqlState, msg, nativeError);
		::MessageBox(NULL, errorMsg, L"진단정보", 0);
	}
}
bool TOdbc::AddSQL(dbitem& record)
{	
	ZeroMemory(m_szInsertName, sizeof(TCHAR) * 10);
	CopyMemory(m_szInsertName, record.name.c_str(), record.name.size()*sizeof(TCHAR));
	ZeroMemory(m_szInsertPass, sizeof(TCHAR) * 10);
	CopyMemory(m_szInsertPass, record.pass.c_str(), record.pass.size() * sizeof(TCHAR));
	m_iDataLength = sizeof(m_szInsertName);
	m_cbColumn = SQL_NTS;

	SQLRETURN ret = SQLBindParameter(g_hInsertStmt, 1, SQL_PARAM_OUTPUT, SQL_C_SHORT, SQL_SMALLINT,
		0, 0, &sRet,
		0, &m_cbColumn);
	if (ret != SQL_SUCCESS && ret != SQL_DATA_AT_EXEC)
	{
		ErrorMsg(g_hInsertStmt);
		return false;
	}

	ret = SQLBindParameter(g_hInsertStmt, 2, SQL_PARAM_INPUT, SQL_UNICODE, SQL_CHAR,
		m_iDataLength, 0, m_szInsertName,
		m_iDataLength, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hInsertStmt);
		return false;
	}
	ret = SQLBindParameter(g_hInsertStmt, 3, SQL_PARAM_INPUT, SQL_UNICODE, SQL_CHAR,
		m_iDataLength, 0, m_szInsertPass,
		m_iDataLength, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hInsertStmt);
		return false;
	}
	//m_iSelectLevel = record.level;

	SQLRETURN hr = SQLExecute(g_hInsertStmt);
	if (hr != SQL_SUCCESS || hr == SQL_SUCCESS_WITH_INFO)
	{
		ErrorMsg(g_hInsertStmt);
		return false;
	}
	if (g_hInsertStmt) SQLCloseCursor(g_hInsertStmt);
	SQLFreeStmt(g_hReadStmt, SQL_RESET_PARAMS);
	SQLFreeStmt(g_hInsertStmt, SQL_CLOSE);
	return true;
}
bool TOdbc::UpdateSQL(dbitem& record, std::wstring selectName)
{
	// 버퍼의 길이가 NCHAR(10) 보다 크면 안된다. 
	ZeroMemory(m_szSelectName, sizeof(TCHAR) * 10);
	CopyMemory(m_szSelectName, selectName.c_str(), selectName.size() * sizeof(TCHAR));

	ZeroMemory(m_szUpdateName, sizeof(TCHAR) * 10);
	CopyMemory(m_szUpdateName, record.name.c_str(), record.name.size() * sizeof(TCHAR));

	ZeroMemory(m_szUpdatePass, sizeof(TCHAR) * 10);
	CopyMemory(m_szUpdatePass, record.pass.c_str(), record.pass.size() * sizeof(TCHAR));


	m_iUpdateLevel = record.level;
	m_iUpdateSex = record.sex;

	m_iDataLength = sizeof(m_szUpdateName);
	m_cbColumn = SQL_NTS;
	::ZeroMemory(&m_ts, sizeof(m_ts));
	SWORD sReturn;
	SQLBindParameter(g_hUpdateStmt, 1, SQL_PARAM_OUTPUT, SQL_C_SHORT, SQL_SMALLINT,
		0, 0, &sReturn,
		0, &m_cbColumn);
	SQLBindParameter(g_hUpdateStmt, 2, SQL_PARAM_INPUT, SQL_UNICODE, SQL_UNICODE,
		m_iDataLength, 0, m_szSelectName,
		m_iDataLength, &m_cbColumn);
	SQLBindParameter(g_hUpdateStmt, 3, SQL_PARAM_INPUT, SQL_UNICODE, SQL_UNICODE,
		m_iDataLength, 0, m_szUpdateName,
		m_iDataLength, &m_cbColumn);
	SQLBindParameter(g_hUpdateStmt, 4, SQL_PARAM_INPUT, SQL_UNICODE, SQL_UNICODE,
		m_iDataLength, 0, m_szUpdatePass,
		m_iDataLength, &m_cbColumn);	
	SQLBindParameter(g_hUpdateStmt, 5, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER,
		0, 0, &m_iUpdateLevel,
		0, &m_cbColumn);	
	SQLBindParameter(g_hUpdateStmt, 6, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER,
		0, 0, &m_iUpdateSex,
		0, &m_cbColumn);

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
	if (hr == SQL_SUCCESS || hr == SQL_SUCCESS_WITH_INFO)
	{
		if (SQLFetch(g_hUpdateStmt) != SQL_NO_DATA)
		{
			if (g_hUpdateStmt) SQLCloseCursor(g_hUpdateStmt);
			SQLFreeStmt(g_hUpdateStmt, SQL_RESET_PARAMS);
			SQLFreeStmt(g_hUpdateStmt, SQL_CLOSE);
			return true;
		}
	}
	ErrorMsg(g_hUpdateStmt);
	if (g_hUpdateStmt) SQLCloseCursor(g_hUpdateStmt);
	SQLFreeStmt(g_hReadStmt, SQL_RESET_PARAMS);
	SQLFreeStmt(g_hUpdateStmt, SQL_CLOSE);
	return true;
}
bool TOdbc::ReadRecord(std::wstring selectName)
{
	SQLRETURN hr1 = SQLBindCol(g_hReadStmt, 1, SQL_INTEGER, &retID, 0, &lID);
	SQLBindCol(g_hReadStmt, 2, SQL_UNICODE, retName, _countof(retName), &lName);
	SQLBindCol(g_hReadStmt, 3, SQL_UNICODE, retPass, _countof(retPass), &lPass);
	SQLBindCol(g_hReadStmt, 4, SQL_INTEGER, &retLevel, 0, &lLevel);
	SQLBindCol(g_hReadStmt, 5, SQL_INTEGER, &retSex, 0, &lSex);
	SQLBindCol(g_hReadStmt, 6, SQL_TYPE_TIMESTAMP, &accountTS, sizeof(accountTS), &lAccount);
	SQLBindCol(g_hReadStmt, 7, SQL_TYPE_TIMESTAMP, &loginTS, sizeof(loginTS), &llogin);
	SQLBindCol(g_hReadStmt, 8, SQL_TYPE_TIMESTAMP, &logoutTS, sizeof(logoutTS), &llogout);
	m_iDataLength = sizeof(m_szReadName);
	m_cbColumn = SQL_NTS;


	SQLRETURN ret = SQLBindParameter(g_hReadStmt, 1, SQL_PARAM_OUTPUT, SQL_C_SHORT, SQL_SMALLINT,
		0, 0, &sRet,
		0, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hReadStmt);
		return false;
	}

	ret = SQLBindParameter(g_hReadStmt, 2, SQL_PARAM_INPUT, SQL_UNICODE, SQL_UNICODE,
		m_iDataLength, 0, m_szReadName,
		m_iDataLength, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hReadStmt);
		return false;
	}

	if (!selectName.empty())
	{
		ZeroMemory(m_szReadName, sizeof(TCHAR)*10);
		CopyMemory(m_szReadName, selectName.c_str(), selectName.size()*sizeof(TCHAR));
		SQLRETURN hr = SQLExecute(g_hReadStmt);
		if (hr == SQL_SUCCESS || hr == SQL_SUCCESS_WITH_INFO)
		{
			if (SQLFetch(g_hReadStmt) != SQL_NO_DATA)
			{
				if (g_hReadStmt) SQLCloseCursor(g_hReadStmt);
				SQLFreeStmt(g_hReadStmt, SQL_RESET_PARAMS);
				SQLFreeStmt(g_hReadStmt, SQL_UNBIND);
				SQLFreeStmt(g_hReadStmt, SQL_CLOSE);
				return true;
			}			
		}
	}
	ErrorMsg(g_hReadStmt);
	if (g_hReadStmt) SQLCloseCursor(g_hReadStmt);
	SQLFreeStmt(g_hReadStmt, SQL_CLOSE);
	return false;
}
bool TOdbc::UserPass(std::wstring szName)
{
	if (!szName.empty())
	{
		ZeroMemory(m_szSelectName, sizeof(TCHAR) * 10);
		CopyMemory(m_szSelectName, szName.c_str(), szName.size()*sizeof(TCHAR));
		m_iDataLength = sizeof(m_szSelectName);
		m_cbColumn = SQL_NTS;


		SQLRETURN ret = SQLBindParameter(g_hPassStmt, 1, SQL_PARAM_OUTPUT, SQL_C_SHORT, SQL_SMALLINT,
			0, 0, &sRet,
			0, &m_cbColumn);
		if (ret != SQL_SUCCESS)
		{
			ErrorMsg(g_hPassStmt);
			return false;
		}

		ret = SQLBindParameter(g_hPassStmt, 2, SQL_PARAM_INPUT, SQL_UNICODE, SQL_CHAR,
			m_iDataLength, 0, m_szSelectName,
			m_iDataLength, &m_cbColumn);
		if (ret != SQL_SUCCESS)
		{
			ErrorMsg(g_hPassStmt);
			return false;
		}

		ret = SQLBindParameter(g_hPassStmt, 3, SQL_PARAM_OUTPUT, SQL_UNICODE, SQL_UNICODE,
			m_iDataLength, 0, m_szOutPass,
			m_iDataLength, &m_cbColumn);
		if (ret != SQL_SUCCESS)
		{
			ErrorMsg(g_hPassStmt);
			return false;
		}

		SQLRETURN hr = SQLExecute(g_hPassStmt);
		if (hr == SQL_SUCCESS || hr == SQL_SUCCESS_WITH_INFO)
		{
			if (SQLFetch(g_hPassStmt) != SQL_NO_DATA)
			{
				if (g_hPassStmt) SQLCloseCursor(g_hPassStmt);
				SQLFreeStmt(g_hPassStmt, SQL_RESET_PARAMS);
				SQLFreeStmt(g_hPassStmt, SQL_CLOSE);
				return true;
			}
		}
	}
	ErrorMsg(g_hPassStmt);
	if (g_hPassStmt) SQLCloseCursor(g_hPassStmt);
	SQLFreeStmt(g_hPassStmt, SQL_CLOSE);
	return false;
}