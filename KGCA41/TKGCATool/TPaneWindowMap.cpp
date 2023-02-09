// TPaneWindowMap.cpp: 구현 파일
//

#include "pch.h"
#include "TKGCATool.h"
#include "TPaneWindowMap.h"


// TPaneWindowMap

IMPLEMENT_DYNAMIC(TPaneWindowMap, CDockablePane)

TPaneWindowMap::TPaneWindowMap()
{

}

TPaneWindowMap::~TPaneWindowMap()
{
}


BEGIN_MESSAGE_MAP(TPaneWindowMap, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()



// TPaneWindowMap 메시지 처리기




//BOOL TPaneWindowMap::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
//{
//	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
//
//	return CDockablePane::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
//}


int TPaneWindowMap::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_wndForm = TMapGen::CreateFormView(this);
	return 0;
}


void TPaneWindowMap::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	if (m_wndForm)
	{
		m_wndForm->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER);
	}
}


int TPaneWindowMap::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	CFrameWnd* pParentFrame = GetParentFrame();
	if (pParentFrame == pDesktopWnd ||
		pDesktopWnd->IsChild(pParentFrame))
	{
		return CDockablePane::OnMouseActivate(pDesktopWnd, nHitTest, message);
	}
	return MA_NOACTIVATE;
}


void TPaneWindowMap::OnFinalRelease()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CDockablePane::OnFinalRelease();
}
