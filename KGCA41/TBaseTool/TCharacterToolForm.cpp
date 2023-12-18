// TCharacterToolForm.cpp: 구현 파일
//

#include "pch.h"
#include "TBaseTool.h"
#include "TCharacterToolForm.h"


// TCharacterToolForm

IMPLEMENT_DYNCREATE(TCharacterToolForm, CFormView)
TCharacterToolForm* TCharacterToolForm::CreateOne(CWnd* parent)
{
	TCharacterToolForm* pForm = new TCharacterToolForm;
	pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 500, 500), parent, 0, NULL);
	return pForm;
}
TCharacterToolForm::TCharacterToolForm()
	: CFormView(IDD_TCharacterToolForm)
{

}

TCharacterToolForm::~TCharacterToolForm()
{
}

void TCharacterToolForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(TCharacterToolForm, CFormView)
END_MESSAGE_MAP()


// TCharacterToolForm 진단

#ifdef _DEBUG
void TCharacterToolForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void TCharacterToolForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// TCharacterToolForm 메시지 처리기
