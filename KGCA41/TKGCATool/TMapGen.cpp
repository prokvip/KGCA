// TMapGen.cpp: 구현 파일
//

#include "pch.h"
#include "TKGCATool.h"
#include "TMapGen.h"


// TMapGen

IMPLEMENT_DYNCREATE(TMapGen, CFormView)

TMapGen::TMapGen()
	: CFormView(IDD_DIALOG1)
	, m_iCols(0)
	, m_iRows(0)
	, m_bWireFrame(FALSE)
	, m_strSelectObject(_T(""))
{

}

TMapGen::~TMapGen()
{
}

void TMapGen::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_iCols);
	DDX_Text(pDX, IDC_EDIT3, m_iRows);
	DDX_Check(pDX, IDC_WIREFRAME, m_bWireFrame);
	DDX_Control(pDX, IDC_LIST1, m_TextureList);
	DDX_Text(pDX, IDC_EDIT2, m_strSelectObject);
	DDX_Control(pDX, IDC_LIST2, m_ObjectList);
}

BEGIN_MESSAGE_MAP(TMapGen, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &TMapGen::OnBnClickedButton1)
	ON_LBN_SELCHANGE(IDC_LIST1, &TMapGen::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_WIREFRAME, &TMapGen::OnBnClickedWireframe)
	ON_LBN_SELCHANGE(IDC_LIST2, &TMapGen::OnLbnSelchangeObject)
	ON_BN_CLICKED(IDC_BUTTON2, &TMapGen::OnBnClickedButton2)
END_MESSAGE_MAP()


// TMapGen 진단
TMapGen* TMapGen::CreateFormView(CWnd* pParent)
{
	TMapGen* pForm = new TMapGen;
	pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 300, 300), pParent, 0, NULL);
	return pForm;
}
void TMapGen::Recurse(LPCTSTR pFilePath, LPCTSTR ext, std::vector<T_STR>& list)
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
#ifdef _DEBUG
void TMapGen::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void TMapGen::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// TMapGen 메시지 처리기


void TMapGen::OnBnClickedButton1()
{
	CTKGCAToolApp* pApp = (CTKGCAToolApp*)AfxGetApp();
	UpdateData(TRUE);
	pApp->m_Sample.CreateMapData(m_iCols, m_iRows);
}


void TMapGen::OnLbnSelchangeList1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString name;
	int iSelect = m_TextureList.GetCurSel();
	m_TextureList.GetText(iSelect, name);

	CTKGCAToolApp* pApp = (CTKGCAToolApp*)AfxGetApp();
	T_STR filepath = L"D:/00_KGCA41Leture/data/map/";
	filepath += name.GetBuffer();
	TTexture* pTex = I_Tex.Load(filepath);
	if (pTex && pApp->m_Sample.m_pTitle && pApp->m_Sample.m_pTitle->m_pMap)
	{
		pApp->m_Sample.m_pTitle->m_pMap->m_pTextureSRV = pTex->m_pTextureSRV;
	}
}


void TMapGen::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	m_iCols = 65;
	m_iRows = 65;
	m_bWireFrame = FALSE;

	UpdateData(FALSE);

	Recurse(L"D:/00_KGCA41Leture/data/map", L"/*.*", m_TextureNameList);
	for (T_STR data : m_TextureNameList)
	{
		CString str(data.c_str());
		m_TextureList.InsertString(m_TextureList.GetCount(), str);
	}

	Recurse(L"D:/00_KGCA41Leture/data/fbx", L"/*.fbx", m_ObjectNameList);
	for (T_STR data : m_ObjectNameList)
	{
		CString str(data.c_str());
		m_ObjectList.InsertString(m_ObjectList.GetCount(), str);
	}
}


void TMapGen::OnFinalRelease()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CFormView::OnFinalRelease();
}


void TMapGen::OnBnClickedWireframe()
{
	CTKGCAToolApp* pApp = (CTKGCAToolApp*)AfxGetApp();

	UpdateData(TRUE);
	pApp->m_Sample.m_bWireFrame = m_bWireFrame;
}



void TMapGen::OnLbnSelchangeObject()
{
	CString name;
	int iSelect = m_ObjectList.GetCurSel();
	m_ObjectList.GetText(iSelect, name);	
	m_strSelectObject = name.GetBuffer();
	UpdateData(FALSE);	
}


void TMapGen::OnBnClickedButton2()
{
	CTKGCAToolApp* pApp = (CTKGCAToolApp*)AfxGetApp();
	pApp->m_Sample.m_bPicking = true;
	T_STR filepath = L"D:/00_KGCA41Leture/data/fbx/";
	filepath += m_strSelectObject.GetBuffer();
	pApp->m_Sample.m_szSelectFbxFile = filepath;
	
}
