#pragma once
#include "TBaseObject.h"
#include "TCollision.h"
class TObject2D : public TBaseObject
{
public:
	TTexture* m_pMaskTex = nullptr;
	POINT	    m_ptImageSize;
	TRect		m_rtInit;
	TRect		m_rtUV;
	TVector2D	m_vPos;
	TVector2D	m_vDir;
	float		m_fSpeed = 100.0f;
	TVector2D	m_vDrawPos;
	TVector2D	m_vDrawSize;
public:
	TVector2D	m_vCameraPos;
	TVector2D	m_vViewSize;
	void  SetCameraPos(TVector2D vCamera) { m_vCameraPos = vCamera; }
	void  SetCameraSize(TVector2D vSize) { m_vViewSize = vSize; }
	void  ScreenToNDC();
	void  ScreenToCamera(TVector2D vCameraPos,
		TVector2D vViewPort = { 2000.0f, 2000.0f });
public:
	bool  Frame() override;
	virtual void  SetRect(TRect vPos);
	virtual void  SetPosition(TVector2D vPos);
	virtual void  SetPosition(TVector2D vPos, TVector2D vCamera);
	virtual void  SetDirection(TVector2D vDir) {
		m_vDir = vDir;
	};
	virtual void  UpdateVertexBuffer() override;
	virtual void  SetMask(TTexture* pMaskTex) {
		m_pMaskTex = pMaskTex;
	};
	
};

