#include "Sample.h"

float g_fMapHalfSizeX = 400;
float g_fMapHalfSizeY = 300;

bool Sample::Init()
{
	//std::unique_ptr<int> aa = std::make_unique<int>();
	////std::unique_ptr<int> bb = std::move(aa);	
	////bb.reset();
	//std::unique_ptr<int>& dd = aa;
	//aa.reset();

	//std::shared_ptr<int> aa1 = std::make_shared<int>();
	////std::shared_ptr<int> bb1 = std::move(aa1);	
	////bb1.reset();
	//std::shared_ptr<int>& bb1 = aa1;
	//aa1.reset();

	//std::shared_ptr<int> bb1 = aa1;
	//aa1.reset();
	
	std::shared_ptr<int> cc1(aa1);
	cc1.reset();
	std::shared_ptr<int>& dd1 = aa1;
	dd1.reset();



	sound = I_Sound.Load(L"../../data/sound/MyLove.mp3");
	sound->Play();

	effectsound = I_Sound.Load(L"../../data/sound/GunShot.mp3");

	m_pMapObj = new TPlaneObj;
	m_pMapObj->Set(m_pDevice, m_pImmediateContext);
	m_pMapObj->SetPos({ 0.0f,0.0f ,0.0f });
	m_pMapObj->SetScale(TVector3(g_fMapHalfSizeX, g_fMapHalfSizeY, 1.0f));
	m_pMapObj->Create(L"../../res/topdownmap.jpg", L"../../res/shader/Plane.hlsl");

	m_MainCamera.Create({0.0f,0.0f, 0.0f },	{ (float)m_dwWindowWidth, (float)m_dwWindowHeight });
	return true;
}
bool Sample::Frame()
{
	I_Sound.Frame();

	if (I_Input.m_dwKeyState[VK_LBUTTON] 
							== KEY_PUSH)
	{
		effectsound->PlayEffect();
	}
	if (I_Input.m_dwKeyState[VK_RBUTTON]
		== KEY_PUSH)
	{
		sound->Stop();
	}
	if (I_Input.m_dwKeyState[VK_HOME]
		== KEY_PUSH)
	{
		sound->Paused();
	}
	if (I_Input.m_dwKeyState[VK_LEFT]
		== KEY_HOLD)
	{
		sound->VolumeDown();
	}
	if (I_Input.m_dwKeyState[VK_RIGHT]
		== KEY_HOLD)
	{
		sound->VolumeUp();
	}
	return true;
}
bool Sample::Render()
{	
	m_pMapObj->SetMatrix(nullptr, &m_MainCamera.m_matView,&m_MainCamera.m_matOrthoProjection);
	m_pMapObj->Render();
	return true;
}
bool Sample::Release()
{
	I_Sound.Release();
	m_pMapObj->Release();
	delete m_pMapObj;
	return true;
}

TGAME(L"kgca", 800, 600)