#pragma once
#include "TObject2D.h"
#include "TSprite.h"
class TEffectObj : public TObject2D
{
public:
	TSprite* m_pSprite;
public:
	virtual bool	Init() override;
	virtual bool	Frame()override;
	virtual bool	Render()override;
	virtual bool	Release()override;
};

