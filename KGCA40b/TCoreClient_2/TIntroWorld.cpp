#include "TIntroWorld.h"
#include "TInput.h"
bool	TIntroWorld::Init()
{
	//m_pBackGroundMusic->Play(true);
	return true;
}
bool	TIntroWorld::Load(std::wstring file)
{	
	m_pBackGroundMusic = I_Sound.Load("../../data/Sound/OnlyLove.MP3");	
	TObject2D* obj = new TObject2D;
	obj->Init();
	obj->SetRectDraw({ 0,0, g_rtClient.right,g_rtClient.bottom });
	obj->SetPosition(TVector2(g_rtClient.right/2.0f, g_rtClient.bottom/2.0f));
	if (!obj->Create(m_pd3dDevice, m_pContext,
		L"../../data/KGCABK.bmp"))
	{
		return false;
	}
	m_UIObj.insert(std::make_pair(L"bk", obj));
	return true;
}
bool	TIntroWorld::Frame()
{	
	m_pBackGroundMusic->Frame();
	return true;
}
bool	TIntroWorld::Render() 
{
	for (auto obj : m_UIObj)
	{
		TObject2D* pObj = obj.second;
		if (pObj != nullptr)
		{
			pObj->Render();
		}
	}
	return true;
}
bool	TIntroWorld::Release() 
{	
	TWorld::Release();
	return true;
}