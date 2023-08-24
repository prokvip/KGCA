#include "Sample.h"

float g_fMapHalfSizeX = 400;
float g_fMapHalfSizeY = 300;

bool Sample::Init()
{
	const TTexture* pTex = I_Tex.Load(L"../../res/ui/0.png");
	m_pTexList.push_back(pTex);
	pTex = I_Tex.Load(L"../../res/ui/1.png");
	m_pTexList.push_back(pTex);
	pTex = I_Tex.Load(L"../../res/ui/2.png");
	m_pTexList.push_back(pTex);
	pTex = I_Tex.Load(L"../../res/ui/3.png");
	m_pTexList.push_back(pTex);
	pTex = I_Tex.Load(L"../../res/ui/4.png");
	m_pTexList.push_back(pTex);
	pTex = I_Tex.Load(L"../../res/ui/5.png");
	m_pTexList.push_back(pTex);
	pTex = I_Tex.Load(L"../../res/ui/6.png");
	m_pTexList.push_back(pTex);
	pTex = I_Tex.Load(L"../../res/ui/7.png");
	m_pTexList.push_back(pTex);
	pTex = I_Tex.Load(L"../../res/ui/8.png");
	m_pTexList.push_back(pTex);
	pTex = I_Tex.Load(L"../../res/ui/9.png");
	m_pTexList.push_back(pTex);
		
	m_pMapObj = new TPlaneObj;
	m_pMapObj->Set(m_pDevice, m_pImmediateContext);
	m_pMapObj->SetPos({ 0.0f,0.0f ,0.0f });
	m_pMapObj->SetScale(TVector3(g_fMapHalfSizeX, g_fMapHalfSizeY, 1.0f));
	m_pMapObj->Create(L"../../res/topdownmap.jpg", L"../../res/shader/Plane.hlsl");

	m_pSpriteObj = new TPlaneObj;
	m_pSpriteObj->Set(m_pDevice, m_pImmediateContext);
	m_pSpriteObj->SetPos({ 0.0f,0.0f ,0.0f });
	m_pSpriteObj->SetScale(TVector3(100, 100, 1.0f));
	m_pSpriteObj->Create(L"../../res/ui/0.png",
		L"../../res/shader/Plane.hlsl");

	m_MainCamera.Create({ 0.0f,0.0f, 0.0f }, { (float)m_dwWindowWidth, (float)m_dwWindowHeight });
	return true;
}
bool Sample::Frame()
{
	if (I_Input.m_dwKeyState[VK_HOME] == KEY_PUSH)
	{
		m_iTexIndex++;
		if (m_iTexIndex >= m_pTexList.size())
		{
			m_iTexIndex = m_pTexList.size() - 1;
		}
	}
	if (I_Input.m_dwKeyState[VK_END] == KEY_PUSH)
	{
		m_iTexIndex--;
		if (m_iTexIndex< 0)
		{
			m_iTexIndex = 0;
		}
	}
	return true;
}
bool Sample::Render()
{
	m_pMapObj->SetMatrix(nullptr, &m_MainCamera.m_matView, &m_MainCamera.m_matOrthoProjection);
	m_pMapObj->Render();

	m_pSpriteObj->SetMatrix(nullptr, &m_MainCamera.m_matView, &m_MainCamera.m_matOrthoProjection);
	m_pSpriteObj->PreRender();

	m_iTexIndex = (int)g_fGameTimer % 10;
	if (m_pTexList[m_iTexIndex]!=nullptr)
	{
		m_pTexList[m_iTexIndex]->Apply(m_pImmediateContext, 0);
	}
	m_pSpriteObj->PostRender();
	return true;
}
bool Sample::Release()
{
	m_pMapObj->Release();
	delete m_pMapObj;

	m_pSpriteObj->Release();
	delete m_pSpriteObj;
	return true;
}

TGAME(L"kgca", 800, 600)