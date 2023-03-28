#include "TLog.h"
#include <fstream>
#include <iostream>

using namespace std;

TLog::TLog() : m_bThreadStarted(false), m_bExit(false)
{
	//rmdir / s / q "..\\..\\include\\TBasis2D"
	//md "..\\..\\include\\TBasis2D"	
	system("md ..\\..\\log");

	time_t now = time(NULL);
	struct tm lt;
	localtime_s(&lt, &now);
	m_szLogFilePath.resize(100);
	strftime(&m_szLogFilePath.at(0), 100, "../../log/%Y년%m월%d일%H시%M분%S초.txt", &lt);

	m_hThread = thread{ &TLog::processEntries, this };
	unique_lock<mutex> lock(m_hMutexStarted);
	m_CondVarStarted.wait(lock, [&]() {return m_bThreadStarted == true; });
}

TLog::~TLog()
{
	m_bExit = true;
	m_CondVar.notify_all();
	m_hThread.join();
}

void TLog::log(const std::string& entry)
{
	unique_lock<mutex> lock(m_hMutex);
	m_szQueue.push(entry);
	m_CondVar.notify_all();
}

void TLog::processEntries()
{
	ofstream ofs(m_szLogFilePath);
	if (ofs.fail()) 
	{
		cerr << "Failed to open logfile." << endl;
		return;
	}

	unique_lock<mutex> lock(m_hMutex);
	m_bThreadStarted = true;
	m_CondVarStarted.notify_all();

	while (true) 
	{
		m_CondVar.wait(lock);
		lock.unlock();
		while (true) 
		{
			lock.lock();
			if (m_szQueue.empty()) 
			{
				break;
			}
			else 
			{
				ofs << m_szQueue.front() << endl;
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
