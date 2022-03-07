#include "TFsm.h"
void TFsm::AddTransition(DWORD dwState, DWORD dwEvent, DWORD dwOutput)
{
	TFsmState* pStateFsm = nullptr;
	auto iter = m_mapStateClass.find(dwState);
	if (iter != m_mapStateClass.end())
	{
		pStateFsm = iter->second;
	}
	else
	{
		pStateFsm = new TFsmState(dwState);
		m_mapStateClass.insert(std::make_pair(dwState, pStateFsm));
	}
	pStateFsm->m_mapEvent.insert(std::make_pair(dwEvent, dwOutput));
}
DWORD TFsm::GetTransition(DWORD dwState, DWORD dwEvent)
{
	TFsmState* pStateFsm = nullptr;
	auto iterState = m_mapStateClass.find(dwState);
	if (iterState == m_mapStateClass.end())
	{
		//error
	}
	pStateFsm = iterState->second;
	auto iterOutput = pStateFsm->m_mapEvent.find(dwEvent);
	if (iterOutput == pStateFsm->m_mapEvent.end())
	{
		// error		
	}
	return iterOutput->second;
}