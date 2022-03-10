#include "TStateAttack.h"
#include "TFsm.h"
void	TStateAttack::Process(TObject2D* pPlayer)
{
	// ������ npc �Ÿ� ��� > ������
	TVector2 vPC = pPlayer->m_vPos;
	TVector2 vNPC = m_pOwner->m_vPos;
	float fDistance = (vPC - vNPC).Length();
	if (fDistance > 300.0f)
	{
		m_pOwner->SetTransition(STATE_LOST_TARGET);
		return;
	}
	TVector2 vAdd = (vPC - vNPC).Normal() * g_fSecPerFrame* 300.0f;
	m_pOwner->AddPosition(vAdd);	
}