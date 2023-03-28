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
	TVector2	m_vPos;
	TVector2	m_vDir;
	float		m_fSpeed = 100.0f;
	float       m_fAngleDegree = 0.0f;
	TVector2	m_vNDCPos;
	TVector2	m_vDrawSize;
	TVector2	m_vBeforePos;
	TVector2	m_vOffsetPos;
public:
	TVector2	m_vCameraPos;
	TVector2	m_vViewSize;
	virtual void  SetCameraPos(TVector2 vCamera) { m_vCameraPos = vCamera; }
	virtual void  SetCameraSize(TVector2 vSize) { m_vViewSize = vSize; }
	virtual void  ScreenToNDC();
	virtual void  ScreenToCamera(TVector2 vCameraPos,
		TVector2 vViewPort = { 2000.0f, 2000.0f });
public:
	bool  Frame() override;
	virtual void  SetRect(TRect vPos);
	virtual void  SetPosition(TVector2 vPos);
	virtual void  SetPosition(TVector2 vPos, TVector2 vCamera);
	virtual void  SetDirection(TVector2 vDir) {
		m_vDir = vDir;
	};
	virtual void  UpdateVertexBuffer() override;
	virtual void  SetMask(TTexture* pMaskTex) {
		m_pMaskTex = pMaskTex;
	};
	virtual void		Rotation();
};

