#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <tchar.h>
#include <string>
#include <vector>
#include "resource.h"

SQLHENV   g_hEnv;
SQLHDBC	  g_hDbc;
SQLHSTMT  g_hStmt;

HWND g_hDlgList;
HWND g_hDlgName;
HWND g_hDlgPrice;
HWND g_hDlgKorean;

struct dbitem
{
	std::wstring name;
	int			price;
	bool		korean;
};
std::vector<dbitem> g_dbList;

void ErrorMsg();
void Connect()
{
	// 환경핸들(g_hEnv), 연결핸들(g_hDbc), 명령핸들(g_hStmt)
	SQLRETURN hr = SQLAllocHandle( SQL_HANDLE_ENV, SQL_NULL_HANDLE, &g_hEnv);
	if (hr != SQL_SUCCESS)
	{
		return;
	}
	if (SQLSetEnvAttr(g_hEnv, SQL_ATTR_ODBC_VERSION,
		(SQLPOINTER)SQL_OV_ODBC3,
		SQL_IS_INTEGER) != SQL_SUCCESS)
	{
		ErrorMsg();
		return;
	}

	if (hr == SQL_SUCCESS)
	{			
		//연결핸들(g_hDbc)
		hr = SQLAllocHandle(SQL_HANDLE_DBC, g_hEnv, &g_hDbc );
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
		if (hr != SQL_SUCCESS)
		{
			ErrorMsg();
			return;
		}

		//명령핸들(g_hStmt)
		SQLAllocHandle(SQL_HANDLE_STMT, g_hDbc, &g_hStmt);
	}
}
void DisConnect()
{
	if(g_hEnv ) SQLFreeHandle(SQL_HANDLE_STMT, g_hEnv);
	if (g_hDbc) SQLFreeHandle(SQL_HANDLE_DBC, g_hDbc);
	if (g_hStmt) SQLFreeHandle(SQL_HANDLE_ENV, g_hStmt);
}

bool AddSQL(dbitem& record);
bool UpdateSQL(dbitem& record, const TCHAR* name);

