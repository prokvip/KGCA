#include "TFsm.h"
void  TFsm::AddTransition(DWORD inState,
	DWORD dwEvent,
	DWORD outState)
{
	TFiniteState* pState = nullptr;
	auto ret = m_State.find(inState);
	if (ret != m_State.end())
	{
		pState = ret->second;
	}
	else
	{
		pState = new TFiniteState();
		m_State[inState] = pState;
	}
	pState->AddTransition(dwEvent, outState);
}

DWORD TFsm::GetTransition(	DWORD dwState,
							DWORD dwEvent)
{
	return m_State[dwState]->GetTransition(dwEvent);
}

TFsm::~TFsm()
{
	for (auto list : m_State)
	{
		delete list.second;
	}
	m_State.clear();
}