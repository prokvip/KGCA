// TCreateMapDlg.cpp: 구현 파일
//

#include "pch.h"
#include "TKGCATool.h"
#include "TCreateMapDlg.h"
#include "afxdialogex.h"


// TCreateMapDlg 대화 상자

IMPLEMENT_DYNAMIC(TCreateMapDlg, CDialogEx)

TCreateMapDlg::TCreateMapDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CREATE_MAP_DLG, pParent)
{

}

TCreateMapDlg::~TCreateMapDlg()
{
}

void TCreateMapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(TCreateMapDlg, CDialogEx)
	ON_BN_CLICKED(IDC_OPENFILE_DLG, &TCreateMapDlg::OnBnClickedOpenfileDlg)
END_MESSAGE_MAP()


// TCreateMapDlg 메시지 처리기


void TCreateMapDlg::OnBnClickedOpenfileDlg()
{
	//	

	CFileDialog FileDlg(FALSE,// TRUE:열기, FALSE:저장
		L"bmp", 
		NULL, 		
		OFN_HIDEREADONLY | OFN_NOCHANGEDIR,
		L"bmp Files(*.bmp)|*.bmp|All Files(*.*)|*.*|",
		this);
	CString selectFileName;
	if (FileDlg.DoModal() == IDOK)
	{
		selectFileName = FileDlg.GetPathName();
		selectFileName += FileDlg.GetFileName();
		AfxMessageBox(selectFileName);
	}
	
	/*CColorDialog ColorDlg(RGB(255, 0, 0), CC_FULLOPEN);
	CString selectFileName;
	if (ColorDlg.DoModal() == IDOK)
	{
		COLORREF color = ColorDlg.GetColor();
		selectFileName.Format(L"%u, %u, %u", GetRValue(color),
			GetGValue(color), 
			GetBValue(color));
		AfxMessageBox(selectFileName);
	}*/

	/*CFontDialog FontDlg;
	CString selectFileName;
	if (FontDlg.DoModal() == IDOK)
	{
		COLORREF color = FontDlg.GetColor();
		selectFileName.Format(L"%s, %d, %u, %u, %u", 
			FontDlg.GetFaceName(),
			FontDlg.GetSize(),
			GetRValue(color),
			GetGValue(color),
			GetBValue(color));
		AfxMessageBox(selectFileName);
	}*/
}
