#pragma once
#include "TArcBall.h"
#include "TFrustum.h"

#define MOUSE_LEFT_BUTTON   0x01
#define MOUSE_MIDDLE_BUTTON 0x02
#define MOUSE_RIGHT_BUTTON  0x04
#define MOUSE_WHEEL         0x08

class TCamera : public TFrustum
{
public:	
	TArcBall			m_WorldArcBall;
    TArcBall			m_ViewArcBall;
	//--------------------------------------------------------------------------------------
	// 멤버 변수
	//--------------------------------------------------------------------------------------
	TVector3			m_vModelCenter;
    TMatrix			m_mModelLastRot;        // Last arcball rotation matrix for model 
    TMatrix			m_mModelRot;            // Rotation matrix of model
  	TMatrix			m_mCameraRotLast;

	// 이동 속도
	float				m_fSpeed;
public:
	//--------------------------------------------------------------------------------------
	// 뷰 회전에 대한 쿼터니언
	//--------------------------------------------------------------------------------------
	TQuaternion		m_qRotation;
	//--------------------------------------------------------------------------------------
	// 각 축에 대한 변위값 w=반지름( 원점으로부터의 거리 )
	//--------------------------------------------------------------------------------------
	TVector3			m_vPosDelta;
           
	//--------------------------------------------------------------------------------------
	// 뷰 행렬 및 투영 행렬
	//--------------------------------------------------------------------------------------
	TMatrix			m_matWorld;
	TMatrix			m_matView;	
	TMatrix			m_matProj;
	//--------------------------------------------------------------------------------------
	// 카메라 방향벡터 및 위치 / 타켓 벡터	
	//--------------------------------------------------------------------------------------
	TVector3			m_vRightVector;
	TVector3			m_vLookVector;
	TVector3			m_vUpVector; 
	TVector3			m_vCameraPos;
	TVector3			m_vTargetPos;	
	TVector3			m_vDefaultEye;          // Default camera eye position
	TVector3			m_vDefaultLookAt;       // Default LookAt position
	//--------------------------------------------------------------------------------------
	// 투영 행렬의 속성값수
	//--------------------------------------------------------------------------------------	
	float				m_fFOV;                 // Field of view
    float				m_fAspect;              // Aspect ratio
    float				m_fNearPlane;           // Near plane
    float				m_fFarPlane;            // Far plane
	// 추가 : 오일러 각 성분 	
	float				m_fCameraYawAngle;
	float				m_fCameraPitchAngle;
	float				m_fCameraRollAngle;
    // 추가 : 반지름( 초기 원점으로부터의 거리) 에 대한 변수 	
	float				m_fRadius;   
    float				m_fDefaultRadius;       // Distance from the camera to model 
    float				m_fMinRadius;           // Min radius
    float				m_fMaxRadius;           // Max radius
	//--------------------------------------------------------------------------------------
	// 마우스 및 버튼의 사용여부를 판단하는 마스크 값과 마우스 휠 변위값
	//--------------------------------------------------------------------------------------	
	int					m_nMouseWheelDelta;     // Amount of middle wheel scroll (+/-) 
	int					m_nRotateModelButtonMask;
	int					m_nZoomButtonMask;
	int					m_nRotateCameraButtonMask;
public :
	//--------------------------------------------------------------------------------------
	// 타켓 위치로 부터 거리(반지름)을 지정한다.
	//--------------------------------------------------------------------------------------
	void						SetRadius( float fDefaultRadius=5.0f, float fMinRadius=1.0f, float fMaxRadius=FLT_MAX  );
public:			
	//--------------------------------------------------------------------------------------
	// 추가 : 뷰 방향벡터 저장 
	//--------------------------------------------------------------------------------------
	virtual TMatrix			UpdateVector();
	//--------------------------------------------------------------------------------------
	// 오브젝트를 화면에 맞추어 뷰 행렬 제어 
	//--------------------------------------------------------------------------------------
	virtual TMatrix			SetObjectView(	TVector3 vMax, 
												TVector3 vMin );	
	//--------------------------------------------------------------------------------------
	// 뷰 행렬 계산  
	//--------------------------------------------------------------------------------------
	virtual TMatrix			SetViewMatrix(	TVector3 vPos, 
												TVector3 vTarget= TVector3( 0.0f, 0.0f, 0.0f ), 
												TVector3 vUp = TVector3( 0.0f, 1.0f, 0.0f ) );
	//--------------------------------------------------------------------------------------
	// 투영 행렬 계산 
	//--------------------------------------------------------------------------------------
	virtual TMatrix			SetProjMatrix(	FLOAT fFOV, 
												FLOAT fAspect, 
												FLOAT fNearPlane, 
												FLOAT fFarPlane );
	//--------------------------------------------------------------------------------------
	// 카메라 위치 정보 
	//--------------------------------------------------------------------------------------
	virtual void				MoveLook( float fValue );
	virtual void				MoveSide( float fValue );	
	virtual void				MoveUp( float fValue );	
	//--------------------------------------------------------------------------------------
	// 오일러 각에 따른 뷰 행렬 계산 
	//--------------------------------------------------------------------------------------
	virtual TMatrix			Update( TVector4 vDirValue );
	virtual bool				Frame();

	// 회전 및 이동에 적용할 버튼 세팅 
	virtual void				SetButtonMasks( int nRotateModelButtonMask, int nZoomButtonMask, int nRotateCameraButtonMask );
	//--------------------------------------------------------------------------------------
	// 윈도우 사이즈가 변경을 반영한다. 
	//--------------------------------------------------------------------------------------
	virtual void				SetWindow( int nWidth, int nHeight, float fArcballRadius=0.9f );		
	//--------------------------------------------------------------------------------------
	// 윈도우 메세지를 사용하여 카메라 제어 
	//--------------------------------------------------------------------------------------
	virtual int					WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	//--------------------------------------------------------------------------------------
	// 모델 회전 중심위치를 지정한다.
	//--------------------------------------------------------------------------------------
    void						SetModelCenter( TVector3 vModelCenter );

	void						SetTargetPos( TVector3 vPos ) { m_vTargetPos = vPos; };

public:
	//--------------------------------------------------------------------------------------
	// 멤버함수
	//--------------------------------------------------------------------------------------
	TMatrix*			GetWorldMatrix() 	{ return &m_matWorld; };
	TMatrix*			GetViewMatrix() 	{ return &m_matView; }
	TMatrix*			GetProjMatrix() 	{ return &m_matProj; }
	
	TVector3*		GetEyePt() 			{ return &m_vCameraPos; }
	TVector3*		GetLookAtPt() 		{ return &m_vTargetPos; }
	float				GetNearClip() 		{ return m_fNearPlane; }
	float				GetFarClip() 		{ return m_fFarPlane; }
	
	float				GetYawAngle() 		{ return m_fCameraYawAngle; }
	float				GetPitchAngle() 	{ return m_fCameraPitchAngle; }
	float				GetRollAngle() 		{ return m_fCameraRollAngle; }
	
	TVector3*		GetRightVector() 	{ return &m_vRightVector; }
	TVector3*		GetLookVector() 	{ return &m_vLookVector; }
	TVector3*		GetUpVector() 		{ return &m_vUpVector; }

	float				GetRadius() 		{ return m_fRadius; }
public:
	TCamera(void);
	virtual ~TCamera(void);
};

class TGameCamera : public TCamera
{
public:
	float m_fOffsetPos;
public:
	TMatrix    Update(TVector4 vPos);
	bool    Frame();
	TGameCamera();
	virtual ~TGameCamera();
};