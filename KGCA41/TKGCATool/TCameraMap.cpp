#include "pch.h"
#include "TCameraMap.h"
#include "TInput.h"
#include "TMath.h"

void  TCameraMap::SetPos(TVector3 vTargetPos, TVector3 vMapHeight)
{
	TVector3 vCamera = TVector3(0, 10, -10);
	TVector3 vTarget = TVector3(0, 0, 0);
	
	vCamera = vTargetPos + vCamera;	
	vCamera.y = vMapHeight.y;
	m_vTarget = vTargetPos;
	m_vBeforePos = vTargetPos;

	m_vPos= vCamera;
	m_vPos.y = max(vTargetPos.y, m_vPos.y) + 10.0f;	
}
void TCameraMap::CreateViewMatrix(TVector3 vEye, TVector3 vAt, TVector3 vUp)
{
	m_vPos = vEye;
	m_vTarget = vAt;
	m_vUp = vUp;
	D3DXMatrixLookAtLH(&m_matView, &vEye, &vAt, &vUp);
	//m_matView.ViewLookAt(vEye, vAt, vUp);
}
void TCameraMap::CreateProjMatrix(float fNear, float fFar, float fFovY, float fAspectRatio)
{
	m_fNear = fNear;
	m_fFar = fFar;
	m_fFovY = fFovY;
	m_fAspectRatio = fAspectRatio;
	D3DXMatrixPerspectiveFovLH(&m_matProj, fFovY, fAspectRatio, fNear, fFar);
	//PerspectiveFovLH(m_matProj, m_fNear, m_fFar, m_fFovY, m_fAspectRatio);
	//matProj.OrthoLH(800, 600, 0.0f, 100.0f);
	//OrthoOffCenterLH(matProj ,-400, 400, -300, 300, 0.0f, 100.0f);
}
bool TCameraMap::Frame()
{
	TVector3 r = m_vRight;
	r.y = 0;
	D3DXVec3Normalize(&r, &r);

	if (I_Input.GetKey('A') == KEY_HOLD)
	{
		TVector3 v = -r * g_fSecondPerFrame * 10.0f;
		m_vPos += v;
	}
	if (I_Input.GetKey('D') == KEY_HOLD)
	{
		TVector3 v = r * g_fSecondPerFrame * 10.0f;
		m_vPos += v;
	}

	TCamera::Frame();	
	return true;
}
void TCameraMap::Update()
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


	D3DXVec3Normalize(&m_vRight, &m_vRight);
	D3DXVec3Normalize(&m_vUp, &m_vUp);
	D3DXVec3Normalize(&m_vLook, &m_vLook);

	m_vFrustum.CreateFrustum(&m_matView, &m_matProj);
}