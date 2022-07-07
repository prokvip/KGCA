#pragma once
#include "TState.h"
class TFsmState
{
public:
	DWORD  m_dwState;
	std::map<DWORD, DWORD> m_mapEvent;
	TFsmState(DWORD state) : m_dwState(state)
	{

	}
};
class TFsm : public TSingleton<TFsm>
{
	friend class TSingleton<TFsm>;
public:
	std::map<DWORD, TFsmState*> m_mapStateClass;	
	void AddTransition(DWORD, DWORD, DWORD);
	DWORD GetTransition(DWORD dwState, DWORD dwOutput);
};
#define I_Fsm TFsm::Get()