void Load()
{
	g_dbList.clear();

	// 전체 레코드 조회, 추가, 수정, 삭제
	TCHAR sql[] = L"select name, price, korean from tblCigar";
	TCHAR retName[25] = { 0, };
	SQLLEN  lName;
	int    retPrice;
	SQLLEN  lPrice;
	int    retKorean;
	SQLLEN  lKorean;
	SQLBindCol(g_hStmt, 1, SQL_UNICODE, retName, _countof(retName), &lName);
	SQLBindCol(g_hStmt, 2, SQL_INTEGER, &retPrice, 0, &lPrice);
	SQLBindCol(g_hStmt, 3, SQL_C_ULONG, &retKorean, 0, &lKorean);

	SQLRETURN hr = SQLExecDirect(g_hStmt, sql, SQL_NTS);
	while (SQLFetch(g_hStmt) != SQL_NO_DATA)
	{
		dbitem item;
		item.name = retName;
		item.price = retPrice;
		item.korean = retKorean;
		g_dbList.push_back(item);
	}
	if (g_hStmt) SQLCloseCursor(g_hStmt);
}
void SelectReadRecord()
{
	int id = SendMessage(g_hDlgList, LB_GETCURSEL, 0, 0);
	if (id != -1)
	{
		SetWindowText(g_hDlgName, g_dbList[id].name.c_str());
		SetWindowText(g_hDlgPrice, std::to_wstring(g_dbList[id].price).c_str());
		SendMessage(g_hDlgKorean, BM_SETCHECK,
					( g_dbList[id].korean ? BST_CHECKED: BST_UNCHECKED), 0);
	}
}
void ReadRecord()
{
	int id = SendMessage(g_hDlgList, LB_GETCURSEL, 0, 0);
	if (id != -1)
	{
		TCHAR selectName[256] = { 0, };
		SendMessage(g_hDlgList, LB_GETTEXT, id, (LPARAM)selectName);
		TCHAR sql[256] = { 0, };
		_stprintf(sql, L"select name, price, korean from tblCigar where name='%s'", selectName);

		TCHAR retName[25] = { 0, };
		SQLLEN  lName;
		int    retPrice;
		SQLLEN  lPrice;
		int    retKorean;
		SQLLEN  lKorean;
		SQLBindCol(g_hStmt, 1, SQL_UNICODE, retName, _countof(retName), &lName);
		SQLBindCol(g_hStmt, 2, SQL_INTEGER, &retPrice, 0, &lPrice);
		SQLBindCol(g_hStmt, 3, SQL_C_ULONG, &retKorean, 0, &lKorean);

		SQLRETURN hr = SQLExecDirect(g_hStmt, sql, SQL_NTS);
		if (hr == SQL_SUCCESS)
		{
			if (SQLFetch(g_hStmt) != SQL_NO_DATA)
			{
				SetWindowText(g_hDlgName, retName);
				SetWindowText(g_hDlgPrice, std::to_wstring(retPrice).c_str());
				SendMessage(g_hDlgKorean, BM_SETCHECK,
					(retKorean ? BST_CHECKED : BST_UNCHECKED), 0);
			}
		}
		if (g_hStmt) SQLCloseCursor(g_hStmt);
	}
}
void ErrorMsg()
{
	int value = -1;
	SQLTCHAR sqlState[10] = { 0, };
	SQLTCHAR msg[SQL_MAX_MESSAGE_LENGTH+1] = { 0, };
	SQLTCHAR errorMsg[SQL_MAX_MESSAGE_LENGTH+1] = { 0, };
	SQLSMALLINT msgLen;
	SQLINTEGER nativeError=0;

	SQLRETURN hr;
	// 복합에러
	/*while (hr = SQLGetDiagRec(SQL_HANDLE_STMT, g_hStmt, value, sqlState, &nativeError, msg,
		_countof(msg), &msgLen) != SQL_NO_DATA)*/
	//단순에러
	SQLError(g_hEnv, g_hDbc, g_hStmt,
		sqlState, &nativeError, msg, SQL_MAX_MESSAGE_LENGTH+1, &msgLen);
	{
		_stprintf(errorMsg, L"SQLSTATE:%s, 진단정보:%s, 에러코드:%d",
			sqlState, msg, nativeError);
		::MessageBox(NULL, errorMsg, L"진단정보", 0);
	}
}
bool AddSQL(dbitem& record)
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
bool UpdateSQL(dbitem& record, const TCHAR* name)
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
void DeleteRecord()
{
	int id = SendMessage(g_hDlgList, LB_GETCURSEL, 0, 0);
	TCHAR selectName[256] = { 0, };
	if (id != -1)
	{		
		SendMessage(g_hDlgList, LB_GETTEXT, id, (LPARAM)selectName);
	}
	TCHAR sql[256] = { 0, };
	_stprintf(sql, L"delete from tblCigar where name='%s'", selectName);
	SQLRETURN hr = SQLExecDirect(g_hStmt, sql, SQL_NTS);
	if (hr == SQL_SUCCESS)
	{
		if (g_hStmt) SQLCloseCursor(g_hStmt);
		SendMessage(g_hDlgList, LB_RESETCONTENT, 0, 0);
		Load();
		for (auto data : g_dbList)
		{
			SendMessage(g_hDlgList, LB_ADDSTRING, 0, (LPARAM)data.name.c_str());
		}
	}		
}
void InsertRecord()
{
	dbitem record;
	SQLTCHAR name[255] = { 0, };
	SQLTCHAR price[255] = { 0, };
	int      korean;
	GetWindowText(g_hDlgName, name, 255);
	GetWindowText(g_hDlgPrice, price, 255);
	BOOL ret = (SendMessage(g_hDlgKorean, BM_GETCHECK, 0, 0) == BST_CHECKED);
	record.name = name;
	record.price = _ttoi(price);
	record.korean = ret;

	if (AddSQL(record))
	{		
		SendMessage(g_hDlgList, LB_RESETCONTENT, 0, 0);
		Load();
		for (auto data : g_dbList)
		{
			SendMessage(g_hDlgList, LB_ADDSTRING, 0, (LPARAM)data.name.c_str());
		}
	}
}
void UpdateRecord()
{
	dbitem record;
	SQLTCHAR name[255] = { 0, };
	SQLTCHAR price[255] = { 0, };
	int      korean;
	GetWindowText(g_hDlgName, name, 255);
	GetWindowText(g_hDlgPrice, price, 255);
	BOOL ret = (SendMessage(g_hDlgKorean, BM_GETCHECK, 0, 0) == BST_CHECKED);
	record.name = name;
	record.price = _ttoi(price);
	record.korean = ret;

	TCHAR selectUpdateName[255] = { 0, };
	int id = SendMessage(g_hDlgList, LB_GETCURSEL, 0, 0);
	if (id != -1)
	{
		SendMessage(g_hDlgList, LB_GETTEXT, id, (LPARAM)selectUpdateName);
	}
	if(UpdateSQL(record, selectUpdateName))
	{
		SendMessage(g_hDlgList, LB_RESETCONTENT, 0, 0);
		Load();
		for (auto data : g_dbList)
		{
			SendMessage(g_hDlgList, LB_ADDSTRING, 0, (LPARAM)data.name.c_str());
		}
	}
}
LRESULT CALLBACK DlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
		case WM_INITDIALOG:
		{
			g_hDlgList = GetDlgItem(hDlg, IDC_LIST1);
			g_hDlgName = GetDlgItem(hDlg, IDC_NAME);
			g_hDlgPrice = GetDlgItem(hDlg, IDC_PRICE);
			g_hDlgKorean = GetDlgItem(hDlg, IDC_KOREAN);
			for( auto data : g_dbList)
			{
				SendMessage(g_hDlgList, LB_ADDSTRING, 0, (LPARAM)data.name.c_str());
			}
		}return TRUE;
		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
				case IDOK:  EndDialog(hDlg, IDOK); return TRUE;
				case IDCANCEL:  EndDialog(hDlg, IDOK); return TRUE;
				case IDC_DELETE: DeleteRecord(); return TRUE;
				case IDC_INSERT: InsertRecord(); return TRUE;
				case IDC_UPDATE: UpdateRecord(); return TRUE;
				case IDC_LIST1:
					switch (HIWORD(wParam))
					{
					case LBN_SELCHANGE:
						ReadRecord();
						return TRUE;
					}
			}
		}
	}
	return FALSE;
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
					, LPSTR lpszCmdParam, int nCmdShow)
{
	Connect();
	Load();
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), HWND_DESKTOP, (DLGPROC)DlgProc );
	DisConnect();
}