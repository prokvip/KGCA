#include "Sample.h"
bool Sample::Init()
{	
	//I_Sound.LoadAll(L"../../data/sound/");
	m_pBGSound = I_Sound.GetPtr(L"OnlyLove.MP3");
	if (m_pBGSound == nullptr)
	{
		m_pBGSound = I_Sound.Load(L"../../data/sound/OnlyLove.MP3");
	}
	m_pShot = I_Sound.GetPtr(L"GunShot.mp3");
	if (m_pShot == nullptr)
	{
		m_pShot = I_Sound.Load(L"../../data/sound/GunShot.mp3");
	}
	m_pEffect = I_Sound.GetPtr(L"romance.mid");
	if (m_pEffect == nullptr)
	{
		m_pEffect = I_Sound.Load(L"../../data/sound/romance.mid");
	}
	m_pBGSound->Play(true);
	return true;
}
bool Sample::Frame()
{	
	I_Sound.Frame();
	if (I_Input.GetKey(VK_HOME) == KEY_PUSH)
	{
		m_pShot->PlayEffect();
	}
	if (I_Input.GetKey(VK_INSERT) == KEY_PUSH)
	{
		m_pEffect->PlayEffect();
	}
	if (I_Input.GetKey(VK_END) == KEY_PUSH)
	{
		m_pBGSound->Stop();
	}
	if (I_Input.GetKey(VK_F1) == KEY_PUSH)
	{
		m_pBGSound->Paused();
	}
	if (I_Input.GetKey(VK_F2) == KEY_PUSH)
	{
		m_pBGSound->VolumeUp();
	}
	if (I_Input.GetKey(VK_F3) == KEY_PUSH)
	{
		m_pBGSound->VolumeDown();
	}
	
	return true;
}
bool Sample::Render()
{	
	m_Writer.Draw(0, 100, m_pBGSound->m_szBuffer);
	return true;
}
bool Sample::Release()
{	
	return true;
}

GAME_RUN(SampleSound, 800,600)