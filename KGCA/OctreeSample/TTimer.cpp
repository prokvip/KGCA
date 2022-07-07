#include "TTimer.h"

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
