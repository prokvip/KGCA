#include "TCamera.h"
#include "TInput.h"
bool TCamera::Init()
{
	m_matView.CreateViewLook(m_vCamera, m_vTarget, m_vUp);
	// Projection
	m_matProj.PerspectiveFovLH(1.0f, 5000.0f,
		TBASIS_PI * 0.25f,
		(float)g_rtClient.right / (float)g_rtClient.bottom);
	return true;
}
bool TCamera::Frame()
{
	/*if (TInput::Get().GetKey(VK_LEFT))
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
	}*/
	m_matView.CreateViewLook(m_vCamera, m_vTarget, m_vDefaultUp);
	m_vLight.x = m_matView._11; 	
	m_vLight.y = m_matView._21;
	m_vLight.z = m_matView._31;
	m_vUp.x = m_matView._12;
	m_vUp.y = m_matView._22;
	m_vUp.z = m_matView._32;
	m_vLook.x = m_matView._13;
	m_vLook.y = m_matView._23;
	m_vLook.z = m_matView._33;

	return true;
}
TCamera::TCamera()
{
	m_vCamera.x = 0.0f;
	m_vCamera.y = 9.0f;
	m_vCamera.z = -15.0f;
	m_vDefaultUp = TVector3(0, 1, 0);
}