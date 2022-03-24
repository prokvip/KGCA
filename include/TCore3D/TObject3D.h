#pragma once
#include "TDxObject.h"
class TObject3D : public TDxObject
{
public:
	TMath::TVector3			m_vPos;
	TMath::TVector3			m_vDirection;
	TMath::TVector4			m_vColor;
	TMath::TMatrix				m_matWorld;
	TMath::TVector3			m_vLight; //x	
	TMath::TVector3			m_vUp; //y 
	TMath::TVector3			m_vLook; // z	
public:
	float  m_fAlpha = 0.0f;
	bool   m_bFadeIn = false;
	bool   m_bFadeOut = false;
	virtual void    FadeIn();
	virtual void    FadeOut();
public:
	virtual void		AddPosition(TMath::TVector3 vPos);
	virtual void		SetPosition(TMath::TVector3 vPos);	
public:
	virtual bool		SetVertexData() override;
	virtual bool		SetIndexData() override;
	virtual bool		Frame() override;
	bool	Load(ID3D11Device* pd3dDevice, std::wstring filename) {
		return true;
	};
	virtual void  UpdateData() {}
	virtual void		SetMatrix(TMath::TMatrix* matWorld,
							TMath::TMatrix* matView, TMath::TMatrix* matProj);
public:
	TObject3D();
	virtual ~TObject3D();
};

