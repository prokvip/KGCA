#include "Sample.h"

float g_fMapHalfSizeX = 400;
float g_fMapHalfSizeY = 300;

bool Sample::Init()
{
	m_pMapObj = new TPlaneObj;
	m_pMapObj->Set(m_pDevice, m_pImmediateContext);
	m_pMapObj->SetPos({ 0.0f,0.0f ,0.0f });
	m_pMapObj->SetScale(TVector3(g_fMapHalfSizeX, g_fMapHalfSizeY, 1.0f));
	m_pMapObj->Create(L"../../res/topdownmap.jpg",
		L"../../res/shader/Plane.hlsl");

	m_pSpriteTexObj = new TSpriteTexture;
	TSpriteInfo info;
	ZeroMemory(&info, sizeof(info));
	info.p = { 0.0f,0.0f ,0.0f };
	info.s = TVector3(100, 100, 1.0f);
	info.texFile = L"../../res/ui/0.png";
	info.shaderFile = L"../../res/shader/Plane.hlsl";
	m_pSpriteTexObj->Load(m_pDevice, m_pImmediateContext, info);


	m_pSpriteUVObj = new TSpriteUV;
	ZeroMemory(&info, sizeof(info));
	info.p = { 200.0f,0.0f ,0.0f };
	info.s = TVector3(100, 100, 1.0f);
	info.texFile = L"../../res/CanonBomb-01.tga";
	info.shaderFile = L"../../res/shader/Plane.hlsl";
	m_pSpriteUVObj->Load(m_pDevice, m_pImmediateContext, info);


	m_pSpriteAirObj = new TSpriteObj;
	ZeroMemory(&info, sizeof(info));
	info.p = { -200.0f,0.0f ,0.0f };
	info.s = TVector3(100, 100, 1.0f);
	info.texFile = L"../../res/bitmap1.bmp";
	info.texAlphaFile = L"../../res/bitmap2.bmp";
	info.shaderFile = L"../../res/shader/PlaneMask.hlsl";
	m_pSpriteAirObj->Load(m_pDevice, m_pImmediateContext, info);

	m_MainCamera.Create({ 0.0f,0.0f, 0.0f }, { (float)m_dwWindowWidth, (float)m_dwWindowHeight });
	return true;
}
bool Sample::Frame()
{
/*	if (I_Input.m_dwKeyState[VK_HOME] == KEY_PUSH)
	{
		m_iCurrentAnimIndex++;
		if (m_iCurrentAnimIndex >= m_pTexList.size())
		{
			m_iCurrentAnimIndex = m_pTexList.size() - 1;
		}
	}
	if (I_Input.m_dwKeyState[VK_END] == KEY_PUSH)
	{
		m_iCurrentAnimIndex--;
		if (m_iCurrentAnimIndex< 0)
		{
			m_iCurrentAnimIndex = 0;
		}
	}
	*/
	m_pMapObj->Frame();
	m_pSpriteTexObj->Frame();
	m_pSpriteUVObj->Frame();
	m_pSpriteAirObj->Frame();
	return true;
}
bool Sample::Render()
{
	m_pMapObj->SetMatrix(nullptr, &m_MainCamera.m_matView, &m_MainCamera.m_matOrthoProjection);
	m_pMapObj->Render();

	m_pSpriteTexObj->SetMatrix(nullptr, &m_MainCamera.m_matView, &m_MainCamera.m_matOrthoProjection);
	m_pSpriteTexObj->Render();	

	m_pSpriteUVObj->SetMatrix(nullptr, &m_MainCamera.m_matView, &m_MainCamera.m_matOrthoProjection);
	m_pSpriteUVObj->Render();

	m_pSpriteAirObj->SetMatrix(nullptr, &m_MainCamera.m_matView, &m_MainCamera.m_matOrthoProjection);
	if (m_pSpriteAirObj->m_pAlphaTex)
	{
		m_pSpriteAirObj->m_pAlphaTex->Apply(m_pImmediateContext, 1);
	}
	m_pSpriteAirObj->Render();	
	return true;
}
bool Sample::Release()
{
	m_pMapObj->Release();
	delete m_pMapObj;

	m_pSpriteTexObj->Release();
	delete m_pSpriteTexObj;

	m_pSpriteUVObj->Release();
	delete m_pSpriteUVObj;

	m_pSpriteAirObj->Release();
	delete m_pSpriteAirObj;
	return true;
}

TGAME(L"kgca", 800, 600)