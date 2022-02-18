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
	return true;
}
bool	TIntroWorld::Frame()
{	
	m_pBackGroundMusic->Frame();
	return true;
}
bool	TIntroWorld::Render() 
{
	
	return true;
}
bool	TIntroWorld::Release() 
{	
	return true;
}