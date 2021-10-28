#pragma once
#include "TCore.h"
#include "TFbxObj.h"
class Sample :   public TCore
{
	TVector3	m_vMoePos;
	std::vector<TFbxObj*> m_pObjectList;
	TFbxObj		m_FbxObjA;
	TFbxObj		m_FbxObjB;
	ID3D11DepthStencilView* m_pDepthStencilView;
	ID3D11DepthStencilState* m_pDsvState;
	TMatrix		m_matShadow;
	ID3D11PixelShader* m_pPSShadow = nullptr;
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

