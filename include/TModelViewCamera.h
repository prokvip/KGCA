#pragma once
#include "TCamera.h"

class TModelViewCamera : public TCamera
{
public:		
	//--------------------------------------------------------------------------------------
	// ī�޶� ��ġ ���� 
	//--------------------------------------------------------------------------------------
	void				MoveLook( float fValue );
	void				MoveSide( float fValue );	
	void				MoveUp( float fValue );	
	//--------------------------------------------------------------------------------------
	// ���Ϸ� ���� ���� �� ��� ��� 
	//--------------------------------------------------------------------------------------
	TMatrix			Update( float fElapseTime );
	// ���� ���� ��ȯ ����� ���⺤��
	TVector3			m_vObjectVector[3];
public:

	//--------------------------------------------------------------------------------------
	// �� ��� ���  
	//--------------------------------------------------------------------------------------
	TMatrix			SetViewMatrix( TVector3 vPos, TVector3 vTarget, TVector3 vUp = TVector3( 0.0f, 1.0f, 0.0f ));	

public:
	TModelViewCamera(void);
	virtual ~TModelViewCamera(void);
};
