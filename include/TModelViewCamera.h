#pragma once
#include "TCamera.h"

class TModelViewCamera : public TCamera
{
public:		
	//--------------------------------------------------------------------------------------
	// 카메라 위치 정보 
	//--------------------------------------------------------------------------------------
	void				MoveLook( float fValue );
	void				MoveSide( float fValue );	
	void				MoveUp( float fValue );	
	//--------------------------------------------------------------------------------------
	// 오일러 각에 따른 뷰 행렬 계산 
	//--------------------------------------------------------------------------------------
	TMatrix			Update( float fElapseTime );
	// 모델의 월드 변환 행렬의 방향벡터
	TVector3			m_vObjectVector[3];
public:

	//--------------------------------------------------------------------------------------
	// 뷰 행렬 계산  
	//--------------------------------------------------------------------------------------
	TMatrix			SetViewMatrix( TVector3 vPos, TVector3 vTarget, TVector3 vUp = TVector3( 0.0f, 1.0f, 0.0f ));	

public:
	TModelViewCamera(void);
	virtual ~TModelViewCamera(void);
};
