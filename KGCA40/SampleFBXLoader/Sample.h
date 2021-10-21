#pragma once
#include "TCore.h"
#include "TFbxObj.h"
class Sample :   public TCore
{
	TFbxObj		m_FbxObjA;
	TFbxObj		m_FbxObjB;
	ID3D11DepthStencilView* m_pDepthStencilView;
	ID3D11DepthStencilState* m_pDsvState;
public:
	HRESULT     CreateDepthStencilView();
	HRESULT     CreateDepthStencilState();
public:
	bool		Init()override;
	bool		Frame()override;
	bool		PreRender();
	bool		Render()override;
	bool		Release()override;
public:
	Sample();
	virtual ~Sample();
};

