#include "TArcBall.h"

//--------------------------------------------------------------------------------------
TArcBall::TArcBall()
{
    Reset();
    m_vDownPt = TVector3(0,0,0);
    m_vCurrentPt = TVector3(0,0,0);
    m_Offset.x = m_Offset.y = 0;

	// �ӽ÷� Ŭ���̾�Ʈ ��ǥ�������� ȭ���� ���̿ͳ��̸� �����Ѵ�.
	// �׷��� ������ ����ɶ� ����̽��� ���̿� ���̷� �����Ǿ� ����ȴ�. 
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
// ���� ������ ���������� �������� ���� �ݱ��� �������� ȭ���� ���̿� ���̷� ��ġ���� 
// ȭ���� ���̿� ���̿� ���� �� ���а��� ���� ���� �������Ŀ� ���� ���а��� ���Ѵ�.
TVector3 TArcBall::ScreenToVector( float fScreenPtX, float fScreenPtY )
{
    // Scale to screen X*X + Y*Y + Z*Z = R * R(���������)
	// �� ������ : ȭ����� = �� ���а� : ȭ�鼺�а�
	// �� ���а� = ( �� ������ * ȭ�鼺�а� ) / ȭ�����
    FLOAT x   = -(fScreenPtX - m_Offset.x - m_nWidth/2)  / (m_fRadius*m_nWidth/2);
    FLOAT y   =  (fScreenPtY - m_Offset.y - m_nHeight/2) / (m_fRadius*m_nHeight/2);

    FLOAT z   = 0.0f;
	// �� z���а� = �� x���а� ���� * �� y���а� ����
    FLOAT mag = x*x + y*y;

	// ȭ����̸� �� ������ 1�� �ξ��� ������ 1���� ũ�� ȭ����̱� ������ ����� ������ ��ü�Ѵ�.
	// X*X + Y*Y + Z*Z = 1(���������)	
    if( mag > 1.0f )
    {
        FLOAT scale = 1.0f/sqrtf(mag);
        x *= scale;
        y *= scale;
    }
    else
	{	// Z���а� ���� = 1 - ( X���а�����+ Y���а����� ) 
        z = sqrtf( 1.0f - mag );
	}

    // Return vector
    return TVector3( x, y, z );
}




//--------------------------------------------------------------------------------------
TQuaternion TArcBall::QuatFromBallPoints(const TVector3 &vFrom, const TVector3 &vTo)
{
	// ������ ��� ������� ����� (ȸ���� : ȸ����/2 )�̱� ������
	// �Ʒ��� ���� ������� ���� �ι��� ������ ȸ���ȴ�. 
	// �̴� ȭ���� ���� �� ���� �Ѱ踦 1 ~ -1�� �ξ��� ������ �ڻ��� 1( 90��)���� 2���� 180�� ȸ���ȴ�.
    TVector3 vPart;
    float fDot = D3DXVec3Dot(&vFrom, &vTo); // ȸ����
    D3DXVec3Cross(&vPart, &vFrom, &vTo);    // ȸ����

    return TQuaternion(vPart.x, vPart.y, vPart.z, fDot);
}




//--------------------------------------------------------------------------------------
// ���콺�� ���� ��ư�� Ŭ���Ǿ����� ȣ���
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
// ���콺 ���� ��ư�� Ŭ���� ���¿��� ���ȣ���.
void TArcBall::OnMove( int nX, int nY )
{
    if (m_bDrag) 
    { 
        m_vCurrentPt = ScreenToVector( (float)nX, (float)nY );
		// ���� ȸ���� ������� ���� ������ ������� ���Ͽ� ������Ų��.
        m_qNow = m_qDown * QuatFromBallPoints( m_vDownPt, m_vCurrentPt );
    }
	m_ptLastMouse.x = nX;
    m_ptLastMouse.y = nY;
}




//--------------------------------------------------------------------------------------
// ���콺 ���ʹ�ư�� Ŭ���� ����Ǹ� ȣ���.
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
