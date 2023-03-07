#define _CRT_SECURE_NO_WARNINGS
#include "TObdc.h"

#include "resource.h"

TObdc  g_odbc;

HWND g_hDlgList;
HWND g_hDlgName;
HWND g_hDlgPass;
HWND g_hDlgLevel;
HWND g_hDlgAccount;
void Load();

void SelectReadRecord()
{
	int id = SendMessage(g_hDlgList, LB_GETCURSEL, 0, 0);
	if (id != -1)
	{
		SetWindowText(g_hDlgName, g_odbc.m_dbDataList[id][1].c_str());
		SetWindowText(g_hDlgPass, g_odbc.m_dbDataList[id][2].c_str());
		SetWindowText(g_hDlgLevel, g_odbc.m_dbDataList[id][3].c_str());		
	}
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
	_stprintf(sql, L"delete from ACCOUNT where NAME='%s'", selectName);
	SQLRETURN hr = SQLExecDirect(g_odbc.g_hStmt, sql, SQL_NTS);
	if (hr == SQL_SUCCESS)
	{
		if (g_odbc.g_hStmt) SQLCloseCursor(g_odbc.g_hStmt);
		SendMessage(g_hDlgList, LB_RESETCONTENT, 0, 0);
		Load();
		for (auto data : g_odbc.m_dbDataList)
		{
			SendMessage(g_hDlgList, LB_ADDSTRING, 0, (LPARAM)data[1].c_str());
		}
	}
}
void InsertRecord()
{
	dbitem record;
	SQLTCHAR name[255] = { 0, };
	SQLTCHAR pass[255] = { 0, };
	SQLTCHAR level[255] = { 0, };
	GetWindowText(g_hDlgName, name, 255);
	GetWindowText(g_hDlgPass, pass, 255);
	GetWindowText(g_hDlgLevel, level, 255);
	
	record.name = name;
	record.pass = pass;
	record.level = _ttoi(level);

	if (g_odbc.AddSQL(record))
	{
		SendMessage(g_hDlgList, LB_RESETCONTENT, 0, 0);
		Load();
		for (auto data : g_odbc.m_dbDataList)
		{
			SendMessage(g_hDlgList, LB_ADDSTRING, 0, (LPARAM)data[1].c_str());
		}
	}
}
void UpdateRecord()
{
	dbitem record;
	SQLTCHAR name[255] = { 0, };
	SQLTCHAR pass[255] = { 0, };
	SQLTCHAR level[255] = { 0, };
	GetWindowText(g_hDlgName, name, 255);
	GetWindowText(g_hDlgPass, pass, 255);
	GetWindowText(g_hDlgLevel, level, 255);

	record.name = name;
	record.pass = pass;
	record.level = _ttol(level);

	TCHAR selectUpdateName[255] = { 0, };
	int id = SendMessage(g_hDlgList, LB_GETCURSEL, 0, 0);
	if (id != -1)
	{
		SendMessage(g_hDlgList, LB_GETTEXT, id, (LPARAM)selectUpdateName);
	}
	if (g_odbc.UpdateSQL(record, selectUpdateName))
	{
		SendMessage(g_hDlgList, LB_RESETCONTENT, 0, 0);
		Load();
		for (auto data : g_odbc.m_dbDataList)
		{
			SendMessage(g_hDlgList, LB_ADDSTRING, 0, (LPARAM)data[1].c_str());
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
			g_hDlgPass = GetDlgItem(hDlg, IDC_PASS);
			g_hDlgLevel = GetDlgItem(hDlg, IDC_LEVEL);
			g_hDlgAccount = GetDlgItem(hDlg, IDC_ACCOUNTTIME);
			for( auto rec : g_odbc.m_dbDataList)
			{
				SendMessage(g_hDlgList, LB_ADDSTRING, 0, (LPARAM)rec[1].c_str());
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
						{
							int id = SendMessage(g_hDlgList, LB_GETCURSEL, 0, 0);
							if (id != -1)
							{
								TCHAR selectName[256] = { 0, };
								SendMessage(g_hDlgList, LB_GETTEXT, id, (LPARAM)selectName);
								
								// 현재 시간을 스트링화
								std::time_t now = std::time(NULL); // 1970,01,01, 0시
								std::tm* ptm = std::localtime(&now);								
								std::string timestring = std::asctime(ptm);
								char buffer[255] = { 0, };
								std::strftime(buffer, 255, "%a, %d.%m.%Y %H:%M:%S", ptm);
								
								// 100일 경과
								//std::time_t difftime = std::time(NULL);
								//difftime += 1 * 24 * 60 * 60;
								//int iDiff = std::difftime(difftime, now);
		

								TCHAR retName[25] = { 0, };
								SQLLEN  lName;
								TCHAR retPass[25] = { 0, };
								SQLLEN  lPass;
								int    retLevel;
								SQLLEN  lLevel;			
								TIMESTAMP_STRUCT ts1;
								SQLBindCol(g_odbc.g_hReadStmt, 1, SQL_UNICODE, retName, _countof(retName), &lName);
								SQLBindCol(g_odbc.g_hReadStmt, 2, SQL_UNICODE, retPass, _countof(retPass), &lPass);
								SQLBindCol(g_odbc.g_hReadStmt, 3, SQL_INTEGER, &retLevel, 0, &lLevel);
								SQLBindCol(g_odbc.g_hReadStmt, 4, SQL_TYPE_TIMESTAMP, &ts1, sizeof(ts1), NULL);
							

								if (g_odbc.ReadRecord(selectName))
								{
									SetWindowText(g_hDlgName, retName);
									SetWindowText(g_hDlgPass, retPass);
									SetWindowText(g_hDlgLevel, std::to_wstring(retLevel).c_str());

									std::wstring account;
									account += std::to_wstring(ts1.year); account += L"년";
									account += std::to_wstring(ts1.month); account += L"월";
									account += std::to_wstring(ts1.day); account += L"일";										
									account += std::to_wstring(ts1.hour); account += L"시";
									account += std::to_wstring(ts1.minute); account += L"분";
									account += std::to_wstring(ts1.second); account += L"초";
									SetWindowText(g_hDlgAccount, account.c_str());									
								}
								return TRUE;
							}
						}						
					}
			}
		}
	}
	return FALSE;
}
void Load()
{
	g_odbc.m_dbDataList.clear();
	g_odbc.m_ColumnList.clear();

	// 전체 레코드 조회, 추가, 수정, 삭제
	/*TCHAR sql[] = L"select * from tblCigar";
	SQLRETURN hr = SQLExecDirect(g_odbc.g_hStmt, sql, SQL_NTS);*/
	SQLRETURN hr = SQLExecute(g_odbc.g_hSelectAllStmt);
	if (hr != SQL_SUCCESS)
	{
		g_odbc.ErrorMsg();
		return;
	}
	SQLLEN  count; // update, insert, delete 사용가능함다.
	SQLRETURN ret = SQLRowCount(g_odbc.g_hSelectAllStmt, &count);

	TColDescription col;
	col.icol = 1;
	while (1)
	{
		ret = SQLDescribeCol(g_odbc.g_hSelectAllStmt,
			col.icol,
			col.szColName,sizeof(col.szColName),
			&col.pcchColName,
			&col.pfSqlType,
			&col.pcbColDef,
			&col.pibScale,
			&col.pfNullable	);
		if (ret != SQL_SUCCESS)
		{
			//g_odbc.ErrorMsg();
			break;
		}
		g_odbc.m_ColumnList.push_back(col);
		col.icol++;
	}


	while (SQLFetch(g_odbc.g_hSelectAllStmt) != SQL_NO_DATA)
	{
		RECORD record;
		dbitem dtItem;
		for (int iCol=0; iCol < g_odbc.m_ColumnList.size(); iCol++)
		{
			/* SQLBindCol 대체한다.
			   데이터형 상관없이 모든 것을 스트링으로 받겠다.*/
			ret = SQLGetData(g_odbc.g_hSelectAllStmt, g_odbc.m_ColumnList[iCol].icol,
				SQL_WCHAR, g_odbc.m_ColumnList[iCol].bindData,
				sizeof(g_odbc.m_ColumnList[iCol].bindData), NULL);
			if (ret == SQL_SUCCESS)
			{	
				record.push_back(g_odbc.m_ColumnList[iCol].bindData);
			}
		}
		if (ret == SQL_SUCCESS)
		{
			g_odbc.m_dbDataList.push_back(record);
		}
	}
	if (g_odbc.g_hSelectAllStmt) SQLCloseCursor(g_odbc.g_hSelectAllStmt);

	SQLFreeStmt(g_odbc.g_hSelectAllStmt, SQL_CLOSE);
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
					, LPSTR lpszCmdParam, int nCmdShow)
{
	g_odbc.Init();
	g_odbc.Connect(L"../../data/db/Account.accdb");
	Load();
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), HWND_DESKTOP, (DLGPROC)DlgProc );
	g_odbc.DisConnect();
}