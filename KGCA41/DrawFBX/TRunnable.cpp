#include "TRunnable.h"
namespace T
{
	void TRunnable::start()
	{
		m_hThread = std::thread(&TRunnable::Run, this);
	}
	void TRunnable::stop() {
		m_bStop = true;
		if (m_hThread.joinable())
		{
			m_hThread.join();
		}
	}
	void TRunnable::suspend()
	{
		SuspendThread(m_hThread.native_handle());
	}
	void TRunnable::resume() {
		ResumeThread(m_hThread.native_handle());
	}
	bool TRunnable::isStop() {
		return m_bStop;
	}

	TRunnable::TRunnable() :m_bStop(false), m_hThread()
	{

	}
	TRunnable::~TRunnable()
	{

	}


	void  TSyncEvent::SetEvent()
	{
		m_bSignaled = true;
		m_hEvent.notify_all();
	}
	void  TSyncEvent::ResetEvent()
	{
		m_bSignaled = false;
	}
	// 0 = m_bSignaled
	// -1 = 
	// -2 = 
	int   TSyncEvent::WaitForSingleObject(UINT timeout)
	{
		if (m_bSignaled) return 0;

		int ret = -1;
		std::unique_lock<std::mutex> lock(m_hMutex);
		// 무한대기
		if (timeout == T_INFINITE)
		{
			m_hEvent.wait(lock);
			return 0;
		}
		// 지정시간 대기
		else
		{
			auto wait = m_hEvent.wait_for(lock,
				std::chrono::milliseconds(timeout));
			if (wait == std::cv_status::timeout)
			{
				ret = -1;
			}
			else if (wait == std::cv_status::no_timeout)
			{
				ret = 0;
			}
			lock.unlock();
		}
		return ret;
	}


	int   TEventHandler::CreateEvent(std::string name)
	{
		m_EventArray.push_back(new TSyncEvent(name));
		return m_EventArray.size();
	}
	void  TEventHandler::SetEvent(int id)
	{
		m_EventArray[id]->SetEvent();
	}
	void  TEventHandler::ResetEvent(int id) {
		m_EventArray[id]->ResetEvent();
	}
	int   TEventHandler::WaitForSingleObject(int id, UINT timeout) {
		return m_EventArray[id]->WaitForSingleObject(timeout);
	}
	int   TEventHandler::WaitForMultipleObject(UINT iNumCount,
		int idlist[], bool waitAll,
		UINT timeout) 
	{
		UINT iTotalWaitTimer = 0;
		std::vector<bool> retList;
		for (int i = 0; i < iNumCount; i++)
		{
			retList.push_back(false);
		}
		int iRet = -1;
		do {
			for (int i = 0; i < iNumCount; i++)
			{
				int iRet =
					m_EventArray[idlist[i]]->WaitForSingleObject(10);
				if (waitAll == false)
				{
					if (iRet == 0) return idlist[i]+1;
				}
				retList[i] = iRet;
				if (iRet == -1)
				{
					iTotalWaitTimer += 10;
					if (iTotalWaitTimer >= timeout)
					{						
						return -1;
					}
				}
			}

			// total event check!
			bool bWorkEnd = true;
			int  iWorkID = -1;
			for (int i = 0; i < iNumCount; i++)
			{
				if (retList[i] != 0)
				{		
					bWorkEnd = false;
					iWorkID = retList[i];
					break;
				}
			}
			if (waitAll == true && bWorkEnd == true)
			{
				return 0;
			}
			if (waitAll == false && bWorkEnd == false)
			{
				return iWorkID;
			}
		} while (1);
		return iRet;
	}
	bool  TEventHandler::CloseHandle(int id) 
	{		
		return true;
	}
	void  TEventHandler::CloseAllHandles() 
	{
		for (auto hEvent : m_EventArray)
		{
			delete hEvent;
		}
	}
}