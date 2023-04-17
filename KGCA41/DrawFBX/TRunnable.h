#pragma once
#include <windows.h>
#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <string>
#include <array>
#include <map>

static const unsigned T_INFINITE = 0xFFFFFFFF;

namespace T
{
	class TRunnable
	{
	public:
		std::thread m_hThread;
		void start();
		void stop();
		void suspend();
		void resume();
		bool isStop();
		virtual void Run() = 0;
	public:
		std::atomic<bool> m_bStop;
	public:
		TRunnable();
		TRunnable(TRunnable const&) = delete;
		TRunnable& operator = (TRunnable const&) = delete;
		virtual ~TRunnable();
	};

	class TSyncEvent
	{
		std::string  m_csName;
	public:
		std::condition_variable m_hEvent;
		std::mutex				m_hMutex;
		bool					m_bSignaled = false;
	public:
		void  SetEvent();
		void  ResetEvent();
		int   WaitForSingleObject(UINT timeout = T_INFINITE);
		TSyncEvent() {}
		TSyncEvent(std::string name) : m_csName(name)
		{}
	};

	class TEventHandler
	{
		int	m_iIndex = 0;
		std::vector<TSyncEvent*> m_EventArray;
	public:
		int   CreateEvent(std::string name);
		void  SetEvent(int id);
		void  ResetEvent(int id);
		int   WaitForSingleObject(int id, UINT timeout = T_INFINITE);
		int   WaitForMultipleObject(UINT iNumCount,
			   int idlilst[], bool waitAll = false, 
				UINT timeout = T_INFINITE		);
		bool  CloseHandle(int id);
		void  CloseAllHandles();
	};
};