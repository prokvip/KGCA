#include "TThread.h"
void TThread::Create()
{
    if (m_bStarted != false) return;
	m_hThread = _beginthreadex(
        nullptr, 0,        
        Runner,
        this,
       0,
        &m_iID );
    m_bStarted = true;
}
void TThread::Create(LPVOID pObject)
{
    if (m_bStarted != false) return;
    m_hThread = _beginthreadex(
        nullptr, 0,
        Runner,
        this,
        0,
        &m_iID);
    m_pObject = pObject;
    m_bStarted = true;
}
bool TThread::Run()
{
    return false;
}
unsigned int WINAPI TThread::Runner(LPVOID param)
{
    TThread* pThread = (TThread*)param;
    if (pThread != nullptr)
    {
        pThread->Run();
        return 1;
    }
    return 0;
}
void TThread::Join()
{
    ::WaitForSingleObject((HANDLE)m_hThread, INFINITE);
}
void TThread::Detach()
{
    CloseHandle((HANDLE)m_hThread);
}
TThread::TThread()
{
    m_bStarted = false;
    m_pObject = nullptr;
}
TThread::TThread(LPVOID pValue) 
{
    m_bStarted = false;
    m_pObject = nullptr;
    Create(pValue);
}
TThread::~TThread()
{
    CloseHandle((HANDLE)m_hThread);
}