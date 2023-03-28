#pragma once
#include <queue>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>

class TLog
{
public:
	static TLog& Get()
	{
		static TLog tLog;
		return tLog;
	}
public:
	TLog();
	virtual ~TLog();
	void log(const std::string& entry);
protected:
	void processEntries();
protected:
	bool					m_bThreadStarted;
	bool					m_bExit;
	std::mutex				m_hMutex;
	std::condition_variable m_CondVar;
	std::queue<std::string> m_szQueue;
	std::thread				m_hThread;
	std::mutex				m_hMutexStarted;
	std::condition_variable m_CondVarStarted;
	std::string				m_szLogFilePath;
private:
	TLog(const TLog& src);
	TLog& operator=(const TLog& rhs);
};


