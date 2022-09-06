#include "Sample.h"
bool Sample::Init()
{	
	I_Input.Init();
	return true;
}
bool Sample::Frame()
{	
	I_Input.Frame();
	if (I_Input.GetKey('W') != KEY_FREE)
	{
		INT K = 0;
	}
	if (I_Input.GetKey('S') != KEY_FREE)
	{
		INT K = 0;
	}
	if (I_Input.GetKey('A') != KEY_FREE)
	{
		INT K = 0;
	}
	if (I_Input.GetKey('D') != KEY_FREE)
	{
		INT K = 0;
	}
	return true;
}
bool Sample::Render()
{	
	I_Input.Render();
	std::wstring test = std::to_wstring(I_Input.m_ptPos.x);
	test += L" ";
	test += std::to_wstring(I_Input.m_ptPos.y);
	test += '\n';
	OutputDebugString(test.c_str());
	return true;
}
bool Sample::Release()
{	
	I_Input.Release();
	return true;
}

GAME_RUN(TestInput, 800,600)