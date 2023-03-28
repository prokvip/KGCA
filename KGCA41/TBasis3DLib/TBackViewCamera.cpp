#include "TBackViewCamera.h"
#include "TInput.h"
#include "TTimer.h"
// �߰�
TMatrix TBackViewCamera::SetViewMatrix( TVector3 vPos, TVector3 vTarget, TVector3 vUp )
{
    m_vDefaultEye		= m_vCameraPos = vPos;
    m_vDefaultLookAt	= m_vTargetPos = vTarget;

	//m_vCameraDestination = m_vCameraPos;

	// Propogate changes to the member arcball
    TQuaternion quat;
    TMatrix mRotation;

    D3DXMatrixLookAtLH( &mRotation, &vPos, &vTarget, &vUp );
    D3DXQuaternionRotationMatrix( &quat, &mRotation );
    m_ViewArcBall.SetQuatNow( quat );

    // Set the radius according to the distance
    TVector3 vEyeToPoint;
    D3DXVec3Subtract( &vEyeToPoint, &vTarget, &vPos );
    SetRadius( D3DXVec3Length( &vEyeToPoint ) );
	return UpdateVector();	
}

TMatrix TBackViewCamera::Update( float fElapseTime )
{
	if( g_InputData.bWKey ) 	MoveLook( fElapseTime * 2.0f);
	if( g_InputData.bSKey )		MoveLook(-fElapseTime * 2.0f);
	if( g_InputData.bDKey )		MoveSide( fElapseTime * 2.0f);
	if( g_InputData.bAKey )		MoveSide(-fElapseTime * 2.0f);
	if( g_InputData.bQKey )		MoveUp(   fElapseTime * 2.0f);
	if( g_InputData.bEKey )		MoveUp(  -fElapseTime * 2.0f);

   // Change the radius from the camera to the model based on wheel scrolling
    if( m_nMouseWheelDelta && m_nZoomButtonMask == MOUSE_WHEEL )
	{
		m_fPrevRadius = m_fRadius;
		m_fRadius += (m_nMouseWheelDelta / 120.0f);
	}
    m_fRadius = __min( m_fMaxRadius, m_fRadius );
    m_fRadius = __max( m_fMinRadius, m_fRadius );
   

    //// Get the inverse of the arcball's rotation matrix
    TMatrix mCameraRot;
    D3DXMatrixInverse( &mCameraRot, NULL, m_ViewArcBall.GetRotationMatrix() );

    //// Transform vectors based on camera's rotation matrix
    TVector3 vWorldUp, vWorldAhead;
    TVector3 vLocalUp    = TVector3(0,1,0);
    TVector3 vLocalAhead = TVector3(0,0,1);
    D3DXVec3TransformCoord( &vWorldUp, &vLocalUp, &mCameraRot );
    D3DXVec3TransformCoord( &vWorldAhead, &vLocalAhead, &mCameraRot );

	
	// ī�޶� ȸ���� ����Ǹ� ��ǥ ��ġ�� �����Ѵ�.
	if( m_ViewArcBall.m_bDrag )
	{
		m_vCameraDestination = m_vCameraPos = m_vTargetPos - vWorldAhead * m_fRadius;
	}
	// ī�޶��� �ޱ��� ���������� �����Ѵ�.
	D3DXVec3Lerp(&m_vLerpUpVector, &vWorldUp, &vLocalUp, min(fElapseTime*3.0f, 1.0f));

	// ���� �̵��������� ����Ͽ� ��ǥ��ġ�� �����Ѵ�.
	if( m_nMouseWheelDelta)	
	{
		TVector3 vTmp;
		D3DXVec3Normalize( &vTmp, &(m_vCameraPos - m_vTargetPos ) );		
		m_vCameraDestination = m_vTargetPos + (vTmp * m_fRadius);
	}

    // õõ�� ������� ī�޶�.	
	D3DXVec3Lerp(	&m_vCameraPos,
					&m_vCameraPos,
					&m_vCameraDestination, min(fElapseTime, 1.0f) );

	if (!m_ViewArcBall.m_bDrag)
	{
		SetViewMatrix(m_vCameraPos, m_vTargetPos, m_vLerpUpVector);
	}
	//  �¿� �յڸ� �����δ�.
	//else
	//{/	
	//	D3DXMatrixLookAtLH(&m_matView, &m_vCameraPos, &m_vTargetPos, &vWorldUp);
	//}
	D3DXMatrixLookAtLH(&m_matView, &m_vCameraPos, &m_vTargetPos, &vWorldUp);

    TMatrix mInvView;
    D3DXMatrixInverse( &mInvView, NULL, &m_matView );
    mInvView._41	= mInvView._42 = mInvView._43 = 0;

    TMatrix mModelLastRotInv;
    D3DXMatrixInverse(&mModelLastRotInv, NULL, &m_mModelLastRot);

	//�� �������� arcballȸ���� ���氪�� �����Ѵ�.
	TMatrix mModelRot;
    mModelRot		= *m_WorldArcBall.GetRotationMatrix();
    m_mModelRot		*= m_matView * mModelLastRotInv * mModelRot * mInvView;

    m_mCameraRotLast= mCameraRot; 
    m_mModelLastRot = mModelRot;

    // ȸ�� ����� lookAt ��ġ�� ������ ��ġ�� ��ȯ�Ѵ�.
    m_mModelRot._41 = m_vTargetPos.x;
    m_mModelRot._42 = m_vTargetPos.y;
    m_mModelRot._43 = m_vTargetPos.z;

    // ���� �߽ɿ��� ������� ��ȯ�Ѵ�.
    TMatrix mTrans;
    D3DXMatrixTranslation( &mTrans, -m_vModelCenter.x, -m_vModelCenter.y, -m_vModelCenter.z );
    m_matWorld		= mTrans * m_mModelRot;

	m_vObjectVector[0].x = m_matWorld._11;
	m_vObjectVector[0].y = m_matWorld._21;
	m_vObjectVector[0].z = m_matWorld._31;	
	m_vObjectVector[1].x = m_matWorld._12;
	m_vObjectVector[1].y = m_matWorld._22;
	m_vObjectVector[1].z = m_matWorld._32;	
	m_vObjectVector[2].x = m_matWorld._13;
	m_vObjectVector[2].y = m_matWorld._23;
	m_vObjectVector[2].z = m_matWorld._33;	

	// ���콺 �ٰ� �ʱ�ȭ
	m_nMouseWheelDelta	= 0;
	m_fPrevRadius		= m_fRadius;
	return UpdateVector();
}
void TBackViewCamera::MoveSide(float fValue)
{
	MovementTarget(fValue, m_vObjectVector[0]);
}
void TBackViewCamera::MoveUp(float fValue)
{
	MovementTarget(fValue, m_vObjectVector[1]);
}
void TBackViewCamera::MoveLook(float fValue)
{
	MovementTarget(fValue, m_vObjectVector[2]);
}
//�̵������� + or - �� �Ǵ��Ͽ� �̵������� ������ �����Ѵ�.
float TBackViewCamera::GetDecideDirection(float fValue)
{
	float fDecideDirection = -m_vOffsetDistance.z;
	if (fValue < 0) fDecideDirection = m_vOffsetDistance.z;
	return fDecideDirection;
}
void TBackViewCamera::MovementTarget(float fValue, TVector3 vDirection)
{
	m_vTargetPos += vDirection * fValue;
	m_vCameraDestination = m_vTargetPos + (vDirection * GetDecideDirection(fValue));
	m_vCameraDestination.y += m_vTargetPos.y + m_vOffsetDistance.y;
}
void TBackViewCamera::SetOffsetDistance(float fDistX, float fDistY, float fDistZ)
{
	m_vOffsetDistance.x = fDistX;
	m_vOffsetDistance.y = fDistY;
	m_vOffsetDistance.z = fDistZ;
}


TBackViewCamera::TBackViewCamera()
{			
	m_fRadius				= 5.0f;
    m_fDefaultRadius		= 5.0f;
    m_fMinRadius			= 1.0f;
    m_fMaxRadius			= FLT_MAX;
	
	m_vLerpUpVector = TVector3(0, 1, 0);
	SetOffsetDistance(0.0f, 5.0f, 15.0f);

	m_vModelCenter			= TVector3(0,0,0);
	m_nRotateModelButtonMask  = MOUSE_LEFT_BUTTON;
    m_nZoomButtonMask         = MOUSE_WHEEL;
    m_nRotateCameraButtonMask = MOUSE_RIGHT_BUTTON;
}
TBackViewCamera::~TBackViewCamera()
{	
}

