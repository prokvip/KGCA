// TMapToolForm.cpp: 구현 파일
//

#include "pch.h"
#include "TBaseTool.h"
#include "TMapToolForm.h"


// TMapToolForm

IMPLEMENT_DYNCREATE(TMapToolForm, CFormView)

TMapToolForm* TMapToolForm::CreateOne(CWnd* parent)
{
	TMapToolForm* pForm = new TMapToolForm;
	pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 500, 500), parent, 0, NULL);
	return pForm;
}
TMapToolForm::TMapToolForm()
	: CFormView(IDD_TMapToolForm)
	, m_csSelectedfile(_T(""))
{

}

TMapToolForm::~TMapToolForm()
{
}

void TMapToolForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OBJECT_LIST, m_ObjectList);
	DDX_Text(pDX, IDC_EDIT4, m_csSelectedfile);
}

BEGIN_MESSAGE_MAP(TMapToolForm, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &TMapToolForm::OnBnCreateMap)
	ON_EN_CHANGE(IDC_EDIT1, &TMapToolForm::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON2, &TMapToolForm::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &TMapToolForm::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &TMapToolForm::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &TMapToolForm::OnBnClickedButton5)
	ON_LBN_SELCHANGE(IDC_OBJECT_LIST, &TMapToolForm::OnLbnSelchangeObjectList)
END_MESSAGE_MAP()


// TMapToolForm 진단

#ifdef _DEBUG
void TMapToolForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void TMapToolForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// TMapToolForm 메시지 처리기


void TMapToolForm::OnBnCreateMap()
{
	CString data1;
	GetDlgItemText(IDC_EDIT1, data1);
	CString data2;
	GetDlgItemText(IDC_EDIT2, data2);
	CString data3;
	GetDlgItemText(IDC_EDIT3, data3);
	CString msg = data1 + data2 + data3;
	AfxMessageBox(msg);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void TMapToolForm::OnEnChangeEdit1()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString data;
	GetDlgItemText(IDC_EDIT1, data);
	//AfxMessageBox(data);
}


void TMapToolForm::OnBnClickedButton2()
{
	CFileDialog dlg(TRUE, L"bmp", NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
		L"bmp Files(*.bmp)|*.bmp| All Files(*.*)|*.*|", this);
	if (dlg.DoModal())
	{
		CString selfilepath = dlg.GetPathName();
		CString selfilename = dlg.GetFileName();
		CString selfileext = dlg.GetFileExt();
		AfxMessageBox(selfilename);
	}
}


void TMapToolForm::OnBnClickedButton3()
{
	CFileDialog dlg(FALSE, L"bmp", NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
		L"bmp Files(*.bmp)|*.bmp| All Files(*.*)|*.*|", this);
	if (dlg.DoModal())
	{
		CString selfilepath = dlg.GetPathName();
		CString selfilename = dlg.GetFileName();
		CString selfileext = dlg.GetFileExt();
		AfxMessageBox(selfilename);
	}
}


void TMapToolForm::OnBnClickedButton4()
{
	CColorDialog dlg(RGB(0, 0, 0), CC_FULLOPEN);
	if (dlg.DoModal())
	{
		COLORREF color = dlg.GetColor();
		CString data;
		data.Format(L"%u, %u %u", GetRValue(color),
			GetGValue(color),
			GetBValue(color));
		AfxMessageBox(data);
	}
}


void TMapToolForm::OnBnClickedButton5()
{
	CFontDialog dlg;
	if (dlg.DoModal())
	{
		CString data = dlg.GetFaceName();		
		AfxMessageBox(data);
	}
}


void TMapToolForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	m_ObjectList.AddString(L"aaa");
	m_ObjectList.AddString(L"bbb");
	m_ObjectList.AddString(L"cc");
	LoadTextureFile();
	m_csSelectedfile = L"aaa";
	UpdateData(FALSE);
}


void TMapToolForm::OnLbnSelchangeObjectList()
{
	CString data;
	int iIndex = m_ObjectList.GetCurSel();
	m_ObjectList.GetText(iIndex, data);
	m_csSelectedfile = data;
	UpdateData(FALSE);
	//AfxMessageBox(data);
}


void TMapToolForm::LoadTextureFile()
{
	TCHAR path[MAX_PATH] = { 0, };
	GetCurrentDirectory(MAX_PATH, path);
	_tcscat_s(path, _T("\\..\\..\\res\\*.*"));
	HANDLE hSearch = NULL;

	WIN32_FIND_DATA  data;
	hSearch = FindFirstFile(path, &data);

	int iCnt = 0;
	BOOL bFind = TRUE;
	while (bFind)
	{
		if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{

		}
		else
		{
			m_ObjectList.AddString(data.cFileName);
			//m_ObjectList.InsertItem(iCnt, data.cFileName, 0);
			//SYSTEMTIME st;
			////SystemTimeToFileTime(&st, &data.ftCreationTime);
			//FileTimeToSystemTime(&data.ftCreationTime, &st);

			//CString date;
			//date.AppendFormat(_T("%ld년%ld월%ld일"), st.wYear,
			//	st.wMonth, st.wDay);
			//m_ObjList.SetItemText(iCnt++, 1, date);
		}
		bFind = FindNextFile(hSearch, &data);
	}
	FindClose(hSearch);
}