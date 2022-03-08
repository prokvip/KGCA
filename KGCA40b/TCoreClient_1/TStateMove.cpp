#include "TStateMove.h"
TVector2 Fun(TVector2 c0, TVector2 c1, TVector2 c2, float t )
{
	// { (t*t -2t+1)c0 }  + {(-2(t*t)+2t)c1} + {(t*t)c2}
	float t2 = t * t;
	TVector2 vC0 = c0* (t2 - 2*t + 1);
	TVector2 vC1 = c1* (-2*t2+2*t);
	TVector2 vC2 = c2 * t2;
	return vC0 + vC1 + vC2;
}
void	TStateMove::Process(TObject2D* pPlayer)
{
	m_pOwner->m_fStopTimer -= g_fSecPerFrame;
	TVector2 vPC = pPlayer->m_vPos;
	TVector2 vNPC = m_pOwner->m_vPos;
	float fDistance = (vPC - vNPC).Length();
	/*if (fDistance < 200.0f)
	{
		m_pOwner->SetTransition(STATE_FIND_TARGET);	
		m_pOwner->ResetTimer();
		return;
	}*/
	/*if (m_pOwner->m_fStopTimer <= 0.0f)
	{
		m_pOwner->SetTransition(STATE_STOP_TIMER);		
		m_pOwner->ResetTimer();
		return;
	}*/	
	
	if(TCollision::SphereToPoint(m_TargetSphere[m_iPathIndex], m_pOwner->m_vPos))
	{
		m_iPathIndex = rand() % m_TargetSphere.size();
		if (m_iPathIndex >= m_TargetSphere.size() - 1)
		{
			m_iPathIndex = 0;
		}
	}
	
	if (m_bCurve)
	{
		static float t = 0.0f;
		t += g_fSecPerFrame * 0.33f;
		static TVector2 vStart = m_pOwner->m_vPos;
		if (t >= 1.0f)
		{
			vStart = m_TargetSphere[c1].vCenter;
			c0 = min(c0 + 2, m_TargetSphere.size() - 1);
			c1 = min(c1 + 2, m_TargetSphere.size() - 1);
			t = 0.0f;
			if (c0 == c1)
			{
				m_bCurve = false;
			}
		}
		TVector2 vPos = Fun(
			vStart,
			m_TargetSphere[c0].vCenter,
			m_TargetSphere[c1].vCenter,
			t);
		m_pOwner->SetPosition(vPos);
	}
	else
	{
		TVector2 vTarget = m_TargetSphere[m_iPathIndex].vCenter;
		TVector2 vDir = vTarget - m_pOwner->m_vPos;
		vDir.Normalize();
		//m_pOwner->AddPosition(vDir * g_fSecPerFrame*200.0f);
		TVector2 vPos = m_pOwner->m_vPos + vDir * g_fSecPerFrame * 200.0f;
		m_pOwner->SetPosition(vPos);
	}
}