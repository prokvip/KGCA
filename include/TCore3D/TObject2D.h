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
	// ȭ����ǥ ��ġ�� �������� NDC ��ȯ
	virtual void	Convert(TVector2 center, float fWidth, float fHeight,
		std::vector<SimpleVertex>& retList);
	// ȭ����ǥ�踦 NDC ��ȯ
	virtual void	Convert(std::vector<SimpleVertex>& list,
		std::vector<SimpleVertex>& retList);
public:
	virtual bool		SetVertexData() override;
	virtual bool		SetIndexData() override;
public:
	TObject2D();
	virtual ~TObject2D();
};

