#pragma once
#include <Windows.h>
#include <map>

enum TFSMState
{
	STATE_STAND = 0,
	STATE_MOVE,
	STATE_ATTACK,
	STATE_COUNT,
};
enum TFSMEvent
{
	EVENT_STOPMOVE = 0,
	EVENT_TIMEMOVE,
	EVENT_POINTMOVE,
	EVENT_FINDTARGET,
	EVENT_LOSTTARGET,
	EVENT_COUNT,
};

class TFiniteState
{
public:
	//      이벤트, 결과상태
	std::map<DWORD, DWORD> m_State;
public:
	void  AddTransition(DWORD dwEvent, DWORD outState);
	DWORD GetTransition(DWORD dwEvent)
	{
		return m_State[dwEvent];
	}
};

