/**
@file      TBasisLib.cpp
@date    2017/12/2
@author prokvip

RapidEngine™
@brief    TBasisLib  소스파일.

게임 플레이 기본 코어 인터페이스.\n
게임 플레이 기본 기능 제공.\n
모든 컨포넌트가 연결되어 있다.
*/
#include "TBasisLib.h"
#include "TRegistry.h"
#include <DbgHelp.h>

LONG WINAPI ExceptionFilter(EXCEPTION_POINTERS* exceptionInfo)
{
	if (IsDebuggerPresent())
		return EXCEPTION_CONTINUE_SEARCH;

	HANDLE hFile = CreateFileA("EasyServer_minidump.dmp", GENERIC_READ | GENERIC_WRITE,
		0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if ((hFile != NULL) && (hFile != INVALID_HANDLE_VALUE))
	{
		MINIDUMP_EXCEPTION_INFORMATION mdei;

		mdei.ThreadId = GetCurrentThreadId();
		mdei.ExceptionPointers = exceptionInfo;
		mdei.ClientPointers = FALSE;


		MINIDUMP_TYPE mdt = (MINIDUMP_TYPE)(MiniDumpWithPrivateReadWriteMemory |
			MiniDumpWithDataSegs | MiniDumpWithHandleData | MiniDumpWithFullMemoryInfo |
			MiniDumpWithThreadInfo | MiniDumpWithUnloadedModules);

		MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
			hFile, mdt, (exceptionInfo != 0) ? &mdei : 0, 0, NULL);

		CloseHandle(hFile);

	}
	else
	{
		printf("CreateFile failed. Error: %u \n", GetLastError());
	}
	return EXCEPTION_EXECUTE_HANDLER;
}

std::wstring	g_szTBasisSamplePath;
std::wstring	g_szDefaultDataPath;

T_STR RasterizerState[] =
{
	_T("g_pRSBackCullSolid"),
	_T("g_pRSNoneCullSolid"),
	_T("g_pRSFrontCullSolid"),
};

T_STR SamplerState[] =
{
	_T("g_pSSWrapLinear"),
	_T("g_pSSWrapPoint"),
	_T("g_pSSMirrorLinear"),
	_T("g_pSSMirrorPoint"),
	_T("g_pSSClampLinear"),
	_T("g_pSSClampPoint"),
};
/**
@return찾은 그룹  (없으면 NULL)
@warning 외부에서 관련된 동기화 객체를 Lock을 걸어 사용한다.
*/
int PrintError(unsigned int line, HRESULT hr)
{
	wprintf_s(L"ERROR: Line:%d HRESULT: 0x%X\n", line, hr);
	return hr;
}
static BOOL GetProcAddresses(HMODULE* hLibrary,	LPCWSTR lpszLibrary,	INT nCount, ...)
{
	BOOL bRet = TRUE;
	va_list va;
	va_start(va, nCount);
	if ((*hLibrary = LoadLibrary(lpszLibrary))
		!= NULL)
	{
		FARPROC* lpfProcFunction = NULL;
		LPCSTR lpszFuncName = NULL;
		INT nIdxCount = 0;
		while (nIdxCount < nCount)
		{
			lpfProcFunction = va_arg(va, FARPROC*);
			lpszFuncName = va_arg(va, LPCSTR);
			if ((*lpfProcFunction = GetProcAddress(*hLibrary, lpszFuncName)) == NULL)
			{
				lpfProcFunction = NULL;
				bRet = FALSE;
				WCHAR wstrDestination[MAX_PATH] = { 0, };
				DWORD cchDestChar = strlen(lpszFuncName);
				int nResult = MultiByteToWideChar(CP_ACP, 0, lpszFuncName, -1,
					wstrDestination, cchDestChar);

				wstrDestination[cchDestChar - 1] = 0;
				MessageBox(0, wstrDestination, _T("Loading Fail"), MB_OK);
				break;
			}
			nIdxCount++;
		}
	}
	else
	{
		bRet = FALSE;
	}
	va_end(va);
	if (bRet == FALSE && *hLibrary != NULL)
		FreeLibrary(*hLibrary);
	return bRet;
}
bool TBasisLib::DllLoads()
{
	// Basis Load
	typedef TUI::TBasisUI* ( CALLBACK *TBASISUI_PTR )( );
	TBASISUI_PTR lpfTBasisUI = NULL;
#ifdef _DEBUG
	if( !GetProcAddresses(	&m_hLibraryUI, _T("TBasisUI_D.dll"), 1, &lpfTBasisUI, "GetTBasisUIPtr") )
	{
		return 0;
	}
#else
	if (!GetProcAddresses(&m_hLibraryUI, _T("TBasisUI_R.dll"), 1, &lpfTBasisUI, "GetTBasisUIPtr"))
	{
		return 0;
	}
#endif
	m_pTBasisUI = (lpfTBasisUI)();
	return true;
}

