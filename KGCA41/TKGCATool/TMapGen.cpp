// TMapGen.cpp: 구현 파일
//

#include "pch.h"
#include "TKGCATool.h"
#include "TMapGen.h"


// TMapGen

IMPLEMENT_DYNCREATE(TMapGen, CFormView)

TMapGen::TMapGen()
	: CFormView(IDD_DIALOG1)
	, m_iCols(0)
	, m_iRows(0)
{

}

TMapGen::~TMapGen()
{
}

void TMapGen::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_iCols);
	DDX_Text(pDX, IDC_EDIT3, m_iRows);
}

BEGIN_MESSAGE_MAP(TMapGen, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &TMapGen::OnBnClickedButton1)
END_MESSAGE_MAP()


// TMapGen 진단
TMapGen* TMapGen::CreateFormView(CWnd* pParent)
{
	TMapGen* pForm = new TMapGen;
	pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 300, 300), pParent, 0, NULL);
	return pForm;
}
void TMapGen::Recurse(LPCTSTR pstr)
{
	// 폴더 읽기
	CFileFind cfile;
	CString strFileName = _T("");
	CString strFilePath(pstr);
	strFilePath += _T("/*.*");
	BOOL bFind;
	bFind = cfile.FindFile(strFilePath);
	while (bFind)
	{
		bFind = cfile.FindNextFile();
		if (!cfile.IsDots() && !cfile.IsDirectory())
		{
			strFileName = cfile.GetFileName();
			LPTSTR strName = strFileName.GetBuffer();
			//UseObject->AddItem(strName);
			//m_objList.push_back(strName);
		}
	}
	cfile.Close();
}
#ifdef _DEBUG
void TMapGen::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void TMapGen::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// TMapGen 메시지 처리기


void TMapGen::OnBnClickedButton1()
{
	CTKGCAToolApp* pApp = (CTKGCAToolApp*)AfxGetApp();
	UpdateData(TRUE);
	pApp->m_Sample.CreateMapData(m_iCols, m_iRows);
}
