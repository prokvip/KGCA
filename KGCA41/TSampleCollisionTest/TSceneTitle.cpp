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
	std::wstring shaderfilename = L"../../data/shader/DefaultShape.txt";
	m_pMapTitle = new TBaseObject;
	m_pMapTitle->Create(m_pd3dDevice,
		m_pImmediateContext,
		shaderfilename,
		L"../../data/kgcabk.bmp");

	m_btnStart = new TButton;
	m_btnStart->Create(m_pd3dDevice,
		m_pImmediateContext,
		shaderfilename,
		L"../../data/ui/main_start_nor.png");
	m_btnStart->Init();

	TTexture* pTexture = I_Tex.Load(L"../../data/ui/main_start_nor.png");
	m_btnStart->m_pCurrentTex = pTexture;
	m_btnStart->m_pStateList.push_back(pTexture);
	pTexture = I_Tex.Load(L"../../data/ui/main_start_sel.png");
	m_btnStart->m_pStateList.push_back(pTexture);
	pTexture = I_Tex.Load(L"../../data/ui/main_start_pus.png");
	m_btnStart->m_pStateList.push_back(pTexture);
	pTexture = I_Tex.Load(L"../../data/ui/main_start_dis.png");
	m_btnStart->m_pStateList.push_back(pTexture);
	m_btnStart->SetRect({ 0, 0,	332.0f, 82.0f });
	m_btnStart->SetPosition({ 400.0f, 300.0f });
	

	m_listControl = new TListControl;
	m_listControl->Create(m_pd3dDevice,
		m_pImmediateContext,
		shaderfilename,
		L"../../data/ui/RAINBOW.png");
	m_listControl->m_pCurrentTex = m_listControl->m_pTexture;
	m_listControl->Init();
	m_listControl->SetRect({ 0, 0,	100.0f, 500.0f });
	m_listControl->SetPosition({ 60, 300 });
	UINT iNumBtn = 10;
	for (int iBtn = 0; iBtn < iNumBtn; iBtn++)
	{
		TButton* btn = new TButton;
		btn->Create(m_pd3dDevice,
			m_pImmediateContext,
			shaderfilename,
			L"../../data/ui/main_plus_dis.png");
		btn->Init();

		TTexture* pTexture = I_Tex.Load(L"../../data/ui/main_shop_nor.png");
		btn->m_pCurrentTex = pTexture;
		btn->m_pStateList.push_back(pTexture);
		pTexture = I_Tex.Load(L"../../data/ui/main_shop_sel.png");
		btn->m_pStateList.push_back(pTexture);
		pTexture = I_Tex.Load(L"../../data/ui/main_shop_pus.png");
		btn->m_pStateList.push_back(pTexture);
		pTexture = I_Tex.Load(L"../../data/ui/main_shop_dis.png");
		btn->m_pStateList.push_back(pTexture);
		btn->SetRect({ 0, 0,	
			m_listControl->m_rtInit.w*0.8f,
			m_listControl->m_rtInit.h/ iNumBtn, });
		btn->SetPosition({  m_listControl->m_vPos.x, 
							m_listControl->m_rtCollision.y1
							+ iBtn*(m_listControl->m_rtCollision.h/ iNumBtn)
							+ (m_listControl->m_rtCollision.h / iNumBtn)/2.0f});

		m_listControl->m_btnList.push_back(btn);
	}
	return true;
}
bool TSceneTitle::Frame()
{	
	m_btnStart->Frame();
	m_listControl->Frame();
	return true;
}
bool TSceneTitle::Render()
{
	m_pMapTitle->Render();
	m_btnStart->Render();
	m_listControl->Render();
	return true;
}
bool TSceneTitle::Release()
{
	m_btnStart->Release();
	m_pMapTitle->Release();
	m_listControl->Release();
	return true;
}