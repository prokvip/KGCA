#pragma once
#include "TFiniteState.h"
class TFsm
{
private:
	//       ����(����), �ش� Ŭ����
	std::map<DWORD, TFiniteState*> m_State;
public:
	void  AddTransition(DWORD inState, 
						DWORD event, 
						DWORD outState);
	DWORD GetTransition(DWORD dwState,
						DWORD dwEvent);

	~TFsm();
};

