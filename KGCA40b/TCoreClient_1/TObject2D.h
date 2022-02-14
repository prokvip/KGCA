#pragma once
#include "TDxObject.h"
class TObject2D : public TDxObject
{
public:
	RECT				m_rtSource;
	RECT				m_rtDraw;
	void				SetRectSouce(RECT rt)
	{
		m_rtSource = rt;
	}
	void				SetRectDraw(RECT rt)
	{
		m_rtDraw = rt;
	}
public:
	virtual bool		SetVertexData() override;
	virtual void		SetPosition(TVector2 vPos);
	// ȭ����ǥ ��ġ�� �������� NDC ��ȯ
	virtual void	Convert(
		TVector2 center, float fWidth, float fHeight,
		std::vector<SimpleVertex>& retList);
	// ȭ����ǥ�踦 NDC ��ȯ
	virtual void	Convert(std::vector<SimpleVertex>& list,
		std::vector<SimpleVertex>& retList);
};

