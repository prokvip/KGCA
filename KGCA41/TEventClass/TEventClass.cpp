// TEventClass.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "TRunnable.h"

#define  MAX_THREAD 3
std::vector<int>			g_EventList;
std::vector<std::thread>	g_ThreadList;
int   g_iThreadInstanceData[MAX_THREAD] = { 0, };
T::TEventHandler g_Handler;
//     frame  render
// map   0    4
// char  1    5
// event 2    6
void PerSceneRenderDeferredProc(int& parameter)
{
	int iInstance = parameter;
	while (1)
	{
		// Frame() 통지 대기
		int iRet = g_Handler.WaitForSingleObject(iInstance, 1000);
		if (iRet == 0)
		{
			std::cout << "Run! threadID=" << iInstance << std::endl;
		}
		else if (iRet == -1)
		{
			std::cout << "Timeout! threadID=" << iInstance << std::endl;
			continue;
		}
		else
		{
			break; // error
		}
		
		//..계산.. commandlist 완성
		
		g_Handler.ResetEvent(iInstance);

		// Render() 이벤트 통지
		g_Handler.SetEvent(MAX_THREAD+iInstance);
	}
}
class TThread : public T::TRunnable
{
public:
	virtual void Run()
	{
		int iCnt = 0;
		while (!m_bStop)
		{
			std::cout << "running" << iCnt++ << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}
};


int main()
{
	std::cout.sync_with_stdio(false);
	//Init()
	for (int iThread = 0; iThread < MAX_THREAD; iThread++)
	{
		g_iThreadInstanceData[iThread] = iThread;
		g_EventList.push_back(g_Handler.CreateEvent(std::to_string(iThread)));
		g_ThreadList.emplace_back( PerSceneRenderDeferredProc,
									std::ref(g_iThreadInstanceData[iThread]));
		// 백그라운드(UI) 쓰레드
		g_ThreadList[g_ThreadList.size() - 1].detach();
		//CloseHandel(CreateThread());
	}
	g_EventList.push_back(g_Handler.CreateEvent("render_0"));
	g_EventList.push_back(g_Handler.CreateEvent("render_1"));
	g_EventList.push_back(g_Handler.CreateEvent("render_2"));
	
	// run
	while (1)
	{
		// Frame
		for (int iThread = 0; iThread < MAX_THREAD; iThread++)
		{
			//std::this_thread::sleep_for(std::chrono::seconds(1));
			g_Handler.SetEvent(iThread);
		}
		
		// Render
		int iEventArray[] = { MAX_THREAD + 0,
								MAX_THREAD + 1,
								MAX_THREAD + 2 };
		int iReturn = g_Handler.WaitForMultipleObject(3,iEventArray,true);
		iReturn -= MAX_THREAD;

		for (int iThread = 0; iThread < MAX_THREAD; iThread++)
		{
			// commendlist execute->Render
			std::cout << "Render! ID=" << iThread << std::endl;
			g_Handler.ResetEvent(MAX_THREAD+iThread);
		}
		std::this_thread::sleep_for(std::chrono::seconds(3));
	}

	TThread th1;
	th1.start();
	while (1)
	{
		th1.suspend();
		std::this_thread::sleep_for(std::chrono::seconds(5));		
		th1.resume();
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
    std::cout << "Hello World!\n";
}
