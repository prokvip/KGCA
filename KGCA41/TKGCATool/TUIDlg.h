#pragma once



// TUIDlg 폼 보기

class TUIDlg : public CFormView
{
	DECLARE_DYNCREATE(TUIDlg)

protected:
	TUIDlg();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~TUIDlg();

public:
public:
	static TUIDlg* CreateFormView(CWnd* pParent);
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UI_DLG };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};


