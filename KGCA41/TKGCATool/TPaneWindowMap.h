#pragma once

#include "TMapCtrl.h"
// TPaneWindowMap

class TPaneWindowMap : public CDockablePane
{
	DECLARE_DYNAMIC(TPaneWindowMap)
public:
	TMapCtrl*		m_wndForm=nullptr;
public:
	TPaneWindowMap();
	virtual ~TPaneWindowMap();

protected:
	DECLARE_MESSAGE_MAP()
public:
//	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	virtual void OnFinalRelease();
};


