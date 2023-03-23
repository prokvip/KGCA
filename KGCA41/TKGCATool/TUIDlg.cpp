// TUIDlg.cpp: 구현 파일
//

#include "pch.h"
#include "TKGCATool.h"
#include "TUIDlg.h"


// TUIDlg

IMPLEMENT_DYNCREATE(TUIDlg, CFormView)
TUIDlg* TUIDlg::CreateFormView(CWnd* pParent)
{
	TUIDlg* pForm = new TUIDlg;
	pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 300, 300), pParent, 0, NULL);
	return pForm;
}
TUIDlg::TUIDlg()
	: CFormView(IDD_UI_DLG)
	, m_iParticleCounter(0)
{

}

TUIDlg::~TUIDlg()
{
}

void TUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_iParticleCounter);
	DDX_Control(pDX, IDC_LIST1, m_TextureList);
}

BEGIN_MESSAGE_MAP(TUIDlg, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &TUIDlg::OnBnClickedButton1)
	ON_LBN_SELCHANGE(IDC_LIST1, &TUIDlg::OnLbnSelchangeList1)
END_MESSAGE_MAP()


// TUIDlg 진단

#ifdef _DEBUG
void TUIDlg::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void TUIDlg::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// TUIDlg 메시지 처리기


void TUIDlg::OnBnClickedButton1()
{
	UpdateData(TRUE);
	UINT iCnt = m_iParticleCounter;
	CTKGCAToolApp* pApp = (CTKGCAToolApp*)AfxGetApp();
	T_STR filepath = L"../../data/effect/";
	filepath += m_strSelectTexture;
	pApp->m_Sample.NewEffect(m_iParticleCounter, filepath);

	/*for (UINT i = 0; i < m_iParticleCounter; i++)
	{
		pApp->m_Sample.NewEffect(m_iParticleCounter);
	}*/
}


void TUIDlg::OnLbnSelchangeList1()
{
	CString name;
	int iSelect = m_TextureList.GetCurSel();
	m_TextureList.GetText(iSelect, name);
	m_strSelectTexture = name.GetBuffer();
	//UpdateData(FALSE);
}


void TUIDlg::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	UpdateData(FALSE);

	Recurse(L"../../data/effect", L"/*.*", m_TextureNameList);
	for (T_STR data : m_TextureNameList)
	{
		CString str(data.c_str());
		m_TextureList.InsertString(m_TextureList.GetCount(), str);
	}
}


void TUIDlg::OnFinalRelease()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CFormView::OnFinalRelease();
}


void TUIDlg::Recurse(LPCTSTR pFilePath, LPCTSTR ext, std::vector<T_STR>& list)
{
	// 폴더 읽기
	CFileFind cfile;
	CString strFileName = _T("");
	CString strFilePath(pFilePath);
	strFilePath += ext;// _T("/*.*");
	BOOL bFind;
	bFind = cfile.FindFile(strFilePath);
	while (bFind)
	{
		bFind = cfile.FindNextFile();
		if (!cfile.IsDots() && !cfile.IsDirectory())
		{
			strFileName = cfile.GetFileName();
			LPTSTR strName = strFileName.GetBuffer();
			//UseObject->AddItem(strName);
			list.push_back(strName);
		}
	}
	cfile.Close();
}