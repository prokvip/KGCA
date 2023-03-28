#include "TArcBall.h"

//--------------------------------------------------------------------------------------
TArcBall::TArcBall()
{
    Reset();
    m_vDownPt = TVector3(0,0,0);
    m_vCurrentPt = TVector3(0,0,0);
    m_Offset.x = m_Offset.y = 0;

	// 임시로 클라이언트 좌표기준으로 화면의 넓이와높이를 세팅한다.
	// 그러나 실제로 적용될때 디바이스의 넓이와 높이로 수정되어 적용된다. 
    RECT rc;
    GetClientRect( GetForegroundWindow(), &rc );
    SetWindow( rc.right, rc.bottom );
}





//--------------------------------------------------------------------------------------
void TArcBall::Reset()
{
    D3DXQuaternionIdentity( &m_qDown );
    D3DXQuaternionIdentity( &m_qNow );
    D3DXMatrixIdentity( &m_mRotation );
    D3DXMatrixIdentity( &m_mTranslation );
    D3DXMatrixIdentity( &m_mTranslationDelta );
    m_bDrag = FALSE;
    m_fRadiusTranslation = 1.0f;
    m_fRadius = 1.0f;
}

void TArcBall::SetTranslationRadius( FLOAT fRadiusTranslation ) 
{ 
	m_fRadiusTranslation = fRadiusTranslation; 
}
void TArcBall::SetWindow( INT nWidth, INT nHeight, FLOAT fRadius )
{ 
	m_nWidth = nWidth; 
	m_nHeight = nHeight; 
	m_fRadius = fRadius; 
	m_vCenter = TVector2(m_nWidth/2.0f,m_nHeight/2.0f); 
}
void TArcBall::SetOffset( INT nX, INT nY ) 
{ 
	m_Offset.x = nX; 
	m_Offset.y = nY; 
}
//--------------------------------------------------------------------------------------
// 모델의 중점을 구방정식의 중점으로 보고 반구를 기준으로 화면의 넓이와 높이로 일치시켜 
// 화면의 넓이와 높이에 대한 구 성분값에 따라 최종 구방정식에 따라서 성분값을 구한다.
TVector3 TArcBall::ScreenToVector( float fScreenPtX, float fScreenPtY )
{
    // Scale to screen X*X + Y*Y + Z*Z = R * R(구면방정식)
	// 구 반지름 : 화면넓이 = 구 성분값 : 화면성분값
	// 구 성분값 = ( 구 반지름 * 화면성분값 ) / 화면넓이
    FLOAT x   = -(fScreenPtX - m_Offset.x - m_nWidth/2)  / (m_fRadius*m_nWidth/2);
    FLOAT y   =  (fScreenPtY - m_Offset.y - m_nHeight/2) / (m_fRadius*m_nHeight/2);

    FLOAT z   = 0.0f;
	// 구 z성분값 = 구 x성분값 제곱 * 구 y성분값 제곱
    FLOAT mag = x*x + y*y;

	// 화면넓이를 구 반지름 1로 두었기 때문에 1보다 크면 화면밖이기 때문에 가까운 값으로 대체한다.
	// X*X + Y*Y + Z*Z = 1(구면방정식)	
    if( mag > 1.0f )
    {
        FLOAT scale = 1.0f/sqrtf(mag);
        x *= scale;
        y *= scale;
    }
    else
	{	// Z성분값 제곱 = 1 - ( X성분값제곱+ Y성분값제곱 ) 
        z = sqrtf( 1.0f - mag );
	}

    // Return vector
    return TVector3( x, y, z );
}




//--------------------------------------------------------------------------------------
TQuaternion TArcBall::QuatFromBallPoints(const TVector3 &vFrom, const TVector3 &vTo)
{
	// 임의의 축과 사원수의 관계는 (회전각 : 회전각/2 )이기 때문에
	// 아래와 같이 사원수를 쓰면 두배의 각으로 회전된다. 
	// 이는 화면의 가로 및 세로 한계를 1 ~ -1로 두었기 때문에 코사인 1( 90도)에서 2배인 180로 회전된다.
    TVector3 vPart;
    float fDot = D3DXVec3Dot(&vFrom, &vTo); // 회전각
    D3DXVec3Cross(&vPart, &vFrom, &vTo);    // 회전축

    return TQuaternion(vPart.x, vPart.y, vPart.z, fDot);
}




