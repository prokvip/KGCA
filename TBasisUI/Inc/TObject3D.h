#pragma once
#include "TDxObject.h"
namespace TBasisFBX
{
	class TBASISFBX_API TObject3D : public TDxObject
	{
	public:
		TBasisFBX::TVector3			m_vPos;
		TBasisFBX::TVector3			m_vDirection;
		TBasisFBX::TVector4			m_vColor;
		TBasisFBX::TMatrix			m_matWorld;
		TBasisFBX::TMatrix			m_matView;
		TBasisFBX::TMatrix			m_matProj;
		TBasisFBX::TVector3			m_vLight; //x	
		TBasisFBX::TVector3			m_vUp; //y 
		TBasisFBX::TVector3			m_vLook; // z	
	public:
		float  m_fSecPerFrame=0.0f;
		float  m_fGameTimer=0.0f;
		float  m_fAlpha = 0.0f;
		bool   m_bFadeIn = false;
		bool   m_bFadeOut = false;
		virtual void    FadeIn();
		virtual void    FadeOut();
	public:
		virtual void		AddPosition(TBasisFBX::TVector3 vPos);
		virtual void		SetPosition(TBasisFBX::TVector3 vPos);
	public:
		virtual bool		SetVertexData() override;
		virtual bool		SetIndexData() override;
		virtual bool		Frame(float fSecperFrame, float fGameTimer) override;
		bool	Load(ID3D11Device* pd3dDevice, std::wstring filename) {
			return true;
		};
		virtual void		UpdateData();
		virtual void		SetMatrix(TBasisFBX::TMatrix* matWorld,
			TBasisFBX::TMatrix* matView, TBasisFBX::TMatrix* matProj);
		virtual void		UpdateCollision();
	public:
		virtual void       GenAABB();
	public:
		TObject3D();
		virtual ~TObject3D();
	};

};