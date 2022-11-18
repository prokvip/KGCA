// TCreateMap.cpp: 구현 파일
//

#include "pch.h"
#include "TGameTool.h"
#include "TCreateMap.h"
#include "afxdialogex.h"


// TCreateMap 대화 상자

IMPLEMENT_DYNAMIC(TCreateMap, CDialogEx)

TCreateMap::TCreateMap(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CREATEMAP, pParent)
	, m_iNumCompoment(_T(""))
	, m_iNumSection(_T(""))
	, m_iDistance(_T(""))
	, m_iSelectTexture(_T(""))
{

}

TCreateMap::~TCreateMap()
{
}

void TCreateMap::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_iNumCompoment);
	DDX_Text(pDX, IDC_EDIT2, m_iNumSection);
	DDX_Text(pDX, IDC_EDIT3, m_iDistance);
	DDX_Control(pDX, IDC_COMBO1, m_TextureSelectCtrl);
	DDX_CBString(pDX, IDC_COMBO1, m_iSelectTexture);
}


BEGIN_MESSAGE_MAP(TCreateMap, CDialogEx)
END_MESSAGE_MAP()


// TCreateMap 메시지 처리기


BOOL TCreateMap::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_iNumCompoment = L"10";
	m_iNumSection = L"20";
	m_iDistance = L"1.0f";
	m_TextureSelectCtrl.AddString(L"1");
	m_TextureSelectCtrl.AddString(L"2");
	m_TextureSelectCtrl.AddString(L"4");
	m_TextureSelectCtrl.AddString(L"8");
	m_TextureSelectCtrl.AddString(L"16");
	m_TextureSelectCtrl.SetCurSel(2);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
