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

HWND	  g_hDlgList;
std::vector<std::wstring> g_dbList;


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
		return;
	}

	if (hr == SQL_SUCCESS)
	{			
		//연결핸들(g_hDbc)
		hr = SQLAllocHandle(SQL_HANDLE_DBC, g_hEnv, &g_hDbc );
		if (hr != SQL_SUCCESS)
		{
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
		if (hr != SQL_SUCCESS) return;

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

LRESULT CALLBACK DlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
		case WM_INITDIALOG:
		{
			g_hDlgList = GetDlgItem(hDlg, IDC_LIST1);
			for( auto data : g_dbList)
			{
				SendMessage(g_hDlgList, LB_ADDSTRING, 0, (LPARAM)data.c_str());
			}
		}return TRUE;
		case WM_COMMAND:
		{
		case IDOK:  EndDialog(hDlg, IDOK); return TRUE;
		case IDCANCEL:  EndDialog(hDlg, IDOK); return TRUE;
		}
	}
	return FALSE;
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
					, LPSTR lpszCmdParam, int nCmdShow)
{
	Connect();
	// 전체 레코드 조회, 추가, 수정, 삭제
	TCHAR sql[] = L"select name from tblCigar";
	TCHAR retName[25] = { 0, };
	SQLLEN  lName;
	SQLBindCol(g_hStmt, 1, SQL_UNICODE, retName, _countof(retName), &lName);

	SQLRETURN hr = SQLExecDirect(g_hStmt, sql, SQL_NTS);	
	while (SQLFetch(g_hStmt) != SQL_NO_DATA)
	{
		g_dbList.push_back(retName);		
	}

	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), HWND_DESKTOP, 
			 (DLGPROC)DlgProc );
	DisConnect();
}