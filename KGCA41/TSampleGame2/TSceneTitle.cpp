#include "TSceneTitle.h"
bool     TSceneTitle::IsNextScene()
{
	if (m_btnStart->m_CurrentState == UI_SELECT)
	{
		return true;
	}
	return false;
}
bool TSceneTitle::Init()
{
	I_Sprite.Load(L"SpriteInfo.txt");
	I_Sprite.Load(L"ui.txt");
	std::wstring shaderfilename = L"../../data/shader/DefaultShape.txt";
	m_pMapTitle = new TBaseObject;
	m_pMapTitle->Create(m_pd3dDevice,m_pImmediateContext,shaderfilename,L"../../data/gameHeight.png");

	m_Dlg = new TDialog;
	m_Dlg->Create(m_pd3dDevice, m_pImmediateContext,shaderfilename,	L"../../data/ui/popup_normal.png");
	m_Dlg->SetAttribute({ 300, 200 },{ 0, 0,	400.0f, 300.0f });
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
	m_Dlg->SetDrawList( 0.1f, 0.1f,0.1f, 0.1f,0.2f, 0.2f,0.33f, 0.33f);

	m_btnStart = new TButton;
	m_btnStart->Create(m_pd3dDevice,m_pImmediateContext,shaderfilename,	L"../../data/ui/main_start_nor.png");
	std::vector<W_STR> stateList;
	stateList.push_back(L"../../data/ui/main_start_nor.png");
	stateList.push_back(L"../../data/ui/main_start_sel.png");
	stateList.push_back(L"../../data/ui/main_start_pus.png");
	stateList.push_back(L"../../data/ui/main_start_dis.png");
	m_btnStart->SetAttribute({ 400.0f, 300.0f }, {0,0, 100, 60}, stateList);

	m_btnStart2 = new TButton;
	m_btnStart2->Create(m_pd3dDevice, m_pImmediateContext, shaderfilename, L"../../data/ui/main_start_nor.png");
	m_btnStart2->SetAttribute({ 400.0f, 500.0f }, { 0,0, 100, 60 }, 
						I_Sprite.GetPtr(L"NormalButton3"));


	m_listControl = new TListControl;
	m_listControl->Create(m_pd3dDevice,	m_pImmediateContext,shaderfilename,	L"../../data/ui/RAINBOW.png");
	m_listControl->SetAttribute({ 300-100, 300-100 },{ 0, 0,	100.0f, 250.0f });
	UINT iNumBtn = 10;
	for (int iBtn = 0; iBtn < iNumBtn; iBtn++)
	{
		TInterface* btn = new TButton;
		btn->Create(m_pd3dDevice,m_pImmediateContext,shaderfilename,L"../../data/ui/main_plus_dis.png");
		TVector2D vPos = { m_listControl->m_vPos.x,
			  m_listControl->m_rtCollision.y1 + iBtn * (m_listControl->m_rtCollision.h / iNumBtn)+(m_listControl->m_rtCollision.h / iNumBtn) / 2.0f };
		TRect rt = { 0, 0,	m_listControl->m_rtInit.w, m_listControl->m_rtInit.h / iNumBtn };
		btn->SetAttribute( vPos, rt,  stateList);		
		m_listControl->AddChild(btn);
	}

	m_Dlg->AddChild(m_btnStart);
	m_Dlg->AddChild(m_listControl);


	m_FadeObject = new TInterface;
	m_FadeObject->Create(m_pd3dDevice, m_pImmediateContext, shaderfilename, L"../../data/black.png");
	return true;
}
bool TSceneTitle::Frame()
{	
	m_btnStart2->Frame();

	TVector2D pos = m_Dlg->m_vPos;
	pos.x = pos.x + g_fSecondPerFrame*10.0f;
	m_Dlg->SetPosition(pos);
	m_Dlg->Frame();

	static float fAlpha = 0.0f;
	//if (fAlpha < 1.0f)
	{
		//fAlpha += g_fSecondPerFrame*0.1f;
		m_FadeObject->FadeInOut(cosf(g_fGameTimer)*0.5f+0.5f);
	}
	m_FadeObject->Frame();
	return true;
}
bool TSceneTitle::Render()
{
	m_pMapTitle->Render();
	m_btnStart2->Render();
	
	m_Dlg->Render();
	//m_FadeObject->Render();
	return true;
}
bool TSceneTitle::Release()
{
	m_pMapTitle->Release();
	m_btnStart2->Release();	
	m_Dlg->Release();
	m_FadeObject->Release();
	return true;
}