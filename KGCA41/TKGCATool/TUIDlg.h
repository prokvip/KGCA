#pragma once



// TUIDlg 폼 보기

class TUIDlg : public CFormView
{
	DECLARE_DYNCREATE(TUIDlg)

protected:
	TUIDlg();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~TUIDlg();

public:
	T_STR  m_strSelectTexture;
	std::vector<T_STR>  m_TextureNameList;
	void Recurse(LPCTSTR pFilePath, LPCTSTR ext, std::vector<T_STR>& list);
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
public:
	UINT m_iParticleCounter;
	afx_msg void OnBnClickedButton1();
	CListBox m_TextureList;
	afx_msg void OnLbnSelchangeList1();
	virtual void OnInitialUpdate();
	virtual void OnFinalRelease();
};


