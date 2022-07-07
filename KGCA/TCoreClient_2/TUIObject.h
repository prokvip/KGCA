#pragma once
#include "TObject2D.h"
class TUIObject : public TObject2D
{
public:
	bool	Frame() override;
	bool	Render()override;
};
class TImageObject : public TUIObject
{
public:
	float  m_fAlpha = 0.0f;
	bool   m_bFadeIn = false;
	bool   m_bFadeOut = false;
public:
	bool	Init() override;
	bool	Frame() override;
	bool	Render()override;
	void    FadeIn();
	void    FadeOut();
};
class TButtonObject : public TUIObject
{
public:
	bool	Frame() override;
	bool	Render()override;
};
