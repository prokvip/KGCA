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
	bool	Init() override;
	bool	Frame() override;
	bool	Render()override;
};
class TButtonObject : public TUIObject
{
public:
	virtual void	HitSelect(TBaseObject* pObj, DWORD dwState) override;
	bool	Init() override;
	bool	Frame() override;
	bool	Render()override;
};
