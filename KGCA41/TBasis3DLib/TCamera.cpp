#include ".\tcamera.h"

#include "TInput.h"
#include "TTimer.h"

int TCamera::WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    if( ( (uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONDBLCLK ) && m_nRotateModelButtonMask & MOUSE_LEFT_BUTTON) ||
        ( (uMsg == WM_MBUTTONDOWN || uMsg == WM_MBUTTONDBLCLK ) && m_nRotateModelButtonMask & MOUSE_MIDDLE_BUTTON) ||
        ( (uMsg == WM_RBUTTONDOWN || uMsg == WM_RBUTTONDBLCLK ) && m_nRotateModelButtonMask & MOUSE_RIGHT_BUTTON) )
    {
        int iMouseX = (short)LOWORD(lParam);
        int iMouseY = (short)HIWORD(lParam);
        m_WorldArcBall.OnBegin( iMouseX, iMouseY );
    }

    if( ( (uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONDBLCLK ) && m_nRotateCameraButtonMask & MOUSE_LEFT_BUTTON) ||
        ( (uMsg == WM_MBUTTONDOWN || uMsg == WM_MBUTTONDBLCLK ) && m_nRotateCameraButtonMask & MOUSE_MIDDLE_BUTTON) ||
        ( (uMsg == WM_RBUTTONDOWN || uMsg == WM_RBUTTONDBLCLK ) && m_nRotateCameraButtonMask & MOUSE_RIGHT_BUTTON) )
    {
        int iMouseX = (short)LOWORD(lParam);
        int iMouseY = (short)HIWORD(lParam);
        m_ViewArcBall.OnBegin( iMouseX, iMouseY );
    }

    if( uMsg == WM_MOUSEMOVE )
    {
        int iMouseX = (short)LOWORD(lParam);
        int iMouseY = (short)HIWORD(lParam);
        m_WorldArcBall.OnMove( iMouseX, iMouseY );
        m_ViewArcBall.OnMove( iMouseX, iMouseY );
    }

    if( (uMsg == WM_LBUTTONUP && m_nRotateModelButtonMask & MOUSE_LEFT_BUTTON) ||
        (uMsg == WM_MBUTTONUP && m_nRotateModelButtonMask & MOUSE_MIDDLE_BUTTON) ||
        (uMsg == WM_RBUTTONUP && m_nRotateModelButtonMask & MOUSE_RIGHT_BUTTON) )
    {
        m_WorldArcBall.OnEnd();
    }

    if( (uMsg == WM_LBUTTONUP && m_nRotateCameraButtonMask & MOUSE_LEFT_BUTTON) ||
        (uMsg == WM_MBUTTONUP && m_nRotateCameraButtonMask & MOUSE_MIDDLE_BUTTON) ||
        (uMsg == WM_RBUTTONUP && m_nRotateCameraButtonMask & MOUSE_RIGHT_BUTTON) )
    {
        m_ViewArcBall.OnEnd();
    }
	// LBUTTONUP를 다른 이벤트(윈도우키 등) 때문에 받지 못할 때 호출된다.
    if( uMsg == WM_CAPTURECHANGED )
    {
        if( (HWND)lParam != hWnd )
        {
            if( (m_nRotateModelButtonMask & MOUSE_LEFT_BUTTON) ||
                (m_nRotateModelButtonMask & MOUSE_MIDDLE_BUTTON) ||
                (m_nRotateModelButtonMask & MOUSE_RIGHT_BUTTON) )
            {
                m_WorldArcBall.OnEnd();
            }
        
            if( (m_nRotateCameraButtonMask & MOUSE_LEFT_BUTTON) ||
                (m_nRotateCameraButtonMask & MOUSE_MIDDLE_BUTTON) ||
                (m_nRotateCameraButtonMask & MOUSE_RIGHT_BUTTON) )
            {
                m_ViewArcBall.OnEnd();
            }
        }
    }

    
	if( uMsg == WM_MOUSEWHEEL )
	{
           // Update member var state
           m_nMouseWheelDelta += (short)HIWORD(wParam);
	}
    return -1;
}
void TCamera::SetButtonMasks( int nRotateModelButtonMask, int nZoomButtonMask, int nRotateCameraButtonMask ) 
{ 
	m_nRotateModelButtonMask	= nRotateModelButtonMask, 
	m_nZoomButtonMask			= nZoomButtonMask; 
	m_nRotateCameraButtonMask	= nRotateCameraButtonMask; 
}

