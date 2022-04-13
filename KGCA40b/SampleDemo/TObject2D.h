#pragma once
#include "TDxObject.h"
class TObject2D : public TDxObject
{
public:
	std::vector<SimpleVertex> m_VertexList2D;
	RECT				m_rtSource;
	RECT				m_rtDraw;	
	T::TVector4			m_vColor;
	T::TVector2	m_vPos;
	T::TVector2	m_vDirection;
public:
	float  m_fAlpha = 0.0f;
	bool   m_bFadeIn = false;
	bool   m_bFadeOut = false;
	virtual void    FadeIn();
	virtual void    FadeOut();
public:	
	virtual void		SetRectSouce(RECT rt);
	virtual void		SetRectDraw(RECT rt);
	virtual void		AddPosition(T::TVector2 vPos);
	virtual void		SetPosition(T::TVector2 vPos);		
	virtual void		UpdateRectDraw(RECT rt);
	// 화면좌표 위치를 중점으로 NDC 변환
	virtual void	Convert(T::TVector2 center, float fWidth, float fHeight,
		std::vector<SimpleVertex>& retList);
	// 화면좌표계를 NDC 변환
	virtual void	Convert(std::vector<SimpleVertex>& list,
		std::vector<SimpleVertex>& retList);
	virtual void	ConvertIndex(T::TVector2 center, float fWidth, float fHeight,
		std::vector<SimpleVertex>& retList);
	// 화면좌표계를 NDC 변환
	virtual void	ConvertIndex(std::vector<SimpleVertex>& list,
		std::vector<SimpleVertex>& retList);
public:
	virtual bool		SetVertexData() override;
	virtual bool		SetIndexData() override;
	virtual bool		Frame() override;
	bool	Load(ID3D11Device* pd3dDevice, std::wstring filename) {
		return true;
	};
	virtual void  UpdateData() {}
public:
	TObject2D();
	virtual ~TObject2D();
};

