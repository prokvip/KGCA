#pragma once
#include "TCharacterToolForm.h"

// TCharacterToolPane

class TCharacterToolPane : public CDockablePane
{
	DECLARE_DYNAMIC(TCharacterToolPane)
public:
	TCharacterToolForm* m_wndForm;
public:
	TCharacterToolPane();
	virtual ~TCharacterToolPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};


