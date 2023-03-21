#include "pch.h"
#include "TTimer.h"
#include <timeapi.h>

float g_fGameTimer = 0.0f;
float g_fSecondPerFrame = 0.0f;

bool		TTimer::Init()
{
    m_fGameTimer = 0.0f;
    m_fElapseTimer = 10.0f;
    m_dwBeforeTime = timeGetTime(); //(1000: 1ÃÊ)
    QueryPerformanceFrequency(&m_Frequency);
    QueryPerformanceCounter(&m_Frame);//m_dwBeforeTime
    return true;
}
bool		TTimer::Frame()
{
    QueryPerformanceCounter(&m_Current);
    m_fElapseTimer = (float)(m_Current.QuadPart - m_Frame.QuadPart) /
        (float)m_Frequency.QuadPart;

 /*   DWORD dwCurrentTime = timeGetTime();
    DWORD dwElapseTime = dwCurrentTime - m_dwBeforeTime;
    m_fElapseTimer = dwElapseTime / 1000.0f;*/
    m_fGameTimer += m_fElapseTimer;
    {
        m_iFPSCounter++;
        m_fFPSTimer += m_fElapseTimer;
        if (m_fFPSTimer >= 1.0f)
        {
            m_iFPS = m_iFPSCounter;
            m_iFPSCounter = 0;
            m_fFPSTimer = m_fFPSTimer - 1.0f;
        }
    }

    g_fGameTimer = m_fGameTimer;
    g_fSecondPerFrame = m_fElapseTimer;
    //m_dwBeforeTime = dwCurrentTime;

    m_Frame = m_Current;
    return true;
}
bool		TTimer::Render()
{
    m_szTimer = std::to_wstring(m_fGameTimer);
    m_szTimer += L"   ";
    m_szTimer += std::to_wstring(m_iFPS);
    m_szTimer += L"\n";
    OutputDebugString(m_szTimer.c_str());
    return true;
}
bool		TTimer::Release()
{
    return true;
}