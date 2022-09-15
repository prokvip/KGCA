#include "Sample.h"
bool Sample::Init()
{	
	m_Sound.Init();
	m_Sound.Load("../../data/sound/abel_leaf.asf");	
	//m_Sound.Load("../../data/sound/Gun1.wav");
	return true;
}
bool Sample::Frame()
{	
	m_Sound.Frame();
	if (I_Input.GetKey(VK_HOME) == KEY_PUSH)
	{
		m_Sound.PlayEffect();
	}
	if (I_Input.GetKey(VK_END) == KEY_PUSH)
	{
		m_Sound.Stop();
	}
	if (I_Input.GetKey(VK_F1) == KEY_PUSH)
	{
		m_Sound.Paused();
	}
	if (I_Input.GetKey(VK_F2) == KEY_PUSH)
	{
		m_Sound.VolumeUp();
	}
	if (I_Input.GetKey(VK_F3) == KEY_PUSH)
	{
		m_Sound.VolumeDown();
	}
	
	return true;
}
bool Sample::Render()
{	
	m_Writer.Draw(0, 100, m_Sound.m_szBuffer);
	return true;
}
bool Sample::Release()
{	
	return true;
}

GAME_RUN(SampleSound, 800,600)