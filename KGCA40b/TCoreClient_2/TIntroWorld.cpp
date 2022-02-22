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
	TTexture* pColorTex = I_Texture.Load(L"../../data/KGCABK.bmp");
	TTexture* pTex = I_Texture.Load(L"../../data/ui/main_start_nor.png");
	TShader* pVShader = I_Shader.CreateVertexShader(
			 m_pd3dDevice, L"Shader.txt", "VS");
	TShader* pPShader = I_Shader.CreatePixelShader(
		m_pd3dDevice, L"Shader.txt","PSAlphaBlend");
	TImageObject* obj = new TImageObject;
	obj->Init();
	obj->SetRectDraw({ 0,0, g_rtClient.right,g_rtClient.bottom });
	obj->SetPosition(TVector2(g_rtClient.right/2.0f, g_rtClient.bottom/2.0f));
	obj->m_pColorTex = pColorTex;
	obj->m_pMaskTex = nullptr;
	obj->m_pVShader = pVShader;
	obj->m_pPShader = pPShader;

	if (!obj->Create(m_pd3dDevice, m_pContext))
	{
		return false;
	}

	m_UIObj.insert(std::make_pair(L"bk", obj));

	TImageObject* btnObj = new TImageObject;
	btnObj->Init();
	btnObj->SetRectDraw({ 0,0, 334,82 });
	btnObj->SetPosition(TVector2(g_rtClient.right / 2.0f, g_rtClient.bottom / 2.0f));
	if (!btnObj->Create(m_pd3dDevice, m_pContext,
		L"../../data/shader/DefaultUI.txt",
		L"../../data/main_start_nor.png"))
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