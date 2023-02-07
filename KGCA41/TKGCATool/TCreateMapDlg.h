#pragma once


// TCreateMapDlg 대화 상자

class TCreateMapDlg : public CDialogEx
{
	DECLARE_DYNAMIC(TCreateMapDlg)

public:
	TCreateMapDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~TCreateMapDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CREATE_MAP_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOpenfileDlg();
};