int TBasisLib::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	return -1;
}
bool TBasisLib::PreInit()
{
	SetUnhandledExceptionFilter(ExceptionFilter);
	DllLoads();

	TLog::Get().log("TBasisLib::PreInit()");
	TLog::Get().log("InitDevice");
	if( FAILED( InitDevice(m_hWnd,this->m_iWindowWidth, this->m_iWindowHeight) ) )
	{
		MessageBox( 0, _T("CreateDevice  실패"), _T("Fatal error"), MB_OK );
		return false;
	}
	
	if (m_pTBasisUI != nullptr)
	{
		m_pTBasisUI->Set(m_hWnd, m_pd3dDevice, m_pImmediateContext, m_pSwapChain);
		m_pTBasisUI->Init();
	}

	HRESULT hr=S_OK;
	if (FAILED(hr = CreateDxResource()))
	{
		return false;
	}

	TLog::Get().log("m_Timer.Init()");
	if( !m_Timer.Init() )	return false;	

	TLog::Get().log("DirectX Input 초기화");
	if( !I_Input.Init() )
	{
		return false;
	}

	if (FAILED(m_AxisLine.Create(GetDevice(), Res(L"shader/Line.hlsl").c_str())))
	{
		MessageBox(0, _T("m_pDirectionLIne 실패"), _T("Fatal error"), MB_OK);
		return 0;
	}

	I_Texture.Set(m_pd3dDevice, m_pImmediateContext);
	return true;
}
bool TBasisLib::Init()
{
	return true;
}
bool TBasisLib::PostInit()
{
	return true;
}
bool TBasisLib::TInit()
{	
	if( !PreInit() ) return false;
	if( !Init() ) return false;
	if( !PostInit() ) return false;
	return true;
}
bool TBasisLib::ToolInit(HWND hWnd, HINSTANCE hInstance)
{
	g_hWnd = m_hWnd = hWnd;
	m_hInstance = hInstance;
	GetWindowRect(m_hWnd, &m_rcWindowBounds);
	GetClientRect(m_hWnd, &m_rcWindowClient);
	g_rtClient = m_rcWindowClient;
	return TInit();
}
bool TBasisLib::PreFrame()
{
	if( !m_Timer.Frame() ) return false;
	if( !I_Input.Frame() ) return false;	
	if (m_bUseEngineStateCtl && !Update(m_pImmediateContext)) return false;
	return true;
}
bool TBasisLib::Update(ID3D11DeviceContext*    pContext)
{
	if (I_Input.KeyCheck(DIK_1) == KEY_UP)
	{
		m_bWireFrameRender = !m_bWireFrameRender;
	}
	if (I_Input.KeyCheck(DIK_2) == KEY_UP)
	{
		++m_iPrimitiveType;
		if (m_iPrimitiveType > 5)
		{
			m_iPrimitiveType = 1;
		}
		m_iPrimitiveType = min(m_iPrimitiveType, 5);
	}
	if (I_Input.KeyCheck(DIK_3) == KEY_UP)
	{
		++m_iCullMode;
		if (m_iCullMode > 3)
		{
			m_iCullMode = 1;
		}
		m_iCullMode = min(m_iCullMode, 3);
		m_bWireFrameRender = false;
	}

	if (I_Input.KeyCheck(DIK_4) == KEY_UP)
	{
		++m_iSamplerMode;
		if (m_iSamplerMode >= 6)
		{
			m_iSamplerMode = 0;
		}
	}

	if (m_bWireFrameRender)
	{
		ApplyRS(pContext, TDxState::g_pRSWireFrame);
	}
	else
	{
		ApplyRS(pContext, TDxState::g_pRS[m_iCullMode - 1]);
	}
	ApplySS(pContext, TDxState::g_pSS[m_iSamplerMode]);
	pContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)m_iPrimitiveType);
	return true;
}
bool TBasisLib::PostFrame()
{
	return true;
}
bool TBasisLib::Frame()
{
	return true;
}
bool TBasisLib::Release()
{
	return true;
}
bool TBasisLib::TRelease()
{
	Release();	
	if( !m_Timer.Release() ) return false;		
	if( !m_Font.Release() ) return false;	
	if( !I_Input.Release() ) return false;	
	m_AxisLine.Release();
	I_Texture.Release();
	TDxState::Release();
	CleanupDevice();

	if (m_pTBasisUI != nullptr)
	{
		m_pTBasisUI->Release();
		delete m_pTBasisUI;
		m_pTBasisUI = nullptr;
	}
	FreeLibrary(m_hLibraryUI);

	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtDumpMemoryLeaks();
	return true;
}
bool TBasisLib::TFrame()
{
	if (I_Input.KeyCheck(DIK_V) == KEY_UP)
	{
		m_bDebugPrint = !m_bDebugPrint;
	}
	if (I_Input.KeyCheck(DIK_GRAVE) == KEY_UP)
	{
		m_bUseEngineStateCtl = !m_bUseEngineStateCtl;
	}
	if( m_pTBasisUI!=nullptr)	m_pTBasisUI->Frame(g_fDurationTime, g_fSecPerFrame);
	PreFrame();
	Frame();
	PostFrame();
	return true;
}
bool TBasisLib::TRender()
{
	if (m_pImmediateContext == nullptr) return true;

	I_Input.Render();
	m_Timer.Render();
	PreRender();
	Render();	
	if( m_bDebugPrint )	DrawDebug();
	if (m_pTBasisUI != nullptr)	m_pTBasisUI->Render(g_fDurationTime, g_fSecPerFrame);
	PostRender();	
	return true;
}

