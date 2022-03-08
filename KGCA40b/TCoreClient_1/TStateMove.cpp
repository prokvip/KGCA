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
	
	if(TCollision::SphereToPoint(m_TargetSphere[m_iPathIndex], m_pOwner->m_vPos))
	{
		m_iPathIndex = rand() % m_TargetSphere.size();
		if (m_iPathIndex >= m_TargetSphere.size() - 1)
		{
			m_iPathIndex = 0;
		}
	}
	TVector2 vTarget = m_TargetSphere[m_iPathIndex].vCenter;
	TVector2 vDir = vTarget - m_pOwner->m_vPos;
	vDir.Normalize();
	m_pOwner->AddPosition(vDir * g_fSecPerFrame*200.0f);
}