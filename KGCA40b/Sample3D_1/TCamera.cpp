#include "TCamera.h"
#include "TInput.h"



bool TCamera::Init()
{
	T::D3DXMatrixLookAtLH(&m_matView, &m_vCamera, &m_vTarget, &m_vUp);
	//m_matView.CreateViewLook(m_vCamera, m_vTarget, m_vUp);
	// Projection
	T::D3DXMatrixPerspectiveFovLH(&m_matProj,
		XM_PI * 0.25f,
		(float)g_rtClient.right / (float)g_rtClient.bottom,
		0.1f, 5000.0f);
	return true;
}
// vValue.x : pitch, y=yaw, z= roll, w =radius
T::TMatrix TCamera::Update(T::TVector4 vDirValue)
{
	T::TMatrix matRotation;
	T::D3DXQuaternionRotationYawPitchRoll(&m_qRotation,
		vDirValue.y,
		vDirValue.x,
		vDirValue.z);

	//m_vCameraPos += m_vLookVector * vDirValue.w;
	//m_fRadius += vDirValue.w;

	//T::D3DXMatrixAffineTransformation(&matRotation, 1.0f, NULL, &m_qRotation, &m_vCameraPos);
	//T::D3DXMatrixInverse(&m_matView, NULL, &matRotation);

	//return UpdateVector();
	return matRotation;
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
	T::D3DXMatrixLookAtLH(&m_matView, &m_vCamera, &m_vTarget, &m_vDefaultUp);
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
	m_vUp = m_vDefaultUp = T::TVector3(0, 1, 0);
}