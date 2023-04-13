#include "TLog.h"

void    TLog::log(std::string data)
{
    std::unique_lock<std::mutex> lock(m_hMutex);
    m_szQueue.push(data);
    m_hEvent.notify_all();
}
void	TLog::Run()
{
    std::ofstream ofs(m_Logfilepath);
    if (ofs.fail())
    {
        return;
    }

    std::unique_lock<std::mutex> lock(m_hMutex);
    m_bThreadstarted = true;
    m_hThreadStartEvent.notify_all();

    while (!m_bExit)
    {
        m_hEvent.wait(lock);
        
        lock.unlock();
        while (1)
        {
            // 1개씩 처리하는 이유는 락을 너무 오래 점유해서 다른 스레드를 블록킹하지 않기 위함이다.
            lock.lock();
            if (m_szQueue.empty())
            {                
                break;
            }else
            {
                ofs << m_szQueue.front() << std::endl;
                m_szQueue.pop();
            }
            lock.unlock();            
        }
        if (m_bExit)
        {
            break;
        }
    }
}
TLog::TLog() :m_bExit(false)
{
    system("md ..\\log");
    time_t now = time(NULL);
    struct tm lt;
    localtime_s(&lt, &now);

    m_Logfilepath.resize(100);
    strftime(&m_Logfilepath.at(0), 100, "../log/%Y년%m월%d일%H시%M분%S초.txt", &lt);

    m_bThreadstarted = false;
	m_hThread = std::thread{ &TLog::Run, this };
    std::unique_lock<std::mutex> lock(m_hMutuxStarted);
    m_hThreadStartEvent.wait(lock,
        [&]() {return m_bThreadstarted == true; });
}
TLog::~TLog()
{
    m_bExit = true;
    m_hEvent.notify_all();
    m_hThread.join();
}