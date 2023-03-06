#define _CRT_SECURE_NO_WARNINGS
#include "TObdc.h"
#include "resource.h"

TObdc  g_odbc;

HWND g_hDlgList;
HWND g_hDlgName;
HWND g_hDlgPrice;
HWND g_hDlgKorean;
void Load();

void SelectReadRecord()
{
	int id = SendMessage(g_hDlgList, LB_GETCURSEL, 0, 0);
	if (id != -1)
	{
		SetWindowText(g_hDlgName, g_odbc.m_dbDataList[id][0].c_str());
		SetWindowText(g_hDlgPrice, g_odbc.m_dbDataList[id][1].c_str());
		SendMessage(g_hDlgKorean, BM_SETCHECK,
			((g_odbc.m_dbDataList[id][2]==L"true") ? BST_CHECKED : BST_UNCHECKED), 0);
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
	_stprintf(sql, L"delete from tblCigar where name='%s'", selectName);
	SQLRETURN hr = SQLExecDirect(g_odbc.g_hStmt, sql, SQL_NTS);
	if (hr == SQL_SUCCESS)
	{
		if (g_odbc.g_hStmt) SQLCloseCursor(g_odbc.g_hStmt);
		SendMessage(g_hDlgList, LB_RESETCONTENT, 0, 0);
		Load();
		for (auto data : g_odbc.m_dbDataList)
		{
			SendMessage(g_hDlgList, LB_ADDSTRING, 0, (LPARAM)data[0].c_str());
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

	if (g_odbc.AddSQL(record))
	{
		SendMessage(g_hDlgList, LB_RESETCONTENT, 0, 0);
		Load();
		for (auto data : g_odbc.m_dbDataList)
		{
			SendMessage(g_hDlgList, LB_ADDSTRING, 0, (LPARAM)data[0].c_str());
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
	if (g_odbc.UpdateSQL(record, selectUpdateName))
	{
		SendMessage(g_hDlgList, LB_RESETCONTENT, 0, 0);
		Load();
		for (auto data : g_odbc.m_dbDataList)
		{
			SendMessage(g_hDlgList, LB_ADDSTRING, 0, (LPARAM)data[0].c_str());
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
			for( auto rec : g_odbc.m_dbDataList)
			{
				SendMessage(g_hDlgList, LB_ADDSTRING, 0, (LPARAM)rec[0].c_str());
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
								
								TCHAR retName[25] = { 0, };
								SQLLEN  lName;
								int    retPrice;
								SQLLEN  lPrice;
								int    retKorean;
								SQLLEN  lKorean;
								SQLBindCol(g_odbc.g_hReadStmt, 1, SQL_UNICODE, retName, _countof(retName), &lName);
								SQLBindCol(g_odbc.g_hReadStmt, 2, SQL_INTEGER, &retPrice, 0, &lPrice);
								SQLBindCol(g_odbc.g_hReadStmt, 3, SQL_C_ULONG, &retKorean, 0, &lKorean);

								if (g_odbc.ReadRecord(selectName))
								{
									SetWindowText(g_hDlgName, retName);
									SetWindowText(g_hDlgPrice, std::to_wstring(retPrice).c_str());
									SendMessage(g_hDlgKorean, BM_SETCHECK,
										(retKorean ? BST_CHECKED : BST_UNCHECKED), 0);
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

	// ��ü ���ڵ� ��ȸ, �߰�, ����, ����
	/*TCHAR sql[] = L"select * from tblCigar";
	SQLRETURN hr = SQLExecDirect(g_odbc.g_hStmt, sql, SQL_NTS);*/
	SQLRETURN hr = SQLExecute(g_odbc.g_hSelectAllStmt);
	if (hr != SQL_SUCCESS)
	{
		g_odbc.ErrorMsg();
		return;
	}
	SQLLEN  count; // update, insert, delete ��밡���Դ�.
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
			/* SQLBindCol ��ü�Ѵ�.
			   �������� ������� ��� ���� ��Ʈ������ �ްڴ�.*/
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
	g_odbc.Connect();
	Load();
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), HWND_DESKTOP, (DLGPROC)DlgProc );
	g_odbc.DisConnect();
}