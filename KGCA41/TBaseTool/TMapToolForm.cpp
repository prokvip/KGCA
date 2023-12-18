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
{

}

TMapToolForm::~TMapToolForm()
{
}

void TMapToolForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(TMapToolForm, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &TMapToolForm::OnBnCreateMap)
	ON_EN_CHANGE(IDC_EDIT1, &TMapToolForm::OnEnChangeEdit1)
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
