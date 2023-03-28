#include "Sample.h"
#include "TDxStateUI.h"
int		Sample::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
	return -1;
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
	m_Dlg->Create(m_pd3dDevice, m_pImmediateContext, shaderfilename, L"../../data/ui/popup_normal.png");
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
	m_listControl->Create(m_pd3dDevice, m_pImmediateContext, shaderfilename, L"../../data/ui/RAINBOW.png");
	m_listControl->SetAttribute({ 20, 20,	100.0f, 250.0f });
	UINT iNumBtn = 10;
	TUI::TRect rtMargin = { 3.0f, 3.0f, 3.0f, 3.0f };
	for (int iBtn = 0; iBtn < iNumBtn; iBtn++)
	{
		TUI::TWidget* btn = new TUI::TButton;
		btn->Create(m_pd3dDevice, m_pImmediateContext, shaderfilename, L"../../data/ui/main_plus_dis.png");
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
	CreateWidget();
	CreateDlg();
	return true;
}
bool Sample::Frame()
{
	for (auto widget : m_pWidgets)
	{
		if (widget->Frame())
		{
			m_DrawWidgets.push(widget);
		}
	}
	return true;
}
bool Sample::Render()
{	
	while (!m_DrawWidgets.empty())
	{
		TUI::TWidget* widget = m_DrawWidgets.top();
		widget->Render();
		m_DrawWidgets.pop();
	}
	
	/*m_pListBox->Render();
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
	return true;
}
//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
HRESULT Sample::CreateResource()
{
	HRESULT hr = S_OK;
	return hr;
}
//--------------------------------------------------------------------------------------
// 
//--------------------------------------------------------------------------------------
HRESULT Sample::DeleteResource()
{
	HRESULT hr= S_OK;	
	return hr;
}
Sample::Sample(void)
{
}
Sample::~Sample(void)
{
}

TBASIS_RUN(L"TBasisSample DrawShape");