// 박스의 크기에 따라서 화면에 꽉차게 카메라(45도 기준)가 조정된다.
TMatrix	TCamera::SetObjectView( TVector3 vMax, TVector3 vMin )
{
	TMatrix	matView;
	TVector3 vCenter = ( vMax + vMin ) * 0.5f;
	float fRadius = D3DXVec3Length( &(vMax-vMin) ) * 0.5f;	

	TVector3 vTarget = TVector3( vCenter.x, vCenter.y, vCenter.z );
	TVector3 vPos = vTarget + (-m_vLookVector *  (fRadius*2) );

	D3DXMatrixLookAtLH( &m_matView, &vPos, &vTarget , &TVector3(0.0f, 1.0f, 0.0f) );
	
	m_vCameraPos	= vPos;	
	m_vTargetPos	= vTarget;
	return m_matView;
}
TMatrix TCamera::SetViewMatrix( TVector3 vPos, TVector3 vTarget, TVector3 vUp )
{
	m_vDefaultEye		= m_vCameraPos	= vPos;	
	m_vDefaultLookAt	= m_vTargetPos	= vTarget;
	D3DXMatrixLookAtLH( &m_matView, &m_vCameraPos, &m_vTargetPos , &vUp );
	
	TQuaternion qRotation;
	TVector3 vTrans, vScale, vAxis;
	TMatrix mInvView;
	TQuaternion q;
	float fAngle;

	if (SUCCEEDED(D3DXMatrixDecompose(&vScale, &qRotation, &vTrans, &m_matView)))
	{
		D3DXQuaternionNormalize(&qRotation, &qRotation);
		D3DXQuaternionToAxisAngle(&qRotation, &vAxis, &fAngle);
		// 반드시 정규화 해야 한다.
		D3DXVec3Normalize(&vAxis, &vAxis);

		q.x = sin(fAngle / 2) * vAxis.x;
		q.y = sin(fAngle / 2) * vAxis.y;
		q.z = sin(fAngle / 2) * vAxis.z;
		q.w = cos(fAngle / 2);
	}
	D3DXMatrixInverse(&mInvView, NULL, &m_matView);
	TVector3* pZBasis = (TVector3*)&mInvView._31;

	m_fCameraYawAngle = atan2f( pZBasis->x, pZBasis->z );
	float fLen = sqrtf( pZBasis->z * pZBasis->z + pZBasis->x * pZBasis->x );
	m_fCameraPitchAngle = -atan2f( pZBasis->y, fLen );

	return 	UpdateVector();	
}
TMatrix TCamera::SetProjMatrix( FLOAT fFOV, FLOAT fAspect, FLOAT fNearPlane, FLOAT fFarPlane )
{
    m_fFOV			= fFOV;
    m_fAspect		= fAspect;
    m_fNearPlane	= fNearPlane;
    m_fFarPlane		= fFarPlane;

    D3DXMatrixPerspectiveFovLH( &m_matProj, fFOV, fAspect, fNearPlane, fFarPlane );	
	return m_matProj;
}
TMatrix TCamera::UpdateVector()
{
	m_vLookVector.x = m_matView._13;
	m_vLookVector.y = m_matView._23;
	m_vLookVector.z = m_matView._33;	
	m_vRightVector.x = m_matView._11;
	m_vRightVector.y = m_matView._21;
	m_vRightVector.z = m_matView._31;	
	m_vUpVector.x = m_matView._12;
	m_vUpVector.y = m_matView._22;
	m_vUpVector.z = m_matView._32;	
	
	D3DXVec3Normalize( &m_vLookVector, &m_vLookVector );
	
	TFrustum::SetMatrix( NULL, &m_matView, &m_matProj );
	TFrustum::CreateFrustum();	
	return m_matView;
}
TMatrix TCamera::Update( TVector4 vDirValue )
{
	TMatrix matRotation;
	D3DXQuaternionRotationYawPitchRoll(			&m_qRotation,	
												vDirValue.y,
												vDirValue.x,
												vDirValue.z );			

	m_vCameraPos += m_vLookVector * vDirValue.w;
	m_fRadius  += vDirValue.w;

	D3DXMatrixAffineTransformation( &matRotation, 1.0f, NULL, &m_qRotation, &m_vCameraPos );
	D3DXMatrixInverse( &m_matView, NULL, &matRotation );

	return UpdateVector();	
}
bool TCamera::Frame()
{
	if( g_InputData.bLeftHold )
	{
		m_fCameraYawAngle	+= XMConvertToRadians(g_InputData.iMouseValue[0] *0.1f);
		m_fCameraPitchAngle	+= XMConvertToRadians(g_InputData.iMouseValue[1] *0.1f);
	}		
	// 카메라 이동 거리 = 속도 + ( 경과시간 * 마우스 휠 변위값 )
	float fValue = (float)g_InputData.iMouseValue[2];
	float fDistance =  m_fSpeed * fValue * g_fSecPerFrame;
	
	if( g_InputData.bSpace )	m_fSpeed += g_fSecPerFrame * 10.0f;
	else						m_fSpeed -= g_fSecPerFrame * 10.0f;
	// 최소값으로 고정
	if( m_fSpeed < 1.0f ) m_fSpeed = 1.0f;

	if( g_InputData.bWKey ) 	MoveLook(g_fSecPerFrame * 5.0f * m_fSpeed);
	if( g_InputData.bSKey )		MoveLook(-g_fSecPerFrame * 5.0f* m_fSpeed);
	if( g_InputData.bDKey )		MoveSide(g_fSecPerFrame * 5.0f* m_fSpeed);
	if( g_InputData.bAKey )		MoveSide(-g_fSecPerFrame * 5.0f* m_fSpeed);
	if( g_InputData.bQKey )		MoveUp(g_fSecPerFrame * 5.0f* m_fSpeed);
	if( g_InputData.bEKey )		MoveUp(-g_fSecPerFrame * 5.0f* m_fSpeed);

	Update( TVector4( m_fCameraPitchAngle, m_fCameraYawAngle, m_fCameraRollAngle, fDistance) );
	UpdateVector();	
	return true;
}
void TCamera::MoveLook( float fValue )
{
	m_vCameraPos += m_vLookVector * fValue;
}
void TCamera::MoveSide( float fValue )
{
	m_vCameraPos += m_vRightVector * fValue;
}
void TCamera::MoveUp( float fValue )
{
	m_vCameraPos += m_vUpVector * fValue;
}
void TCamera::SetRadius( float fDefaultRadius, float fMinRadius, float fMaxRadius  ) 
{ 
  m_fDefaultRadius = m_fRadius = fDefaultRadius;
  m_fMinRadius = fMinRadius; 
  m_fMaxRadius = fMaxRadius; 
}
void TCamera::SetWindow( int nWidth, int nHeight, float fArcballRadius )
{ 
	m_WorldArcBall.SetWindow( nWidth, nHeight, fArcballRadius ); 
	m_ViewArcBall.SetWindow( nWidth, nHeight, fArcballRadius ); 
}

