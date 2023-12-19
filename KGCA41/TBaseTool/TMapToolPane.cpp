// TMapToolPane.cpp: 구현 파일
//

#include "pch.h"
#include "TBaseTool.h"
#include "TMapToolPane.h"


// TMapToolPane

IMPLEMENT_DYNAMIC(TMapToolPane, CDockablePane)

TMapToolPane::TMapToolPane()
{

}

TMapToolPane::~TMapToolPane()
{
}


BEGIN_MESSAGE_MAP(TMapToolPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_MOUSEACTIVATE()
	ON_BN_CLICKED(IDC_BUTTON2, &TMapToolPane::OnBnClickedButton2)
END_MESSAGE_MAP()



// TMapToolPane 메시지 처리기




//BOOL TMapToolPane::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
//{
//	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
//
//	return CDockablePane::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
//}


int TMapToolPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	m_wndForm = TMapToolForm::CreateOne(this);
	return 0;
}


void TMapToolPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_wndForm)
	{
		m_wndForm->SetWindowPos(
				NULL, 0,0,cx,cy, 
				SWP_NOZORDER);
	}
}


int TMapToolPane::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CFrameWnd* pParentFrame = GetParentFrame();
	if (pParentFrame == pDesktopWnd ||
		pDesktopWnd->IsChild(pParentFrame))
	{
		return CDockablePane::OnMouseActivate(
			pDesktopWnd, nHitTest, message	);
	}
	return MA_NOACTIVATE;
}


void TMapToolPane::OnBnClickedButton2()
{
	CFileDialog dlg(TRUE, L"bmp", NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
		L"bmp Files(*.bmp)|*.bmp| All Files(*.*)|*.*|", this);
	if (dlg.DoModal())
	{
		CString selfilename = dlg.GetPathName();
		AfxMessageBox(selfilename);
	}
}
