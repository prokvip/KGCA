#include "TDebugCamera.h"
bool  TDebugCamera::Init()
{
	TCamera::Init();
	return true;
}
bool  TDebugCamera::Frame()
{
	TVector3 vOffset = m_vSide * g_fSecondPerFrame * m_fSpeed;
	m_vCameraPos = m_vCameraPos + vOffset;
	//m_vTargetPos = m_vTargetPos + vOffset;
	CreateLookAt(m_vCameraPos, m_vTargetPos);
	return true;
}
bool  TDebugCamera::Render()
{
	TCamera::Render();
	return true;
}
bool  TDebugCamera::Release()
{
	TCamera::Release();
	return true;
}