#pragma once
#include "TFiniteState.h"
class TFsm
{
private:
	//       상태(동작), 해당 클래스
	std::map<DWORD, TFiniteState*> m_State;
public:
	void  AddTransition(DWORD inState, 
						DWORD event, 
						DWORD outState);
	DWORD GetTransition(DWORD dwState,
						DWORD dwEvent);

	~TFsm();
};

