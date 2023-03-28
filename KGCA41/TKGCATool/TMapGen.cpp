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
	, m_iDepthBias(0)
	, m_fDepthBiasClamp(0)
	, m_fSlopeScaledDepthBias(0)
	, m_fCameraNear(0)
	, m_fCameraFar(0)
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
	DDX_Control(pDX, IDC_PICTURE_TEXTURE, m_PictureTextureCtrl);
	DDX_Text(pDX, IDC_DEPHBIAS, m_iDepthBias);
	DDX_Text(pDX, IDC_DEPTHBIASCLAMP, m_fDepthBiasClamp);
	DDX_Text(pDX, IDC_SLOPESCALEDDEPTHBIAS, m_fSlopeScaledDepthBias);
	DDX_Text(pDX, IDC_CAMERANEAR, m_fCameraNear);
	DDX_Text(pDX, IDC_CAMERAFAR, m_fCameraFar);
	DDX_Control(pDX, IDC_COMBO1, m_CBSplattingTexturfe);
}

BEGIN_MESSAGE_MAP(TMapGen, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &TMapGen::OnBnClickedButton1)
	ON_LBN_SELCHANGE(IDC_LIST1, &TMapGen::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_WIREFRAME, &TMapGen::OnBnClickedWireframe)
	ON_LBN_SELCHANGE(IDC_LIST2, &TMapGen::OnLbnSelchangeObject)
	ON_BN_CLICKED(IDC_BUTTON2, &TMapGen::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_LANDSCAPE_UP, &TMapGen::OnBnClickedLandscapeUp)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_LANDSCAPE_DOWN, &TMapGen::OnBnClickedLandscapeDown)
	ON_BN_CLICKED(IDC_LANDSCAPE_PLAT, &TMapGen::OnBnClickedLandscapePlat)
	ON_EN_CHANGE(IDC_DEPHBIAS, &TMapGen::OnEnChangeDephbias)
	ON_EN_CHANGE(IDC_DEPTHBIASCLAMP, &TMapGen::OnEnChangeDepthbiasclamp)
	ON_EN_CHANGE(IDC_SLOPESCALEDDEPTHBIAS, &TMapGen::OnEnChangeSlopescaleddepthbias)
	ON_EN_CHANGE(IDC_CAMERANEAR, &TMapGen::OnEnChangeCameranear)
	ON_EN_CHANGE(IDC_CAMERAFAR, &TMapGen::OnEnChangeCamerafar)
	ON_BN_CLICKED(IDC_SPLATTING, &TMapGen::OnBnClickedSplatting)
	ON_CBN_SELCHANGE(IDC_COMBO1, &TMapGen::OnCbnSelchangeCombo1)
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
	T_STR filepath = L"../../data/map/";
	filepath += name.GetBuffer();
	TTexture* pTex = I_Tex.Load(filepath);
	if (pTex && pApp->m_Sample.m_pTitle && pApp->m_Sample.m_pTitle->m_pMap)
	{
		pApp->m_Sample.m_pTitle->m_pMap->m_pTextureSRV = pTex->m_pTextureSRV;
	}


	m_img.Destroy();
	HRESULT hr = m_img.Load(filepath.c_str());
	if (FAILED(hr))	{}
		
	RedrawWindow();
}


void TMapGen::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	m_iCols = 65;
	m_iRows = 65;
	m_bWireFrame = FALSE;
	m_iDepthBias = 35000;
	m_fDepthBiasClamp = 0.0f;
	m_fSlopeScaledDepthBias = 3.0f;
	m_fCameraNear = 0.1f;
	m_fCameraFar = 1000.0f;

	m_CBSplattingTexturfe.InsertString(0, L"017.bmp");
	m_CBSplattingTexturfe.InsertString(1, L"037.bmp");
	m_CBSplattingTexturfe.InsertString(2, L"036.jpg");
	m_CBSplattingTexturfe.InsertString(3, L"026.jpg");
	m_CBSplattingTexturfe.SetCurSel(0);
	UpdateData(FALSE);

	Recurse(L"../../data/map", L"/*.*", m_TextureNameList);
	for (T_STR data : m_TextureNameList)
	{
		CString str(data.c_str());
		m_TextureList.InsertString(m_TextureList.GetCount(), str);
	}

	Recurse(L"../../data/fbx", L"/*.fbx", m_ObjectNameList);
	for (T_STR data : m_ObjectNameList)
	{
		CString str(data.c_str());
		m_ObjectList.InsertString(m_ObjectList.GetCount(), str);
	}

	

	T_STR filepath = L"../../data/map/";
	filepath += L"000.jpg";
	m_img.Load(filepath.c_str());
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
	
	T_STR filepath = L"../../data/fbx/";
	filepath += m_strSelectObject.GetBuffer();
	pApp->m_Sample.m_szSelectFbxFile = filepath;
	
	pApp->m_Sample.m_bObjectPicking = true;
	pApp->m_Sample.m_bUpPicking = false;
	pApp->m_Sample.m_bDownPicking = false;
	pApp->m_Sample.m_bPlatPicking = false;	
	pApp->m_Sample.m_bSplatting = false;
}

