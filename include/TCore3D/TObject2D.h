#pragma once
#include "TDxObject.h"
class TObject2D : public TDxObject
{
public:
	RECT				m_rtSource;
	RECT				m_rtDraw;	
public:	
	virtual void		SetRectSouce(RECT rt);
	virtual void		SetRectDraw(RECT rt);
	virtual void		AddPosition(TVector2 vPos);
	virtual void		SetPosition(TVector2 vPos);		
	virtual void		UpdateRectDraw(RECT rt);
	// 화면좌표 위치를 중점으로 NDC 변환
	virtual void	Convert(TVector2 center, float fWidth, float fHeight,
		std::vector<SimpleVertex>& retList);
	// 화면좌표계를 NDC 변환
	virtual void	Convert(std::vector<SimpleVertex>& list,
		std::vector<SimpleVertex>& retList);
public:
	virtual bool		SetVertexData() override;
	virtual bool		SetIndexData() override;
public:
	TObject2D();
	virtual ~TObject2D();
};

