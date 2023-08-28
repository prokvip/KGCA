#include "Sample.h"

float g_fMapHalfSizeX = 400;
float g_fMapHalfSizeY = 300;

bool Sample::Init()
{
	// 소유권(유일무일한 포인터를 들고 있다.), 참조레퍼런싱 X
	m_pMapObj = std::make_unique<TPlaneObj>();
	m_pMapObj->Set(m_pDevice, m_pImmediateContext);
	m_pMapObj->SetPos({ 0.0f,0.0f ,0.0f });
	m_pMapObj->SetScale(TVector3(g_fMapHalfSizeX, g_fMapHalfSizeY, 1.0f));
	m_pMapObj->Create(L"../../res/topdownmap.jpg",L"../../res/shader/Plane.hlsl");

	// 참조레퍼런싱 
	m_pSpriteTexObj = std::make_unique<TSpriteTexture>();
	TSpriteInfo info;	
	info.p = { 0.0f,0.0f ,0.0f };
	info.s = TVector3(100, 100, 1.0f);
	info.fAnimTimer = 10.0f;
	info.texFile = L"../../res/ui/0.png";
	info.texList.push_back(L"../../res/ui/0.png");
	info.texList.push_back(L"../../res/ui/1.png");
	info.texList.push_back(L"../../res/ui/2.png");
	info.texList.push_back(L"../../res/ui/3.png");
	info.texList.push_back(L"../../res/ui/4.png");
	info.texList.push_back(L"../../res/ui/5.png");
	info.texList.push_back(L"../../res/ui/6.png");
	info.texList.push_back(L"../../res/ui/7.png");
	info.texList.push_back(L"../../res/ui/8.png");
	info.texList.push_back(L"../../res/ui/9.png");
	
	info.shaderFile = L"../../res/shader/Plane.hlsl";
	m_pSpriteTexObj->Load(m_pDevice, m_pImmediateContext, info);
	m_SpriteList.insert(std::make_pair(m_iSpriteIndex++, m_pSpriteTexObj.get()));

	info.Reset();
	m_pSpriteUVObj = std::make_unique<TSpriteUV>();	
	info.iNumRow = 4;
	info.iNumColumn = 4;
	info.fAnimTimer = 1.0f;
	info.p = { 200.0f,0.0f ,0.0f };
	info.s = TVector3(100, 100, 1.0f);
	info.texFile = L"../../res/CanonBomb-01.tga";
	info.shaderFile = L"../../res/shader/Plane.hlsl";
	m_pSpriteUVObj->Load(m_pDevice, m_pImmediateContext, info);
	m_SpriteList.insert(std::make_pair(m_iSpriteIndex++, m_pSpriteUVObj.get()));


	info.Reset();
	m_pSpriteAirObj = std::make_unique<TSpriteObj>(); 
	ZeroMemory(&info, sizeof(info));
	info.p = { -200.0f,0.0f ,0.0f };
	info.s = TVector3(100, 100, 1.0f);
	info.fAnimTimer = 1.0f;
	info.texFile = L"../../res/bitmap1.bmp";
	info.texAlphaFile = L"../../res/bitmap2.bmp";
	info.shaderFile = L"../../res/shader/PlaneMask.hlsl";
	m_pSpriteAirObj->Load(m_pDevice, m_pImmediateContext, info);
	m_SpriteList.insert(std::make_pair(m_iSpriteIndex++, m_pSpriteAirObj.get()));

	
	m_MainCamera.Create({ 0.0f,0.0f, 0.0f }, { (float)m_dwWindowWidth, (float)m_dwWindowHeight });
	return true;
}
bool Sample::Frame()
{
	m_pMapObj->Frame();
	if (I_Input.m_dwKeyState[VK_LBUTTON] == KEY_PUSH)
	{
		TVector3 mouse = I_Input.GetWorldPos(
			{ (float)g_dwWindowWidth , (float)g_dwWindowHeight },
			m_MainCamera.m_vCameraPos);
		TParticle info;
		ZeroMemory(&info, sizeof(info));
		info.m_bLife = true;
		info.m_iID = rand() % m_SpriteList.size();
		info.m_vPos = mouse;// { randstep(-400.0f, +400.0f), randstep(-400.0f, +400.0f), 0 };
		info.m_vSclae = { 50.0f, 50.0f, 50.0f };
		info.m_iMaxFrame = m_SpriteList[info.m_iID]->GetMaxSize();
		info.m_fOffsetTime = m_SpriteList[info.m_iID]->m_fAnimTimer/ info.m_iMaxFrame;
		//info.m_fOffsetTime = randstep(0.1f, 1.0f);
		m_ParticleList.push_back(info);
	}	

	for (std::list<TParticle>::iterator iter = m_ParticleList.begin();
		iter != m_ParticleList.end();
		)
	{
		TParticle& info = *iter;
		info.Frame();
		if (info.m_bLife == false)
		{
			iter = m_ParticleList.erase(iter);
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

	for (std::list<TParticle>::iterator iter = m_ParticleList.begin();
		iter != m_ParticleList.end();
		iter++)
	{
		TParticle& info = *iter;
		TSpriteObj* pBaseObj = m_SpriteList[info.m_iID];
		pBaseObj->SetPos(info.m_vPos);
		pBaseObj->SetScale(info.m_vSclae);		
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
	m_pSpriteTexObj->Release();
	m_pSpriteUVObj->Release();
	m_pSpriteAirObj->Release();
	return true;
}

TGAME(L"kgca", 800, 600)