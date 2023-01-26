#include "TFiniteState.h"
void  TFiniteState::AddTransition(DWORD dwEvent, DWORD outState)
{
	m_State[dwEvent] = outState;
}