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

	m_SpriteList.insert(std::make_pair(0, m_pSpriteTexObj));

	m_pSpriteUVObj = new TSpriteUV;
	ZeroMemory(&info, sizeof(info));
	info.p = { 200.0f,0.0f ,0.0f };
	info.s = TVector3(100, 100, 1.0f);
	info.texFile = L"../../res/CanonBomb-01.tga";
	info.shaderFile = L"../../res/shader/Plane.hlsl";
	m_pSpriteUVObj->Load(m_pDevice, m_pImmediateContext, info);

	m_SpriteList.insert(std::make_pair(1, m_pSpriteUVObj));


	m_pSpriteAirObj = new TSpriteObj;
	ZeroMemory(&info, sizeof(info));
	info.p = { -200.0f,0.0f ,0.0f };
	info.s = TVector3(100, 100, 1.0f);
	info.texFile = L"../../res/bitmap1.bmp";
	info.texAlphaFile = L"../../res/bitmap2.bmp";
	info.shaderFile = L"../../res/shader/PlaneMask.hlsl";
	m_pSpriteAirObj->Load(m_pDevice, m_pImmediateContext, info);
	m_SpriteList.insert(std::make_pair(2, m_pSpriteAirObj));


	
	m_MainCamera.Create({ 0.0f,0.0f, 0.0f }, { (float)m_dwWindowWidth, (float)m_dwWindowHeight });
	return true;
}
bool Sample::Frame()
{
	m_pMapObj->Frame();
	if (I_Input.m_dwKeyState[VK_HOME] == KEY_PUSH)
	{
		TEffectInfo info;
		ZeroMemory(&info, sizeof(info));
		info.bLifeEnable = true;
		info.index = rand() % m_SpriteList.size();
		info.p = { randstep(-400.0f, +400.0f), randstep(-400.0f, +400.0f), 0 };
		info.s = { 50.0f, 50.0f, 50.0f };
		info.iMaxSize = m_SpriteList[info.index]->GetMaxSize();
		info.m_fOffsetTime = 3.0f/ info.iMaxSize;
		effectList.push_back(info);
	}	

	for (std::list<TEffectInfo>::iterator iter = effectList.begin();
		iter != effectList.end();
		)
	{
		TEffectInfo& info = *iter;
		info.Frame();
		if (info.bLifeEnable == false)
		{
			iter = effectList.erase(iter);
		}
		else
		{
			iter++;
		}
	}
	return true;
}
bool Sample::Render()
{
	m_pMapObj->SetMatrix(nullptr, &m_MainCamera.m_matView, &m_MainCamera.m_matOrthoProjection);
	m_pMapObj->Render();

	for (std::list<TEffectInfo>::iterator iter = effectList.begin();
		iter != effectList.end();
		iter++)
	{
		TEffectInfo& info = *iter;
		TSpriteObj* pBaseObj = m_SpriteList[info.index];
		pBaseObj->SetPos(info.p);
		pBaseObj->SetScale(info.s);		
		pBaseObj->Frame();
		pBaseObj->m_iCurrentAnimIndex = info.m_iCurrentAnimIndex;
		pBaseObj->SetMatrix(nullptr, &m_MainCamera.m_matView, &m_MainCamera.m_matOrthoProjection);
		pBaseObj->Render();
	}
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