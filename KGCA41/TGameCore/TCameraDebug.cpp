#include "TCameraDebug.h"
#include "TInput.h"
#include "TMath.h"
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
	if (I_Input.GetKey(VK_SPACE) == KEY_HOLD)
	{
		m_fSpeed += g_fSecondPerFrame * 100.0f;
	}
	else
	{
		m_fSpeed -= g_fSecondPerFrame * 100.0f;
	}
	m_fSpeed = max(10.0f, m_fSpeed);
	m_fSpeed = min(100.0f, m_fSpeed);

	if (I_Input.GetKey('W') == KEY_HOLD)
	{
		TVector v = m_vLook * m_fSpeed * g_fSecondPerFrame;
		m_vPos += v;
	}
	if (I_Input.GetKey('S') == KEY_HOLD)
	{
		TVector v = m_vLook * -m_fSpeed * g_fSecondPerFrame;
		m_vPos += v;
	}
	if (I_Input.GetKey('A') == KEY_HOLD)
	{
		TVector v = m_vRight * m_fSpeed * g_fSecondPerFrame;
		m_vPos += v;
	}
	if (I_Input.GetKey('D') == KEY_HOLD)
	{
		TVector v = m_vRight * -m_fSpeed * g_fSecondPerFrame;
		m_vPos += v;
	}
	if (I_Input.GetKey('Q') == KEY_HOLD)
	{
		TVector v = m_vUp * m_fSpeed * g_fSecondPerFrame;
		m_vPos += v;
	}
	if (I_Input.GetKey('E') == KEY_HOLD)
	{
		TVector v = m_vUp * -m_fSpeed * g_fSecondPerFrame;
		m_vPos += v;
	}
	

	TBASIS_EX::TVector3 vPos;
	vPos.x = m_vPos.x;
	vPos.y = m_vPos.y;
	vPos.z = m_vPos.z;

	//////////////////////////// DebugCamera ////////////////////////////
	TBASIS_EX::TMatrix matWorld;
	TBASIS_EX::TMatrix matView;
	TBASIS_EX::TMatrix matRotation;
	TBASIS_EX::TQuaternion m_qRotation;
	TBASIS_EX::D3DXQuaternionRotationYawPitchRoll(&m_qRotation, m_fYaw, m_fPitch, m_fRoll);
	TBASIS_EX::D3DXMatrixAffineTransformation(&matWorld,1.0f, NULL, &m_qRotation, &vPos);
	TBASIS_EX::D3DXMatrixInverse(&matView, NULL, &matWorld);
	m_matView = *((TMatrix*)&matView);
	
	//////////////////////////// Model View////////////////////////////
	/*TVector vLocalUp = { 0.0f, 1.0f, 0.0f };
	TVector vLocalLook = { 0.0f, 0.0f, 1.0f };

	TBASIS_EX::TMatrix matRotation;
	TBASIS_EX::D3DXMatrixRotationYawPitchRoll(
		&matRotation, m_fYaw, 0, m_fRoll);
	TMatrix* matR = (TMatrix*)&matRotation;
	
	TVector vWorldLook = vLocalLook * (*matR);
	TVector vWorldUp = vLocalUp * (*matR);
	TVector vWorld = vWorldLook * 10.0f;
	m_vPos = m_vTarget - vWorld;
	m_matView.ViewLookAt(m_vPos, m_vTarget, m_vUp);*/

	Update();
	return true;
}

void TCameraDebug::Update()
{
	m_vRight.x = m_matView._11;
	m_vRight.y = m_matView._21;
	m_vRight.z = m_matView._31;

	m_vUp.x = m_matView._12;
	m_vUp.y = m_matView._22;
	m_vUp.z = m_matView._32;

	m_vLook.x = m_matView._13;
	m_vLook.y = m_matView._23;
	m_vLook.z = m_matView._33;

	m_vRight.Normalized();
	m_vUp.Normalized();
	m_vLook.Normalized();

	m_vFrustum.CreateFrustum(&m_matView, &m_matProj);
}