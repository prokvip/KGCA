// TMapCtrl.cpp: 구현 파일
//

#include "pch.h"
#include "TKGCATool.h"
#include "TMapCtrl.h"


// TMapCtrl

IMPLEMENT_DYNCREATE(TMapCtrl, CFormView)

TMapCtrl* TMapCtrl::CreateFormView(CWnd* pParent)
{
	TMapCtrl* pForm = new TMapCtrl;
	pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 300, 300), pParent, 0, NULL);
	return pForm;
}

TMapCtrl::TMapCtrl()
	: CFormView(IDD_MAP_CTL)
{

}

TMapCtrl::~TMapCtrl()
{
}

void TMapCtrl::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(TMapCtrl, CFormView)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDOK, &TMapCtrl::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK1, &TMapCtrl::OnBnClickedCheck1)
END_MESSAGE_MAP()


// TMapCtrl 진단

#ifdef _DEBUG
void TMapCtrl::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void TMapCtrl::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// TMapCtrl 메시지 처리기


int TMapCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}


void TMapCtrl::OnBnClickedOk()
{
	AfxMessageBox(L"OnBnClickedOk");
}


void TMapCtrl::OnBnClickedCheck1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
