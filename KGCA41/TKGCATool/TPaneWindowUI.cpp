// TPaneWindowUI.cpp: 구현 파일
//

#include "pch.h"
#include "TKGCATool.h"
#include "TPaneWindowUI.h"


// TPaneWindowUI

IMPLEMENT_DYNAMIC(TPaneWindowUI, CDockablePane)

TPaneWindowUI::TPaneWindowUI()
{

}

TPaneWindowUI::~TPaneWindowUI()
{
}


BEGIN_MESSAGE_MAP(TPaneWindowUI, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()



// TPaneWindowUI 메시지 처리기




int TPaneWindowUI::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	m_wndForm = TUIDlg::CreateFormView(this);
	return 0;
}


void TPaneWindowUI::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_wndForm)
	{
		m_wndForm->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER);
	}
}


int TPaneWindowUI::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	CFrameWnd* pParentFrame = GetParentFrame();
	if (pParentFrame == pDesktopWnd ||
		pDesktopWnd->IsChild(pParentFrame))
	{
		return CDockablePane::OnMouseActivate(pDesktopWnd, nHitTest, message);
	}
	return MA_NOACTIVATE;
}
