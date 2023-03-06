#include "TObdc.h"
bool TObdc::CreatePrepare()
{
	SQLRETURN ret;
	/// <summary>
	/// g_hSelectAllStmt
	/// </summary>
	/// <returns></returns>
	std::wstring sql= L"select * from tblCigar";
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
	std::wstring sql2 = L"select * from tblCigar where name=?";
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
	TCHAR sql3[256] = L"insert into tblCigar (name,price,korean) values(?,?,?)";
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
	ret = SQLBindParameter(g_hInsertStmt, 2, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER,
		0, 0, &m_iSelectPrice,
		0, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg();
		return false;
	}
	ret = SQLBindParameter(g_hInsertStmt, 3, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER,
		0, 0, &m_iSelectKorean,
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
	TCHAR sql4[256] = L"update tblCigar set name=?,price=?,korean=? where name=?";
	ret = SQLAllocHandle(SQL_HANDLE_STMT, g_hDbc, &g_hUpdateStmt);
	ret = SQLPrepare(g_hUpdateStmt, (SQLTCHAR*)sql4, SQL_NTS);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg();
		return false;
	}

	m_iDataLength = sizeof(m_szInsertName);
	m_cbColumn = SQL_NTS;

	ret = SQLBindParameter(g_hUpdateStmt, 1, SQL_PARAM_INPUT, SQL_UNICODE, SQL_CHAR,
		m_iDataLength, 0, m_szInsertName,
		m_iDataLength, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg();
		return false;
	}
	ret = SQLBindParameter(g_hUpdateStmt, 2, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER,
		0, 0, &m_iSelectPrice,
		0, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg();
		return false;
	}
	ret = SQLBindParameter(g_hUpdateStmt, 3, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER,
		0, 0, &m_iSelectKorean,
		0, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg();
		return false;
	}

	ret = SQLBindParameter(g_hUpdateStmt, 4, SQL_PARAM_INPUT, SQL_UNICODE, SQL_CHAR,
		m_iDataLength, 0, m_szSelectName,
		m_iDataLength, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg();
		return false;
	}
	return true;
}
void TObdc::Init()
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
void TObdc::Connect()
{
	TCHAR  inConnect[255] = { 0, };
	TCHAR  outConnect[255] = { 0, };
	TCHAR  dir[MAX_PATH] = { 0, };
	GetCurrentDirectory(MAX_PATH, dir);
	_stprintf(inConnect, _T("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};DBQ=%s\\cigarette.accdb;"), dir);

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
void TObdc::DisConnect()
{
	if (g_hEnv) SQLFreeHandle(SQL_HANDLE_STMT, g_hEnv);
	if (g_hDbc) SQLFreeHandle(SQL_HANDLE_DBC, g_hDbc);
	if (g_hStmt) SQLFreeHandle(SQL_HANDLE_ENV, g_hStmt);
}

void TObdc::ErrorMsg()
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
bool TObdc::AddSQL(dbitem& record)
{	
	ZeroMemory(m_szInsertName, sizeof(m_szInsertName));
	CopyMemory(m_szInsertName, record.name.c_str(), record.name.size()*sizeof(TCHAR));

	m_iSelectPrice = record.price;
	m_iSelectKorean = record.korean;
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
bool TObdc::UpdateSQL(dbitem& record, std::wstring selectName)
{
	//TCHAR sql[256] = { 0, };
	//_stprintf(sql, L"update tblCigar set name='%s',price=%d,korean=%d where name='%s'",
	//	record.name.c_str(), record.price, (int)record.korean, name);
	//SQLRETURN hr = SQLExecDirect(g_hStmt, sql, SQL_NTS);

	ZeroMemory(m_szSelectName, sizeof(m_szSelectName));
	CopyMemory(m_szSelectName, selectName.c_str(), selectName.size() * sizeof(TCHAR));

	ZeroMemory(m_szInsertName, sizeof(m_szInsertName));
	CopyMemory(m_szInsertName, record.name.c_str(), record.name.size() * sizeof(TCHAR));

	m_iSelectPrice = record.price;
	m_iSelectKorean = record.korean;
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
bool TObdc::ReadRecord(const TCHAR* szName)
{
	if (szName != nullptr)
	{
		ZeroMemory(m_szSelectName, sizeof(m_szSelectName));
		CopyMemory(m_szSelectName, szName, sizeof(szName));

		SQLRETURN hr = SQLExecute(g_hReadStmt);

		if (hr != SQL_SUCCESS)
		{
			ErrorMsg();
			if (g_hReadStmt) SQLCloseCursor(g_hReadStmt);
			return false;
		}
		if (SQLFetch(g_hReadStmt) != SQL_NO_DATA)
		{
			if (g_hReadStmt) SQLCloseCursor(g_hReadStmt);
			return true;
		}		
		if (g_hReadStmt) SQLCloseCursor(g_hReadStmt);
	}
	SQLFreeStmt(g_hReadStmt, SQL_CLOSE);
	return false;
}