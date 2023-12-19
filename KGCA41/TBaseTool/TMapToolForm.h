#pragma once



// TMapToolForm 폼 보기

class TMapToolForm : public CFormView
{
	DECLARE_DYNCREATE(TMapToolForm)
public:
	static TMapToolForm* CreateOne(CWnd* parent);
protected:
	TMapToolForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~TMapToolForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TMapToolForm };
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
	afx_msg void OnBnCreateMap();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	CListBox m_ObjectList;
	virtual void OnInitialUpdate();
	afx_msg void OnLbnSelchangeObjectList();
public:
	void LoadTextureFile();
	CString m_csSelectedfile;
};


