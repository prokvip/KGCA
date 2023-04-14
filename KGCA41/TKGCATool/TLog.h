#pragma once
#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <string>

class TLog
{
public:
	static TLog& Get() { static TLog log; return log; }
	TLog();
	virtual ~TLog();
public:
	std::string m_Logfilepath;
	void   log(std::string data);
protected:
	std::thread	m_hThread;
	std::mutex  m_hMutex;
	std::mutex  m_hMutuxStarted;
	std::condition_variable m_hEvent;
	std::condition_variable m_hThreadStartEvent;
	std::queue<std::string>   m_szQueue;
	//bool		m_bExit = false;
	std::atomic<bool> m_bExit;
	bool m_bThreadstarted = false;
protected:
	void	Run();
};

