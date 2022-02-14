#include "TTimer.h"
float   g_fSecPerFrame = 0.0f;
float   g_fGameTimer = 0.0f;
bool TTimer::Init()
{
    m_fSecondPerFrame = 0.0f;
    m_fTimer = 0.0f;
    m_dwBeforeTime = timeGetTime();
    return false;
}

bool TTimer::Frame()
{
    DWORD dwCurrentTime = timeGetTime();
    DWORD dwElapseTime = dwCurrentTime - m_dwBeforeTime;
    m_fSecondPerFrame = dwElapseTime / 1000.0f;
    m_fTimer += m_fSecondPerFrame;

    g_fSecPerFrame  = m_fSecondPerFrame;
    g_fGameTimer    = m_fTimer;

    m_dwBeforeTime = dwCurrentTime;
    return false;
}

bool TTimer::Render()
{
    return false;
}

bool TTimer::Release()
{
    return false;
}
