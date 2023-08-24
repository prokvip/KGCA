#include "TTimer.h"
#include "TWriter.h"
float	g_fGameTimer = 0.0f;
float   g_fSecondPerFrame = 0.0f;

bool	TTimer::Init()
{
	m_fFramePerSecond = 0.0f;
	m_fGameTimer	= 0.0f;
	m_fSecondPerFrame = 0.0f;
	m_dwBeforeTime = timeGetTime();// tick coounter 1000 => 1ÃÊ
	return true;
}
bool	TTimer::Frame()
{
	DWORD dwCurrentTime = timeGetTime();
	DWORD dwElapseTime = dwCurrentTime - m_dwBeforeTime;
	g_fSecondPerFrame = m_fSecondPerFrame = dwElapseTime / 1000.0f;
	g_fGameTimer = m_fGameTimer += m_fSecondPerFrame;
	m_dwBeforeTime = dwCurrentTime;

	m_fFramePerSecond += m_fSecondPerFrame;
	return true;
}
int   TTimer::GetFPS()
{
	static int iFPS = 0;
	if (m_fFramePerSecond >= 1.0f)
	{
		m_iFPS = iFPS;
		iFPS = 0;
		m_fFramePerSecond -= 1.0f;
	}
	iFPS++;
	return m_iFPS;
}
bool	TTimer::Render()
{
	std::wstring msg = L"[FPS]";
	msg += std::to_wstring(GetFPS());
	msg += L"[GT]";
	msg += std::to_wstring(m_fGameTimer);
	msg += L"[SPF]";
	msg += std::to_wstring(m_fSecondPerFrame);

	I_Writer.AddText(
		msg.c_str(), 0, 0,
		{ 1.0f, 1.0f, 0.0f, 1.0f });	
	return true;
}
bool	TTimer::Release()
{
	return true;
}