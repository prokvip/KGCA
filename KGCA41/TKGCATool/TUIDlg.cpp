// TUIDlg.cpp: 구현 파일
//

#include "pch.h"
#include "TKGCATool.h"
#include "TUIDlg.h"


// TUIDlg

IMPLEMENT_DYNCREATE(TUIDlg, CFormView)
TUIDlg* TUIDlg::CreateFormView(CWnd* pParent)
{
	TUIDlg* pForm = new TUIDlg;
	pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 300, 300), pParent, 0, NULL);
	return pForm;
}
TUIDlg::TUIDlg()
	: CFormView(IDD_UI_DLG)
{

}

TUIDlg::~TUIDlg()
{
}

void TUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(TUIDlg, CFormView)
END_MESSAGE_MAP()


// TUIDlg 진단

#ifdef _DEBUG
void TUIDlg::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void TUIDlg::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// TUIDlg 메시지 처리기
