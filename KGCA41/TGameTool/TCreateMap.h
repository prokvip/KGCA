#pragma once


// TCreateMap 대화 상자

class TCreateMap : public CDialogEx
{
	DECLARE_DYNAMIC(TCreateMap)

public:
	TCreateMap(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~TCreateMap();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CREATEMAP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_iNumCompoment;
	CString m_iNumSection;
	CString m_iDistance;
	virtual BOOL OnInitDialog();
	CComboBox m_TextureSelectCtrl;
	CString m_iSelectTexture;
};
