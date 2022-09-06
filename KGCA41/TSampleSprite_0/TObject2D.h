#pragma once
#include "TBaseObject.h"
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
private:
	TVector2D	m_vDrawPos;
	TVector2D	m_vDrawSize;
public:
	bool  Frame() override;
	void  SetRect(TRect vPos);
	void  SetPosition(TVector2D vPos);
	void  SetDirection(TVector2D vDir) {
		m_vDir = vDir;
	};
	void  UpdateVertexBuffer();
	void  SetMask(TTexture* pMaskTex) {
		m_pMaskTex = pMaskTex;
	};
};

