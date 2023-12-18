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