//--------------------------------------------------------------------------------------
// 마우스의 왼쪽 버튼이 클릭되었을때 호출됨
void TArcBall::OnBegin( int nX, int nY )
{
    // Only enter the drag state if the click falls
    // inside the click rectangle.
    if( nX >= m_Offset.x &&
        nX < m_Offset.x + m_nWidth &&
        nY >= m_Offset.y &&
        nY < m_Offset.y + m_nHeight )
    {
        m_bDrag = true;
        m_qDown = m_qNow;
        m_vDownPt = ScreenToVector( (float)nX, (float)nY );
    }
}




//--------------------------------------------------------------------------------------
// 마우스 왼쪽 버튼이 클릭된 상태에서 계속호출됨.
void TArcBall::OnMove( int nX, int nY )
{
    if (m_bDrag) 
    { 
        m_vCurrentPt = ScreenToVector( (float)nX, (float)nY );
		// 현재 회전된 사원수와 현재 설정된 사원수를 곱하여 누적시킨다.
        m_qNow = m_qDown * QuatFromBallPoints( m_vDownPt, m_vCurrentPt );
    }
	m_ptLastMouse.x = nX;
    m_ptLastMouse.y = nY;
}




//--------------------------------------------------------------------------------------
// 마우스 왼쪽버튼의 클릭이 종료되면 호출됨.
void TArcBall::OnEnd()
{
    m_bDrag = false;
}




//--------------------------------------------------------------------------------------
// Desc:
//--------------------------------------------------------------------------------------
LRESULT TArcBall::HandleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    // Current mouse position
    int iMouseX = (short)LOWORD(lParam);
    int iMouseY = (short)HIWORD(lParam);

    switch( uMsg )
    {
        case WM_LBUTTONDOWN:
        case WM_LBUTTONDBLCLK:
            SetCapture( hWnd );
            OnBegin( iMouseX, iMouseY );
            return TRUE;

        case WM_LBUTTONUP:
            ReleaseCapture();
            OnEnd();
            return TRUE;
        case WM_CAPTURECHANGED:
            if( (HWND)lParam != hWnd )
            {
                ReleaseCapture();
                OnEnd();
            }
            return TRUE;

        case WM_RBUTTONDOWN:
        case WM_RBUTTONDBLCLK:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONDBLCLK:
            SetCapture( hWnd );
            // Store off the position of the cursor when the button is pressed
            m_ptLastMouse.x = iMouseX;
            m_ptLastMouse.y = iMouseY;
            return TRUE;

        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
            ReleaseCapture();
            return TRUE;
        case WM_MOUSEWHEEL:
        {
            short delta = HIWORD(wParam);
            delta = (delta / WHEEL_DELTA); // WHEEL_DELTA : 120
            return TRUE;
        }
        case WM_MOUSEMOVE:
            if( MK_LBUTTON&wParam )
            {
                OnMove( iMouseX, iMouseY );
            }
            else if( (MK_RBUTTON&wParam) || (MK_MBUTTON&wParam) )
            {
                // Normalize based on size of window and bounding sphere radius
                FLOAT fDeltaX = ( m_ptLastMouse.x-iMouseX ) * m_fRadiusTranslation / m_nWidth;
                FLOAT fDeltaY = ( m_ptLastMouse.y-iMouseY ) * m_fRadiusTranslation / m_nHeight;

                if( wParam & MK_RBUTTON )
                {
                    D3DXMatrixTranslation( &m_mTranslationDelta, -2*fDeltaX, 2*fDeltaY, 0.0f );
                    D3DXMatrixMultiply( &m_mTranslation, &m_mTranslation, &m_mTranslationDelta );
                }
                else  // wParam & MK_MBUTTON
                {
                    D3DXMatrixTranslation( &m_mTranslationDelta, 0.0f, 0.0f, 5*fDeltaY );
                    D3DXMatrixMultiply( &m_mTranslation, &m_mTranslation, &m_mTranslationDelta );
                }

                // Store mouse coordinate
                m_ptLastMouse.x = iMouseX;
                m_ptLastMouse.y = iMouseY;
            }
            return TRUE;
    }

    return FALSE;
}
