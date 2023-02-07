#pragma once



// TMapCtrl 폼 보기

class TMapCtrl : public CFormView
{
	DECLARE_DYNCREATE(TMapCtrl)
public:
	static TMapCtrl* CreateFormView(CWnd* pParent);
protected:
	TMapCtrl();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~TMapCtrl();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAP_CTL };
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
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCheck1();
};


