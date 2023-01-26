#include "Sample.h"
#include "TDxStateUI.h"
static BOOL GetProcAddresses(HMODULE* hLibrary, LPCWSTR lpszLibrary, INT nCount, ...)
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

LRESULT		Sample::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (m_Dlg != nullptr)
	{
		m_Dlg->MsgProc(uMsg, wParam, lParam);
	}
	if (m_pEditBox != nullptr)
	{
		m_pEditBox->MsgProc(uMsg, wParam, lParam);
		m_pListBox->MsgProc(uMsg, wParam, lParam);
	}
	return TGameCore::MsgProc(hWnd, uMsg, wParam, lParam);
}
void Sample::CreateDlg()
{
	std::wstring shaderfilename = L"DefaultShape.txt";
	std::vector<TUI::W_STR> stateList;
	stateList.push_back(L"../../data/ui/main_start_nor.png");
	stateList.push_back(L"../../data/ui/main_start_sel.png");
	stateList.push_back(L"../../data/ui/main_start_pus.png");
	stateList.push_back(L"../../data/ui/main_start_dis.png");

	m_Dlg = std::make_shared<TUI::TDialog>();
	m_Dlg->Create(m_pd3dDevice.Get(), m_pImmediateContext.Get(), shaderfilename, L"../../data/ui/popup_normal.png");
	m_Dlg->SetAttribute({ 0, 0 ,400.0f, 300.0f });
	/////////////// position ///////
	//       left          right
	// u -> 0~0.1f  ~     0.9f~1.0f
	//        top          bottom
	// v -> 0~0.1f  ~     0.9f~1.0f
	/////////////// uv ///////
	//       left          right
	// u -> 0~0.2f  ~     0.8f~1.0f
	//        top          bottom
	// v -> 0~0.33f  ~     0.67f~1.0f	
	m_Dlg->SetDrawList(0.1f, 0.1f, 0.1f, 0.1f, 0.2f, 0.2f, 0.33f, 0.33f);

	TUI::TWidget* m_listControl = new TUI::TListControl;
	m_listControl->Create(m_pd3dDevice.Get(), m_pImmediateContext.Get(), shaderfilename, L"../../data/ui/RAINBOW.png");
	m_listControl->SetAttribute({ 20, 20,	100.0f, 250.0f });
	UINT iNumBtn = 10;
	TUI::TRect rtMargin = { 3.0f, 3.0f, 3.0f, 3.0f };
	for (int iBtn = 0; iBtn < iNumBtn; iBtn++)
	{
		TUI::TWidget* btn = new TUI::TButton;
		btn->Create(m_pd3dDevice.Get(), m_pImmediateContext.Get(), shaderfilename, L"../../data/ui/main_plus_dis.png");
		TUI::TRect rt = {	m_listControl->m_rtCollision.x1+ rtMargin.x1,
							m_listControl->m_rtCollision.y1 + iBtn * (m_listControl->m_rtCollision.h / iNumBtn)+ rtMargin.y1,
							m_listControl->m_rtInitPos.w - rtMargin.w*2.0f,
							m_listControl->m_rtInitPos.h / iNumBtn - rtMargin.h * 2.0f };
		btn->SetAttribute( rt, stateList);
		m_listControl->AddChild(btn);
	}

	TUI::TWidget* m_btnCheck = TUI::CreateCheckButton(m_pTBasisUI, TUI::I_Sprite.GetPtr(L"DXCheckButton"),
		{ m_listControl->m_rtCollision.x1 + 150,  m_listControl->m_rtCollision.y1 + 20, 50, 30 });

	TUI::TWidget* m_btnButton = TUI::CreateButton(m_pTBasisUI, TUI::I_Sprite.GetPtr(L"DXNormalButton"),
		{ m_listControl->m_rtCollision.x1 + 150, m_listControl->m_rtCollision.y1 + 60, 50, 30 });

	TUI::TWidget* m_pSlider = TUI::CreateSlider(m_pTBasisUI, TUI::I_Sprite.GetPtr(L"DXSlider"),
		{ m_listControl->m_rtCollision.x1 + 150, m_listControl->m_rtCollision.y1 + 100, 120, 30 });

	TUI::TWidget* m_pProgressBar = TUI::CreateProgressBar(m_pTBasisUI, TUI::I_Sprite.GetPtr(L"ProgressBar"),
		{ m_listControl->m_rtCollision.x1 + 150, m_listControl->m_rtCollision.y1 + 150, 200, 20 });

	m_Dlg->AddChild(m_btnCheck);
	m_Dlg->AddChild(m_btnButton);
	m_Dlg->AddChild(m_pSlider);
	m_Dlg->AddChild(m_listControl);
	m_Dlg->AddChild(m_pProgressBar);

	m_Dlg->SetDrawID(0);
	m_DrawWidgets.push(m_Dlg.get());
	m_pWidgets.push_back(m_Dlg.get());
}
void Sample::CreateWidget()
{
	////m_pText = std::make_shared<TUI::TText>();
	////m_pText->Create({ 300,0,100,50 }, L"Elephant", 20);
	m_pText.reset(TUI::CreateText(m_pTBasisUI, { 400,0,100,50 }, L"Elephant", 20, {0,0,0,1}));
	m_pText->SetPriority(2);
	m_pWidgets.push_back(m_pText.get());

	m_pComboBox.reset(TUI::CreateComboBox(m_pTBasisUI, TUI::I_Sprite.GetPtr(L"DXEditBox"), TUI::I_Writer.m_pTextFormat.Get(), { 400,  50, 200, 30 }));	
	m_pComboBox->Push(TUI::CreateComboBox(m_pTBasisUI, TUI::I_Sprite.GetPtr(L"DXEditBox"), TUI::I_Writer.m_pTextFormat.Get(), m_pComboBox->GetNextRect()));
	m_pComboBox->Push(TUI::CreateComboBox(m_pTBasisUI, TUI::I_Sprite.GetPtr(L"DXEditBox"), TUI::I_Writer.m_pTextFormat.Get(), m_pComboBox->GetNextRect()));
	m_pComboBox->SetPriority(10);
	m_pWidgets.push_back(m_pComboBox.get());
	
	m_pListBox.reset(TUI::CreateListBox(m_pTBasisUI, TUI::I_Sprite.GetPtr(L"DXListBox"), 10, 0, { 630,  50, 150, 300.0f }));
	m_pListBox->Push(L"aaa");
	m_pListBox->Push(TUI::CreateComboBox(m_pTBasisUI, TUI::I_Sprite.GetPtr(L"DXEditBox"), TUI::I_Writer.m_pTextFormat.Get(),m_pListBox->GetNextRect()));
	m_pListBox->Push(TUI::CreateButton(m_pTBasisUI,   TUI::I_Sprite.GetPtr(L"DXNormalButton"),m_pListBox->GetNextRect()));
	m_pListBox->Push(TUI::CreateEditBox(m_pTBasisUI, TUI::I_Sprite.GetPtr(L"DXEditBox"), TUI::I_Writer.m_pTextFormat.Get(), m_pListBox->GetNextRect()));
	m_pListBox->Push(L"bbb");
	m_pListBox->Push(L"ccc");
	m_pListBox->Push(L"ddd");
	m_pListBox->Push(L"eee");
	m_pListBox->Push(TUI::CreateSlider(m_pTBasisUI, TUI::I_Sprite.GetPtr(L"DXSlider"), m_pListBox->GetNextRect()));
	m_pListBox->Push(TUI::CreateText(m_pTBasisUI, { 400,0,100,30 }, L"Elephant", 10, { 1,0,0,1 }));
	m_pListBox->Push(TUI::CreateText(m_pTBasisUI, { 400,0,100,30 }, L"Elephant", 20, { 0,1,0,1 }));
	m_pListBox->Push(TUI::CreateText(m_pTBasisUI, { 400,0,100,30 }, L"Elephant", 30, { 0,0,1,1 }));
	m_pListBox->Push(TUI::CreateText(m_pTBasisUI, { 400,0,100,30 }, L"Elephant", 10, { 1,0,1,1 }));
	m_pListBox->Push(TUI::CreateText(m_pTBasisUI, { 400,0,100,30 }, L"Elephant", 10, { 0,1,1,1 }));
	m_pEditBox.reset(TUI::CreateEditBox(m_pTBasisUI, TUI::I_Sprite.GetPtr(L"DXEditBox"), TUI::I_Writer.m_pTextFormat.Get(), { 100,  550, 600, 40 }));	
	m_pListBox->SetPriority(3);
	m_pWidgets.push_back(m_pListBox.get());
	m_pEditBox->SetPriority(4);
	m_pWidgets.push_back(m_pEditBox.get());
}
bool Sample::Init()
{
	if (m_pTBasisUI != nullptr)
	{
		m_pTBasisUI->Set(m_hWnd, m_pd3dDevice.Get(), m_pImmediateContext.Get(), m_pSwapChain.Get());
		m_pTBasisUI->Init();
		IDXGISurface1* pBackBuffer = NULL;
		HRESULT hr = m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface), (LPVOID*)&pBackBuffer);
		if (m_pTBasisUI) m_pTBasisUI->CreateDevice(pBackBuffer);
		if (pBackBuffer)	pBackBuffer->Release();
	}

	CreateWidget();
	CreateDlg();
	return true;
}
bool Sample::Frame()
{
	if (m_pTBasisUI != nullptr)	m_pTBasisUI->Frame(g_fGameTimer,g_fSecondPerFrame);
	for (auto widget : m_pWidgets)
	{
		if (widget->Frame())
		{
			m_DrawWidgets.push(widget);
		}
	}
	return true;
}
bool Sample::UIRender()
{	
	m_pImmediateContext->OMSetDepthStencilState(TDxState::g_pDisableDepthStencil, 0xff);
	if (m_pTBasisUI != nullptr)	m_pTBasisUI->Render(g_fGameTimer,g_fSecondPerFrame );
	while (!m_DrawWidgets.empty())
	{
		TUI::TWidget* widget = m_DrawWidgets.top();
		widget->Render();
		m_DrawWidgets.pop();
	}
	/*m_pText->Render();
	m_pListBox->Render();
	m_pComboBox->Render();
	m_pText->Render();	
	m_pEditBox->Render();
	m_Dlg->Render();*/
	return true;
}
bool Sample::Release()
{
	for (auto widget : m_pWidgets)
	{
		widget->Release();
	}
	m_pEditBox.reset();
	m_pListBox.reset();
	m_pComboBox.reset();
	m_pText.reset();
	m_Dlg.reset();	

	if (m_pTBasisUI != nullptr)
	{
		m_pTBasisUI->Release();
		delete m_pTBasisUI;
		m_pTBasisUI = nullptr;
	}
	FreeLibrary(m_hLibraryUI);
	return true;
}
//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
HRESULT Sample::CreateDXResource()
{
	TGameCore::CreateDXResource();
	IDXGISurface1* pBackBuffer = NULL;
	HRESULT hr = m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface), (LPVOID*)&pBackBuffer);	
		if (m_pTBasisUI) m_pTBasisUI->CreateDevice(pBackBuffer);
	if (pBackBuffer)	pBackBuffer->Release();
	return hr;
}
//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
HRESULT Sample::DeleteDXResource()
{
	TGameCore::DeleteDXResource();
	HRESULT hr= S_OK;	
	if (m_pTBasisUI) m_pTBasisUI->ResetDevice();
	return hr;
}
Sample::Sample(void)
{
	DllLoads();
	
}
Sample::~Sample(void)
{
}
bool Sample::DllLoads()
{
	// Basis Load
	typedef TUI::TBasisUI* (CALLBACK* TBASISUI_PTR)();
	TBASISUI_PTR lpfTBasisUI = NULL;
#ifdef _DEBUG
	if (!GetProcAddresses(&m_hLibraryUI, _T("TBasisUI_D.dll"), 1, &lpfTBasisUI, "GetTBasisUIPtr"))
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

GAME_RUN(DrawWidget, 800, 600)
