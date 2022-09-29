#pragma once
#include "TBaseObject.h"
#include "TCollision.h"

class TObject2D : public TBaseObject
{
public:
	W_STR			m_szName;
	UINT			m_IIndex;
	W_STR			m_szTexturePath;
	W_STR			m_szMaskTexturePath;
	W_STR			m_szShaderPath;
public:
	TTexture*	m_pMaskTex = nullptr;
	POINT	    m_ptImageSize;
	TRect		m_rtInit;
	TRect		m_rtUV;
	TVector2D	m_vPos;
	TVector2D	m_vDir;
	float		m_fSpeed = 100.0f;
	float       m_fAngleDegree = 0.0f;
	TVector2D	m_vNDCPos;
	TVector2D	m_vDrawSize;
	TVector2D	m_vBeforePos;
	TVector2D	m_vOffsetPos;
public:
	TVector2D	m_vCameraPos;
	TVector2D	m_vViewSize;
	virtual void  SetCameraPos(TVector2D vCamera) { m_vCameraPos = vCamera; }
	virtual void  SetCameraSize(TVector2D vSize) { m_vViewSize = vSize; }
	virtual void  ScreenToNDC();
	virtual void  ScreenToCamera(TVector2D vCameraPos,
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
	virtual void		Rotation();
};

