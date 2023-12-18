#pragma once
#include "TMapToolForm.h"

// TMapToolPane

class TMapToolPane : public CDockablePane
{
	DECLARE_DYNAMIC(TMapToolPane)
public:
	TMapToolForm* m_wndForm;
public:
	TMapToolPane();
	virtual ~TMapToolPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
//	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};


