#pragma once
#include "TBasisStd.h"

class TArcBall
{
public:
    TMatrix		m_mRotation;         // Matrix for arc ball's orientation
    TMatrix		m_mTranslation;      // Matrix for arc ball's position
    TMatrix		m_mTranslationDelta; // Matrix for arc ball's position

    POINT				m_Offset;   // window offset, or upper-left corner of window
    INT					m_nWidth;   // arc ball's window width
    INT					m_nHeight;  // arc ball's window height
    TVector2			m_vCenter;  // center of arc ball 
    FLOAT				m_fRadius;  // arc ball's radius in screen coords
    FLOAT				m_fRadiusTranslation; // arc ball's radius for translating the target

    TQuaternion		m_qDown;             // Quaternion before button down
    TQuaternion		m_qNow;              // Composite quaternion for current drag
    bool				m_bDrag;             // Whether user is dragging arc ball

    POINT				m_ptLastMouse;      // position of last mouse point
    TVector3			m_vDownPt;           // starting point of rotation arc
    TVector3			m_vCurrentPt;        // current point of rotation arc

    TVector3			ScreenToVector( float fScreenPtX, float fScreenPtY );

public:
    // Functions to change behavior
    void				Reset(); 
    void				SetTranslationRadius( FLOAT fRadiusTranslation );
    void				SetWindow( INT nWidth, INT nHeight, FLOAT fRadius = 0.9f );
    void				SetOffset( INT nX, INT nY ); ;

    // Call these from client and use GetRotationMatrix() to read new rotation matrix
    void				OnBegin( int nX, int nY );  // start the rotation (pass current mouse position)
    void				OnMove( int nX, int nY );   // continue the rotation (pass current mouse position)
    void				OnEnd();                    // end the rotation 

    // Or call this to automatically handle left, middle, right buttons
    LRESULT				HandleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

    // Functions to get/set state
    const TMatrix*	GetRotationMatrix()                   { return D3DXMatrixRotationQuaternion(&m_mRotation, &m_qNow); };
    const TMatrix*	GetTranslationMatrix() const          { return &m_mTranslation; }
    const TMatrix*	GetTranslationDeltaMatrix() const     { return &m_mTranslationDelta; }
    bool				IsBeingDragged() const                { return m_bDrag; }
    TQuaternion		GetQuatNow() const                    { return m_qNow; }
    void				SetQuatNow( TQuaternion q )		  { m_qNow = q; }

    static TQuaternion WINAPI QuatFromBallPoints( const TVector3 &vFrom, const TVector3 &vTo );
public:
    TArcBall();

};
