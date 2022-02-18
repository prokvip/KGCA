#include "TZoneWorld.h"
#include "TInput.h"
bool	TZoneWorld::Init()
{
	return true;
}
bool	TZoneWorld::Load(std::wstring file)
{
	TSound* pSound = I_Sound.Load("../../data/Sound/OnlyLove.MP3");
	pSound = I_Sound.Load("../../data/Sound/MyLove.MP3");
	m_pBackGroundMusic = I_Sound.Load("../../data/Sound/OnlyLove.MP3");
	pSound = I_Sound.Load("../../data/Sound/00_Menu.MP3");
	pSound = I_Sound.Load("../../data/Sound/Gun1.wav");
	pSound = I_Sound.Load("../../data/Sound/abel_leaf.asf");
	pSound = I_Sound.Load("../../data/Sound/GunShot.mp3");
	//pSound = I_Sound.Load("../../data/Sound/pianoSound_00.MP3");
	//pSound = I_Sound.Load("../../data/Sound/romance.mid");
	m_pBackGroundMusic->Play(true);

	m_PlayerObj.Init();
	m_PlayerObj.SetPosition(TVector2(400, 500));
	m_PlayerObj.SetRectSouce({ 91,1,42,56 });
	//m_PlayerObj.SetRectSouce({ 46,63,69,79 });
	m_PlayerObj.SetRectDraw({ 0,0, 42,56 });

	if (!m_PlayerObj.Create(m_pd3dDevice, m_pContext,
		L"../../data/bitmap1.bmp",
		L"../../data/bitmap2.bmp"))
	{
		return false;
	}

	for (int iNpc = 0; iNpc < 10; iNpc++)
	{
		TObjectNpc2D* npc = new TObjectNpc2D;
		npc->Init();
		if (iNpc % 2 == 0)
		{
			npc->SetRectSouce({ 46,63,69,79 });
			npc->SetRectDraw({ 0,0, 69,79 });
		}
		else
		{
			npc->SetRectSouce({ 1,63,42,76 });
			npc->SetRectDraw({ 0,0, 42,76 });
		}

		npc->SetPosition(TVector2(50 + iNpc * 150, 50));
		if (!npc->Create(m_pd3dDevice, m_pContext,
			L"../../data/bitmap1.bmp",
			L"../../data/bitmap2.bmp"))
		{
			return false;
		}
		m_NpcLlist.push_back(npc);
	}
	return true;
}
bool	TZoneWorld::Frame()
{
	if (TInput::Get().GetKey(VK_F1) == KEY_PUSH)
	{
		TSound* pSound = I_Sound.GetPtr(L"GunShot.mp3");
		if (pSound != nullptr)
		{
			pSound->PlayEffect();
		}
	}

	m_pBackGroundMusic->Frame();

	if (TInput::Get().GetKey(VK_UP) == KEY_HOLD)
	{
		m_pBackGroundMusic->VolumeUp(g_fSecPerFrame);
	}
	if (TInput::Get().GetKey(VK_DOWN) == KEY_HOLD)
	{
		m_pBackGroundMusic->VolumeDown(g_fSecPerFrame);
	}

	m_PlayerObj.Frame();
	for (int iObj = 0; iObj < m_NpcLlist.size(); iObj++)
	{
		m_NpcLlist[iObj]->Frame();
	}
	return true;
}
bool	TZoneWorld::Render()
{
	for (int iObj = 0; iObj < m_NpcLlist.size(); iObj++)
	{
		if (m_NpcLlist[iObj]->m_bDead == false)
		{
			m_NpcLlist[iObj]->Render();
		}
	}
	m_PlayerObj.Render();

	//RECT rt = g_rtClient;
	//rt.top = 300;
	//rt.left = 0;
	//m_dxWrite.Draw(m_pBackGroundMusic->m_szBuffer, rt, D2D1::ColorF(0, 0, 0, 1),
	//	m_dxWrite.m_pd2dMTShadowTF);
	return true;
}
bool	TZoneWorld::Release()
{
	for (int iObj = 0; iObj < m_NpcLlist.size(); iObj++)
	{
		m_NpcLlist[iObj]->Release();
		delete m_NpcLlist[iObj];
	}
	m_PlayerObj.Release();
	m_NpcLlist.clear();
	return true;
}