void TCamera::SetModelCenter( TVector3 vModelCenter ) 
{ 
	m_vModelCenter = vModelCenter;
}
TCamera::TCamera()
{		
	// 추가
	m_vPosDelta				= TVector3( 0.0f, 0.0f, 0.0f );
	 
	m_fDefaultRadius		= 0.0f;
    m_fMinRadius			= 1.0f;
    m_fMaxRadius			= FLT_MAX;
	m_fRadius				= 0.0f;

	m_fCameraYawAngle		= 0.0f;
	m_fCameraPitchAngle		= 0.0f;
	m_fCameraRollAngle		= 0.0f;

	m_fSpeed				= 0.0f;

	// 추가
	m_nMouseWheelDelta		= 0;

	m_vModelCenter			= TVector3(0,0,0);

	D3DXMatrixIdentity( &m_matWorld );
    D3DXMatrixIdentity( &m_mModelRot );
    D3DXMatrixIdentity( &m_mModelLastRot );    
    D3DXMatrixIdentity( &m_mCameraRotLast );    

	m_nRotateModelButtonMask  = MOUSE_LEFT_BUTTON;
    m_nZoomButtonMask         = MOUSE_WHEEL;
    m_nRotateCameraButtonMask = MOUSE_RIGHT_BUTTON;


	D3DXMatrixIdentity( &m_matWorld );
	SetProjMatrix( XM_PI / 4.0f, 1.0f, 0.1f, 1000.0f );
	SetViewMatrix( TVector3( 0.0f, 0.0f, 10.0f ), TVector3( 0.0f, 0.0f, 0.0f ),TVector3( 0.0f, 1.0f, 0.0f ));
}

TCamera::~TCamera()
{	
}


TMatrix TGameCamera::Update(TVector4 vPos)
{
	m_vTargetPos.x = vPos.x;
	m_vTargetPos.y = vPos.y;
	m_vTargetPos.z = vPos.z;

	m_fOffsetPos +=vPos.w * g_fSecPerFrame *m_fSpeed;
	m_vCameraPos = m_vTargetPos + TVector3(0, 0, -1.0f) * m_fOffsetPos;
	return m_matView;
}
bool    TGameCamera::Frame()
{
	D3DXMatrixLookAtLH(
		&m_matView,
		&m_vCameraPos,
		&m_vTargetPos,
		&m_vUpVector);
	
	m_vLookVector.x = m_matView._13;
	m_vLookVector.y = m_matView._23;
	m_vLookVector.z = m_matView._33;
	m_vRightVector.x = m_matView._11;
	m_vRightVector.y = m_matView._21;
	m_vRightVector.z = m_matView._31;
	m_vUpVector.x = m_matView._12;
	m_vUpVector.y = m_matView._22;
	m_vUpVector.z = m_matView._32;

	D3DXVec3Normalize(&m_vLookVector, &m_vLookVector);
	//UpdateVector();
	return true;
};
TGameCamera::TGameCamera()
{
	m_fSpeed = 10.0f;
	m_fOffsetPos = 30.0f;
}
TGameCamera::~TGameCamera()
{
}