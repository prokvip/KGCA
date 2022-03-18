#pragma once
#include "TDxObject.h"
class TObject3D : public TDxObject
{
public:
	TVector3			m_vPos;
	TVector3			m_vDirection;
	TVector4			m_vColor;
	TMatrix				m_matWorld;
public:
	float  m_fAlpha = 0.0f;
	bool   m_bFadeIn = false;
	bool   m_bFadeOut = false;
	virtual void    FadeIn();
	virtual void    FadeOut();
public:
	virtual void		AddPosition(TVector3 vPos);
	virtual void		SetPosition(TVector3 vPos);	
public:
	virtual bool		SetVertexData() override;
	virtual bool		SetIndexData() override;
	virtual bool		Frame() override;
	bool	Load(ID3D11Device* pd3dDevice, std::wstring filename) {
		return true;
	};
	virtual void  UpdateData() {}
	virtual void		SetMatrix(TMatrix* matWorld,
							TMatrix* matView, TMatrix* matProj);
public:
	TObject3D();
	virtual ~TObject3D();
};

