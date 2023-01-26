#include "TNpc.h"
#include "TPlayer.h"
void TStandState::Process(TPlayer* player)
{
	std::cout << "TStandState!\n";
	if (rand() % 2 == 0)
	{
		m_pOwner->SetTransition(EVENT_TIMEMOVE);
	}
	if (rand() % 2 == 1)
	{
		m_pOwner->SetTransition(EVENT_FINDTARGET);
	}
}
void TMoveState::Process(TPlayer* player)
{
	std::cout << "TMoveState!\n";
	if (rand() % 100 < 5)
	{
		m_pOwner->SetTransition(EVENT_STOPMOVE);
	}
}
void TAttackState::Process(TPlayer* player)
{
	std::cout << "TAttackState!\n";
	if (rand() % 100 < 5)
	{
		m_pOwner->SetTransition(EVENT_LOSTTARGET);
	}
}
TNpc::TNpc(TFsm* fsm)
{
	m_pFsm = fsm;
	m_pActionList.push_back(new TStandState(this));
	m_pActionList.push_back(new TMoveState(this));
	m_pActionList.push_back(new TAttackState(this));
	m_pCurentState = m_pActionList[0];
	m_dwState = STATE_STAND;
}
void TNpc::Process(TPlayer* player)
{
	m_pCurentState->Process(player);
}
void TNpc::SetTransition(DWORD dwEvent)
{
	DWORD dwOutput = m_pFsm->GetTransition(m_dwState, dwEvent);
	m_pCurentState = m_pActionList[dwOutput];
}
TNpc::~TNpc()
{
	for (auto list : m_pActionList)
	{
		delete list;
	}
	m_pActionList.clear();
}