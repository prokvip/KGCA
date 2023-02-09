#pragma once



// TMapGen 폼 보기

class TMapGen : public CFormView
{
	DECLARE_DYNCREATE(TMapGen)
public:
	void Recurse(LPCTSTR pstr);
public:
	std::vector<T_STR>  m_objList;
	static TMapGen* CreateFormView(CWnd* pParent);
protected:
	TMapGen();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~TMapGen();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
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
	UINT m_iCols;
	afx_msg void OnBnClickedButton1();
	UINT m_iRows;
};


