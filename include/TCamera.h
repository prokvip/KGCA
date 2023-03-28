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
	// ��� ����
	//--------------------------------------------------------------------------------------
	TVector3			m_vModelCenter;
    TMatrix			m_mModelLastRot;        // Last arcball rotation matrix for model 
    TMatrix			m_mModelRot;            // Rotation matrix of model
  	TMatrix			m_mCameraRotLast;

	// �̵� �ӵ�
	float				m_fSpeed;
public:
	//--------------------------------------------------------------------------------------
	// �� ȸ���� ���� ���ʹϾ�
	//--------------------------------------------------------------------------------------
	TQuaternion		m_qRotation;
	//--------------------------------------------------------------------------------------
	// �� �࿡ ���� ������ w=������( �������κ����� �Ÿ� )
	//--------------------------------------------------------------------------------------
	TVector3			m_vPosDelta;
           
	//--------------------------------------------------------------------------------------
	// �� ��� �� ���� ���
	//--------------------------------------------------------------------------------------
	TMatrix			m_matWorld;
	TMatrix			m_matView;	
	TMatrix			m_matProj;
	//--------------------------------------------------------------------------------------
	// ī�޶� ���⺤�� �� ��ġ / Ÿ�� ����	
	//--------------------------------------------------------------------------------------
	TVector3			m_vRightVector;
	TVector3			m_vLookVector;
	TVector3			m_vUpVector; 
	TVector3			m_vCameraPos;
	TVector3			m_vTargetPos;	
	TVector3			m_vDefaultEye;          // Default camera eye position
	TVector3			m_vDefaultLookAt;       // Default LookAt position
	//--------------------------------------------------------------------------------------
	// ���� ����� �Ӽ�����
	//--------------------------------------------------------------------------------------	
	float				m_fFOV;                 // Field of view
    float				m_fAspect;              // Aspect ratio
    float				m_fNearPlane;           // Near plane
    float				m_fFarPlane;            // Far plane
	// �߰� : ���Ϸ� �� ���� 	
	float				m_fCameraYawAngle;
	float				m_fCameraPitchAngle;
	float				m_fCameraRollAngle;
    // �߰� : ������( �ʱ� �������κ����� �Ÿ�) �� ���� ���� 	
	float				m_fRadius;   
    float				m_fDefaultRadius;       // Distance from the camera to model 
    float				m_fMinRadius;           // Min radius
    float				m_fMaxRadius;           // Max radius
	//--------------------------------------------------------------------------------------
	// ���콺 �� ��ư�� ��뿩�θ� �Ǵ��ϴ� ����ũ ���� ���콺 �� ������
	//--------------------------------------------------------------------------------------	
	int					m_nMouseWheelDelta;     // Amount of middle wheel scroll (+/-) 
	int					m_nRotateModelButtonMask;
	int					m_nZoomButtonMask;
	int					m_nRotateCameraButtonMask;
public :
	//--------------------------------------------------------------------------------------
	// Ÿ�� ��ġ�� ���� �Ÿ�(������)�� �����Ѵ�.
	//--------------------------------------------------------------------------------------
	void						SetRadius( float fDefaultRadius=5.0f, float fMinRadius=1.0f, float fMaxRadius=FLT_MAX  );
public:			
	//--------------------------------------------------------------------------------------
	// �߰� : �� ���⺤�� ���� 
	//--------------------------------------------------------------------------------------
	virtual TMatrix			UpdateVector();
	//--------------------------------------------------------------------------------------
	// ������Ʈ�� ȭ�鿡 ���߾� �� ��� ���� 
	//--------------------------------------------------------------------------------------
	virtual TMatrix			SetObjectView(	TVector3 vMax, 
												TVector3 vMin );	
	//--------------------------------------------------------------------------------------
	// �� ��� ���  
	//--------------------------------------------------------------------------------------
	virtual TMatrix			SetViewMatrix(	TVector3 vPos, 
												TVector3 vTarget= TVector3( 0.0f, 0.0f, 0.0f ), 
												TVector3 vUp = TVector3( 0.0f, 1.0f, 0.0f ) );
	//--------------------------------------------------------------------------------------
	// ���� ��� ��� 
	//--------------------------------------------------------------------------------------
	virtual TMatrix			SetProjMatrix(	FLOAT fFOV, 
												FLOAT fAspect, 
												FLOAT fNearPlane, 
												FLOAT fFarPlane );
	//--------------------------------------------------------------------------------------
	// ī�޶� ��ġ ���� 
	//--------------------------------------------------------------------------------------
	virtual void				MoveLook( float fValue );
	virtual void				MoveSide( float fValue );	
	virtual void				MoveUp( float fValue );	
	//--------------------------------------------------------------------------------------
	// ���Ϸ� ���� ���� �� ��� ��� 
	//--------------------------------------------------------------------------------------
	virtual TMatrix			Update( TVector4 vDirValue );
	virtual bool				Frame();

	// ȸ�� �� �̵��� ������ ��ư ���� 
	virtual void				SetButtonMasks( int nRotateModelButtonMask, int nZoomButtonMask, int nRotateCameraButtonMask );
	//--------------------------------------------------------------------------------------
	// ������ ����� ������ �ݿ��Ѵ�. 
	//--------------------------------------------------------------------------------------
	virtual void				SetWindow( int nWidth, int nHeight, float fArcballRadius=0.9f );		
	//--------------------------------------------------------------------------------------
	// ������ �޼����� ����Ͽ� ī�޶� ���� 
	//--------------------------------------------------------------------------------------
	virtual int					WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	//--------------------------------------------------------------------------------------
	// �� ȸ�� �߽���ġ�� �����Ѵ�.
	//--------------------------------------------------------------------------------------
    void						SetModelCenter( TVector3 vModelCenter );

	void						SetTargetPos( TVector3 vPos ) { m_vTargetPos = vPos; };

public:
	//--------------------------------------------------------------------------------------
	// ����Լ�
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