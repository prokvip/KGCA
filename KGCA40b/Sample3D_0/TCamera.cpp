#include "TCamera.h"
#include "TInput.h"

bool TCamera::Frame()
{
	if (TInput::Get().GetKey(VK_LEFT))
	{
		m_vCamera.z -= g_fSecPerFrame * 1.0f;
	}
	if (TInput::Get().GetKey(VK_RIGHT))
	{
		m_vCamera.z += g_fSecPerFrame * 1.0f;
	}
	if (TInput::Get().GetKey(VK_UP))
	{
		m_vCamera.y += g_fSecPerFrame * 1.0f;
	}
	if (TInput::Get().GetKey(VK_DOWN))
	{
		m_vCamera.y -= g_fSecPerFrame * 1.0f;
	}

	TVector3 vTarget(0, 0, 0);
	TVector3 vUp(0, 1, 0);
	m_matView.CreateViewLook(m_vCamera, vTarget, vUp);
	// Projection
	m_matProj.PerspectiveFovLH(1.0f, 100.0f,
		TBASIS_PI * 0.25f,
		(float)g_rtClient.right / (float)g_rtClient.bottom);
	return true;
}