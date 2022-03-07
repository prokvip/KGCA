#include "TStateMove.h"
void	TStateMove::Process(TObject2D* pPlayer)
{
	m_pOwner->m_fStopTimer += g_fSecPerFrame;
	TVector2 vPC = pPlayer->m_vPos;
	TVector2 vNPC = m_pOwner->m_vPos;
	float fDistance = (vPC - vNPC).Length();
	if (fDistance < 200.0f)
	{
		m_pOwner->SetTransition(STATE_FIND_TARGET);	
		m_pOwner->m_fStopTimer = 0.0f;
		return;
	}
	if (m_pOwner->m_fStopTimer > 3.0f)
	{
		m_pOwner->SetTransition(STATE_STOP_TIMER);
		m_pOwner->m_fStopTimer = 0.0f;
		return;
	}
	m_pOwner->AddPosition(TVector2(30 * g_fSecPerFrame, 30 * g_fSecPerFrame));
}