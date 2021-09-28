#include "TTimer.h"

float g_fSecPerFrame = 0.0f;

bool TTimer::Init()
{
	QueryPerformanceCounter(&m_Frame);
	
	return false;
}
bool TTimer::Frame()
{
	QueryPerformanceCounter(&m_Current);
	m_fSecPerFrame =
		static_cast<float>(m_Current.QuadPart - m_Frame.QuadPart)
		/ static_cast<float>(m_Frequency.QuadPart);
	g_fSecPerFrame = m_fSecPerFrame;

	static float fpsTime = 0.0f;
	m_iTmpCounter++;
	fpsTime += m_fSecPerFrame;
	if (fpsTime >= 1.0f)
	{
		m_iFPS = m_iTmpCounter;
		m_iTmpCounter = 0;
		fpsTime -= 1.0f;		
	}
	
	m_fGameTimer += m_fSecPerFrame;
	m_Frame = m_Current;
	return true;
}
bool TTimer::Render()
{
	_stprintf_s(m_szTimerString,
		L"FPS=%d:Timer=%10.4f",
		m_iFPS,
		m_fGameTimer);
	//OutputDebugString(timerString);
	return true;
}
bool TTimer::Release()
{
	return false;
}
TTimer::TTimer()
{
	m_iTmpCounter = 0;
	m_iFPS = 0;
	m_fSecPerFrame = 0.0f;
	m_fGameTimer = 0.0f;
	QueryPerformanceFrequency(&m_Frequency);
}
TTimer::~TTimer()
{

}