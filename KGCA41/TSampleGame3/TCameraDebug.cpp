#include "TCameraDebug.h"
#include "TInput.h"
void TCameraDebug::CreateViewMatrix(TVector vEye, TVector vAt, TVector vUp)
{
	m_vPos = vEye;
	m_vTarget = vAt;
	m_vUp = vUp;
	m_matView.ViewLookAt(vEye, vAt, vUp);
}
void TCameraDebug::CreateProjMatrix(float fNear, float fFar, float fFovY, float fAspectRatio)
{
	m_fNear = fNear;
	m_fFar = fFar;
	m_fFovY = fFovY;
	m_fAspectRatio = fAspectRatio;
	PerspectiveFovLH(m_matProj, m_fNear, m_fFar, m_fFovY, m_fAspectRatio);
	//matProj.OrthoLH(800, 600, 0.0f, 100.0f);
	//OrthoOffCenterLH(matProj ,-400, 400, -300, 300, 0.0f, 100.0f);
}
bool TCameraDebug::Frame()
{
	if (I_Input.GetKey('W') == KEY_HOLD)
	{
		TVector v = m_vLook * 10.0f * g_fSecondPerFrame;
		m_vPos += v;
	}
	if (I_Input.GetKey('S') == KEY_HOLD)
	{
		TVector v = m_vLook * -10.0f * g_fSecondPerFrame;
		m_vPos += v;
	}
	if (I_Input.GetKey('A') == KEY_HOLD)
	{
		TVector v = m_vRight * 10.0f * g_fSecondPerFrame;
		m_vPos += v;
	}
	if (I_Input.GetKey('D') == KEY_HOLD)
	{
		TVector v = m_vRight * -10.0f * g_fSecondPerFrame;
		m_vPos += v;
	}
	if (I_Input.GetKey('Q') == KEY_HOLD)
	{
		TVector v = m_vUp * 10.0f * g_fSecondPerFrame;
		m_vPos += v;
	}
	if (I_Input.GetKey('E') == KEY_HOLD)
	{
		TVector v = m_vUp * -10.0f * g_fSecondPerFrame;
		m_vPos += v;
	}

	//TVector vUp = { 0,1,0 };
	////TMatrix mCamera = TMath::RotationY(g_fGameTimer);
	////vPos = (vPos + vPosMovement) * mCamera;
	//m_matView.ViewLookAt(m_vPos, m_vTarget, m_vUp);

	Update();
	return true;
}