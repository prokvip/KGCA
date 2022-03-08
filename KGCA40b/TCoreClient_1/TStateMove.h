#pragma once
#include "TState.h"
class TStateMove : public TState
{
public:
	std::vector<TSphere>   m_TargetSphere;
	int	m_iPathIndex=0;
	int c0 = 0;
	int c1 = 1;
	bool m_bCurve=true;
public:
	virtual void	Process(TObject2D* pPlayer) override;
	TStateMove(TObjectAI* owner) : TState(owner)
	{
		//m_TargetSphere.vCenter.x = rand() % g_rtClient.right;
		//m_TargetSphere.vCenter.y = rand() % g_rtClient.bottom;
		TSphere v;
		v.vCenter.x = 400.0f;v.vCenter.y = 600.0f;
		m_TargetSphere.push_back(v);
		v.vCenter.x = 800.0f;v.vCenter.y = 0.0f;
		m_TargetSphere.push_back(v);
		v.vCenter.x = 0.0f;	v.vCenter.y = 300.0f;
		m_TargetSphere.push_back(v);
		v.vCenter.x = 800.0f;v.vCenter.y = 600.0f;
		m_TargetSphere.push_back(v);
		v.vCenter.x = 400.0f;v.vCenter.y = 0.0f;
		m_TargetSphere.push_back(v);
		v.vCenter.x = 0.0f;v.vCenter.y = 600.0f;
		m_TargetSphere.push_back(v);
		v.vCenter.x = 800.0f;v.vCenter.y = 300.0f;
		m_TargetSphere.push_back(v);
		v.vCenter.x = 0.0f;	v.vCenter.y = 0.0f;
		m_TargetSphere.push_back(v);
	}
};

