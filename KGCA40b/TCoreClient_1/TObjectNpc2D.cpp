#include "TObjectNpc2D.h"
#include "TFsm.h"
bool	TObjectNpc2D::Init()
{
	m_pStateArray[STATE_STOP] = new TStateStop(this);
	m_pStateArray[STATE_MOVE] = new TStateMove(this);
	m_pStateArray[STATE_ATTACK] = new TStateAttack(this);
	m_pStateAction = m_pStateArray[STATE_STOP];
	m_dwStateAction = m_pStateAction->m_dwStateAI;
	return true;
}

void	TObjectNpc2D::SetTransition(DWORD dwEvent)
{
	m_dwStateAction = I_Fsm.GetTransition(m_dwStateAction, dwEvent);
	m_pStateAction = m_pStateArray[m_dwStateAction];
}
bool	TObjectNpc2D::Frame()
{
	//m_pState = m_pStateArray[m_dwStateAI];
	//m_vPos += TVector2(10 * g_fSecPerFrame, 10 * g_fSecPerFrame);
	//SetPosition(m_vPos);
	//AddPosition(TVector2(30 * g_fSecPerFrame, 30 * g_fSecPerFrame));
	
	return true;
}