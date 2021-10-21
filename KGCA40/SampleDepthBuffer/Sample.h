#pragma once
#include "TCore.h"
class Sample :   public TCore
{
	ID3D11DepthStencilView* m_pDepthStencilView;
	ID3D11DepthStencilState* m_pDsvState;
public:
	HRESULT     CreateDepthStencilView();
	HRESULT     CreateDepthStencilState();
public:
	bool		Init()override;
	bool		Frame()override;
	bool		Render()override;
	bool		Release()override;
public:
	Sample();
	virtual ~Sample();
};

