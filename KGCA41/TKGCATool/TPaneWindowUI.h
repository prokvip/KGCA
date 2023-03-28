#pragma once
#include "TUIDlg.h"

// TPaneWindowUI

class TPaneWindowUI : public CDockablePane
{
	DECLARE_DYNAMIC(TPaneWindowUI)
	TUIDlg* m_wndForm;
public:
	TPaneWindowUI();
	virtual ~TPaneWindowUI();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};


