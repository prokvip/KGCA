#pragma once
#include "TCore.h"
#include "TFbxObj.h"
class Sample :   public TCore
{
	TViewRT     m_Rt;
	TVector3	m_vMoePos;
	std::vector<TFbxObj*> m_pObjectList;
	TFbxObj		m_FbxObjA;
	TFbxObj		m_FbxObjB;		
	TMatrix		m_matShadow;
	ID3D11PixelShader* m_pPSShadow = nullptr;
public:
	ID3D11DepthStencilState* m_pDsvState;
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