void TMapGen::OnBnClickedLandscapeUp()
{
	CTKGCAToolApp* pApp = (CTKGCAToolApp*)AfxGetApp();
	pApp->m_Sample.m_bUpPicking = true;
	pApp->m_Sample.m_bDownPicking = false;
	pApp->m_Sample.m_bPlatPicking = false;
	pApp->m_Sample.m_bObjectPicking = false;
	pApp->m_Sample.m_bSplatting = false;
}


void TMapGen::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
}


void TMapGen::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CFormView::OnPaint()을(를) 호출하지 마십시오.
	CRect rect;
	CRect rect2;
	m_PictureTextureCtrl.GetWindowRect(rect);
	ScreenToClient(rect);
	::SetStretchBltMode(dc, COLORONCOLOR);  // set iMode.
	if (m_img.IsNull()==false)
	{
		//int width = m_img.GetWidth();
		//int height = m_img.GetHeight();
		//m_PictureTextureCtrl.MoveWindow(rect.left, rect.top, width,height);
		//m_PictureTextureCtrl.GetWindowRect(rect2);
		//ScreenToClient(rect2);
		m_img.StretchBlt(dc, rect,SRCCOPY);
	}
}


void TMapGen::OnBnClickedLandscapeDown()
{
	CTKGCAToolApp* pApp = (CTKGCAToolApp*)AfxGetApp();
	pApp->m_Sample.m_bUpPicking = false;
	pApp->m_Sample.m_bDownPicking = true;
	pApp->m_Sample.m_bPlatPicking = false;
	pApp->m_Sample.m_bObjectPicking = false;
	pApp->m_Sample.m_bSplatting = false;
}


void TMapGen::OnBnClickedLandscapePlat()
{
	CTKGCAToolApp* pApp = (CTKGCAToolApp*)AfxGetApp();
	pApp->m_Sample.m_bUpPicking = false;
	pApp->m_Sample.m_bDownPicking = false;
	pApp->m_Sample.m_bPlatPicking = true;
	pApp->m_Sample.m_bObjectPicking = false;
	pApp->m_Sample.m_bSplatting = false;
}


void TMapGen::OnEnChangeDephbias()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CTKGCAToolApp* pApp = (CTKGCAToolApp*)AfxGetApp();
	UpdateData(TRUE);
	pApp->m_Sample.m_SlopeScaledDepthBiasDesc.DepthBias = m_iDepthBias;
}


void TMapGen::OnEnChangeDepthbiasclamp()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CTKGCAToolApp* pApp = (CTKGCAToolApp*)AfxGetApp();
	UpdateData(TRUE);
	pApp->m_Sample.m_SlopeScaledDepthBiasDesc.DepthBiasClamp = m_fDepthBiasClamp;
}


void TMapGen::OnEnChangeSlopescaleddepthbias()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CTKGCAToolApp* pApp = (CTKGCAToolApp*)AfxGetApp();
	UpdateData(TRUE);
	pApp->m_Sample.m_SlopeScaledDepthBiasDesc.SlopeScaledDepthBias = m_fSlopeScaledDepthBias;
}


void TMapGen::OnEnChangeCameranear()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CTKGCAToolApp* pApp = (CTKGCAToolApp*)AfxGetApp();
	UpdateData(TRUE);
	pApp->m_Sample.m_fDepthMapCameraNear = m_fCameraNear;	
	pApp->m_Sample.SetShadowProjectionDistance();
}


void TMapGen::OnEnChangeCamerafar()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CTKGCAToolApp* pApp = (CTKGCAToolApp*)AfxGetApp();
	UpdateData(TRUE);
	pApp->m_Sample.m_fDepthMapCameraFar = m_fCameraFar;
	pApp->m_Sample.SetShadowProjectionDistance();
}


void TMapGen::OnBnClickedSplatting()
{
	CTKGCAToolApp* pApp = (CTKGCAToolApp*)AfxGetApp();
	pApp->m_Sample.m_bUpPicking = false;
	pApp->m_Sample.m_bDownPicking = false;
	pApp->m_Sample.m_bPlatPicking = false;
	pApp->m_Sample.m_bObjectPicking = false;
	pApp->m_Sample.m_bSplatting = true;
}


void TMapGen::OnCbnSelchangeCombo1()
{
	UINT iIndex = m_CBSplattingTexturfe.GetCurSel();
	CString name;
	m_CBSplattingTexturfe.GetLBText(iIndex, name);

	CTKGCAToolApp* pApp = (CTKGCAToolApp*)AfxGetApp();
	pApp->m_Sample.m_iSplattingTexIndex = iIndex;
}
