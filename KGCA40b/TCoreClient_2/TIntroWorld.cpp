#include "TIntroWorld.h"
#include "TInput.h"
bool	TIntroWorld::Init()
{
	TWorld::Init();
	//m_pBackGroundMusic->Play(true);
	return true;
}
bool	TIntroWorld::Load(std::wstring file)
{	
	m_pBackGroundMusic = I_Sound.Load("../../data/Sound/OnlyLove.MP3");	
	TTexture* pTex = I_Texture.Load(L"../../data/KGCABK.bmp");
	          pTex = I_Texture.Load(L"../../data/ui/main_start_nor.png");
	TShader* pShader = I_Shader.Load(L"../../data/shader/DefaultUI.txt");
			 pShader = I_Shader.Load(L"Shader.txt");

	TImageObject* obj = new TImageObject;
	obj->Init();
	obj->SetRectDraw({ 0,0, g_rtClient.right,g_rtClient.bottom });
	obj->SetPosition(TVector2(g_rtClient.right/2.0f, g_rtClient.bottom/2.0f));
	if (!obj->Create(m_pd3dDevice, m_pContext,
		L"Shader.txt",
		L"KGCABK.bmp"))
	{
		return false;
	}
	m_UIObj.insert(std::make_pair(L"bk", obj));

	TImageObject* btnObj = new TImageObject;
	btnObj->Init();
	btnObj->SetRectDraw({ 0,0, 334,82 });
	btnObj->SetPosition(TVector2(g_rtClient.right / 2.0f, g_rtClient.bottom / 2.0f));
	if (!btnObj->Create(m_pd3dDevice, m_pContext,
		L"DefaultUI.txt",
		L"main_start_nor.png"))
	{
		return false;
	}
	m_UIObj.insert(std::make_pair(L"btnStart", btnObj));

	return true;
}
bool	TIntroWorld::Frame()
{	
	m_pBackGroundMusic->Frame();
	TWorld::Frame();
	return true;
}
bool	TIntroWorld::Render() 
{	
	TWorld::Render();
	return true;
}
bool	TIntroWorld::Release() 
{	
	TWorld::Release();
	return true;
}