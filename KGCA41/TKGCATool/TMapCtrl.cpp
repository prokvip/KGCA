// TMapCtrl.cpp: 구현 파일
//

#include "pch.h"
#include "TKGCATool.h"
#include "TMapCtrl.h"


// TMapCtrl

IMPLEMENT_DYNCREATE(TMapCtrl, CFormView)

TMapCtrl* TMapCtrl::CreateFormView(CWnd* pParent)
{
	TMapCtrl* pForm = new TMapCtrl;
	pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 300, 300), pParent, 0, NULL);
	return pForm;
}

TMapCtrl::TMapCtrl()
	: CFormView(IDD_MAP_CTL)
	, m_bWireFrame(FALSE)
{

}

TMapCtrl::~TMapCtrl()
{
}

void TMapCtrl::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK2, m_bWireFrame);
	DDX_Control(pDX, IDC_LIST1, m_TextureList);
}

BEGIN_MESSAGE_MAP(TMapCtrl, CFormView)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDOK, &TMapCtrl::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK2, &TMapCtrl::OnBnClickedCheck2)
	ON_LBN_SELCHANGE(IDC_LIST1, &TMapCtrl::OnLbnSelchangeList1)
END_MESSAGE_MAP()


// TMapCtrl 진단

#ifdef _DEBUG
void TMapCtrl::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void TMapCtrl::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// TMapCtrl 메시지 처리기


int TMapCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}


void TMapCtrl::OnBnClickedOk()
{
	AfxMessageBox(L"OnBnClickedOk");
}


void TMapCtrl::OnBnClickedCheck1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void TMapCtrl::OnBnClickedCheck2()
{
	CTKGCAToolApp* pApp = (CTKGCAToolApp*)AfxGetApp();

	UpdateData(TRUE);
	pApp->m_Sample.m_bWireFrame = m_bWireFrame;
}


void TMapCtrl::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	Recurse(L"D:/00_KGCA41Leture/data/map");
	for (T_STR data : m_objList)
	{
		CString str(data.c_str());
		m_TextureList.InsertString(m_TextureList.GetCount(), str);
	}	
}


void TMapCtrl::OnLbnSelchangeList1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString name;
	int iSelect = m_TextureList.GetCurSel();
	m_TextureList.GetText(iSelect, name);

	CTKGCAToolApp* pApp = (CTKGCAToolApp*)AfxGetApp();
	T_STR filepath = L"D:/00_KGCA41Leture/data/map/";
	filepath += name.GetBuffer();
	TTexture* pTex = I_Tex.Load(filepath);
	if (pTex)
	{
		pApp->m_Sample.m_pTitle->m_pMap->m_pTextureSRV = pTex->m_pTextureSRV;
	}
}


void TMapCtrl::Recurse(LPCTSTR pstr)
{
	// 폴더 읽기
	CFileFind cfile;
	CString strFileName = _T("");
	CString strFilePath(pstr);
	strFilePath += _T("/*.*");
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
			m_objList.push_back(strName);
		}
	}
	cfile.Close();
}