bool TBasisLib::Render()
{
	return true;
}
bool TBasisLib::PreRender()
{	
	// Just clear the backbuffer
    float ClearColor[4] = { 0.4f, 0.4f, 0.4f, 1.0f }; //red,green,blue,alpha
	m_pImmediateContext->ClearRenderTargetView( GetRenderTargetView(), ClearColor );	
	m_pImmediateContext->ClearDepthStencilView(m_DefaultRT.m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_pImmediateContext->OMSetRenderTargets(1, GetRenderTargetViewAddress(), m_DefaultRT.m_pDepthStencilView.Get() );
	m_pImmediateContext->RSSetViewports(1, &m_DefaultRT.m_vp);
	DX::ApplyDSS(m_pImmediateContext, DX::TDxState::g_pDSSDepthEnable);
	DX::ApplyBS(m_pImmediateContext, DX::TDxState::g_pAlphaBlend);
	return true;
}
bool TBasisLib::DrawDebug()
{
	if (m_pGameCamera != nullptr)
	{
		m_AxisLine.SetMatrix(NULL, &m_pGameCamera->m_matView, &m_pGameCamera->m_matProj);
		m_AxisLine.Render(GetContext());
	}
	// FPS 출력
	TCHAR pBuffer[256];
	memset( pBuffer, 0, sizeof( TCHAR ) * 256 );
	_stprintf_s( pBuffer, _T("FPS:%d"), m_Timer.GetFPS() );	
	
	m_Font.Begin();
	m_Font.m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	m_Font.m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
	RECT rc1 = { m_iWindowWidth/2,0, (LONG)m_iWindowWidth, (LONG)m_iWindowHeight};
	m_Font.DrawText( rc1, pBuffer,  D2D1::ColorF(1.0f, 1.0f, 1.0f,0.5));		
	m_Font.End();

	////-----------------------------------------------------------------------
	//// 적용되어 RasterizerState 타입 표시
	////-----------------------------------------------------------------------	
	//RECT rc;
	//rc.top = 30;
	//rc.bottom = 600;
	//rc.left = 0;
	//rc.right = 800;
	//T_STR str = RasterizerState[m_iCullMode];
	//DrawDebugRect(&rc, const_cast<TCHAR*>(str.c_str()), TColor(0.0f, 0.0f, 1.0f, 1.0f));

	////-----------------------------------------------------------------------
	//// 적용되어 SamplerState 타입 표시
	////-----------------------------------------------------------------------	
	//rc.top = 50;
	//rc.bottom = 600;
	//rc.left = 0;
	//rc.right = 800;
	//str = SamplerState[m_iSamplerMode];
	//DrawDebugRect(&rc, const_cast<TCHAR*>(str.c_str()), TColor(0.0f, 0.0f, 1.0f, 1.0f));

	return true;
}

bool TBasisLib::DrawDebugRect(RECT* rcDest, const TCHAR* pString, TBASIS_EX::TColor color )
{
	if( rcDest == NULL ) return false;	

	if( m_Font.m_pTextFormat)
	{
		D2D1_SIZE_F rtSize = m_Font.m_pRT->GetSize ();
        //Draw a grid background.
        int width = static_cast <int> (rtSize.width);
        int height = static_cast <int> (rtSize.height);

		m_Font.Begin();
		m_Font.m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		m_Font.m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		//RECT rc1 = {0,0, m_iWindowWidth, m_iWindowHeight};
		m_Font.DrawText( *rcDest, const_cast<TCHAR*>(pString),  D2D1::ColorF(color.x,color.y,color.z,0.5f));		
		m_Font.End();
	}

	return true;
}
bool TBasisLib::PostRender()
{	
	HRESULT hr;	
	if (FAILED(hr = GetSwapChain()->Present(0, 0)))
	{
		H(hr);
	}
	return true;
}
bool TBasisLib::ToolRun()
{
	TFrame();
    TRender();        	
	return true;
}
bool TBasisLib::Run()
{
	//CreateWICTextureFromFile의 초기화 처리에 사용된다.
	//호출 스레드를 사용하는 COM library를 초기화 하고, 스레드의 동시성 모델을 설정하고, 필요한 경우 스레드의 apartment를 생성한다.
	//아파트먼트(Apartment) = > 아파트먼트는 같은 스레딩 모델을 공유하는 객체들이 존재하는 곳
#if (_WIN32_WINNT >= 0x0A00 /*_WIN32_WINNT_WIN10*/)
	Microsoft::WRL::Wrappers::RoInitializeWrapper initialize(RO_INIT_MULTITHREADED);
	if (FAILED(initialize))//Windows::Foundation::Initialize호출되며 해제자에서 Windows::Foundation::Uninitialize호출된다.
	{
		PrintError(__LINE__, initialize);
	}
#else
	HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
	if (FAILED(hr))
	{
		PrintError(__LINE__, initialize);
	}
#endif

	I_Input.m_hWnd = m_hWnd;
	if( !TInit() ) return false;
	// Main message loop
    MSG msg = {0};
    while( WM_QUIT != msg.message )
    {
        if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else
        {
			TFrame();
            TRender();
        }
    }
	if( !TRelease() ) return false;
#if (_WIN32_WINNT >= 0x0A00 /*_WIN32_WINNT_WIN10*/)

#else
	CoUninitialize();
#endif
	//Windows::Foundation::Uninitialize();

	//MemoryReporting();
	return true;
}
HRESULT TBasisLib::CreateDxResource()
{
	IDXGISurface1*		pBackBuffer=NULL;
	HRESULT hr = GetSwapChain()->GetBuffer(0, __uuidof(IDXGISurface), (LPVOID*)&pBackBuffer);	
		m_Font.Set(m_hWnd, m_iWindowWidth,	m_iWindowHeight, pBackBuffer );		
		if(m_pTBasisUI) m_pTBasisUI->CreateDevice(pBackBuffer);
	if( pBackBuffer )	pBackBuffer->Release();

	if (FAILED(hr = GetSwapChain()->GetDesc(&m_SwapChainDesc)))
	{
		return hr;
	}	

	if (FAILED(hr = m_DefaultRT.UpdateDepthStencilView(m_pd3dDevice, m_SwapChainDesc.BufferDesc.Width, m_SwapChainDesc.BufferDesc.Height)))
	{
		return hr;
	}
	return CreateResource();
}
HRESULT TBasisLib::DeleteDxResource()
{
	m_Font.Release();	
	if (m_pTBasisUI) m_pTBasisUI->ResetDevice();
	return DeleteResource();
}
HRESULT TBasisLib::CreateResource()
{
	return S_OK;
}
HRESULT TBasisLib::DeleteResource()
{
	return S_OK;
}
TBasisLib::TBasisLib(void)
{	
	m_iPrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_iCullMode = 1;
	m_iSamplerMode = 0;
	m_bWireFrameRender = false;
	m_bDebugPrint = true;
	m_bUseEngineStateCtl = true;
	m_pGameCamera = nullptr;

	TCHAR tok[] = L"\\";

	WCHAR buffer[256] = { 0, };
	GetCurrentDirectory(256, buffer);


	T_STR strSaveDefaultDataDir;
	T_STR strDataDir;
	TCHAR* pszNextToken = 0;
	TCHAR* temp = _tcstok_s(buffer, tok, &pszNextToken);
	
	if (temp != NULL)
	{
		strDataDir += temp;
		strDataDir += L"\\";
		for (UINT iSize = 0;
			temp = _tcstok_s(0, tok, &pszNextToken);
			iSize++)
		{
			strDataDir += temp;
			strDataDir += L"\\";
			T_STR dataDir = strDataDir + L"data";
			SHFILEINFO shFileInfo;
			memset(&shFileInfo, 0, sizeof(shFileInfo));  
			if (SHGetFileInfo((LPWSTR)dataDir.c_str(), 0, &shFileInfo, sizeof(SHFILEINFO), SHGFI_TYPENAME) != 0)
			{
				strSaveDefaultDataDir = dataDir;
				break;
			}						
		}
	}

	// administrator로 환경변수 등록한다.
	if (!strSaveDefaultDataDir.empty())
	{
		strSaveDefaultDataDir += L"/";
		if (I_Reg.SystemKeyAdd(L"TBASIS_SAMPLES_DATA", strSaveDefaultDataDir.c_str()))
		{
			DWORD cbDest = MAX_PATH * sizeof(TCHAR);
				I_Reg.QueryValue(m_szDefaultDataPath, L"TBASIS_SAMPLES_DATA", &cbDest);
				g_szDefaultDataPath = m_szDefaultDataPath;
		}
		else
		{
			g_szDefaultDataPath = strSaveDefaultDataDir;
		}
	}
	
	// 시스템->고급시스템 설정->환경변수 직접 등록 이후 조회 가능( 주의 : 없으면 에러처리됨. )
	//g_szTBasisSamplePath = _wgetenv(L"TBASIS_SAMPLES");
	//g_szDefaultDataPath = _wgetenv(L"TBASIS_SAMPLES_DATA");
	 
	////시스템 환경변수 패스 등록
	//I_Reg.SystemPathAdd("D:\\KGCA");		

	////프로세스에서 만 설정 가능한 환경변수 등록 API
		//::SetEnvironmentVariable(L"KGCA_DIR", L"d:\\kgca");
		//int nSize = ::GetEnvironmentVariable(L"KGCA_DIR", NULL, NULL);
		//TCHAR* buffer = new TCHAR[nSize + 1];
		//::GetEnvironmentVariable(L"KGCA_DIR", buffer, nSize);
		////::SetEnvironmentVariable(L"KGCA_DIR", NULL); // delete	

	////프로세스에서 만 설정 가능한 환경변수 등록 C언어
		//WCHAR szpath[256] = L"KGCA_RES_DIR=D:\\00_TBasisSampels2022\\";
		//_wputenv(szpath);
		//std::wstring path = _wgetenv(L"KGCA_RES_DIR");

	//// 레지스터리 설정 클래스 사용방법
		//I_Reg.Create(HKEY_CURRENT_USER, BASIS_APP_KEY);
		//TCHAR szDefaultDataPath[MAX_PATH] = { 0, };
		//DWORD cbDest = MAX_PATH * sizeof(TCHAR);
		//I_Reg.QueryValue(szDefaultDataPath, L"TCORE_PATH", &cbDest);
		//T_STR ptsh = szDefaultDataPath;
}

TBasisLib::~TBasisLib(void)
{
}
