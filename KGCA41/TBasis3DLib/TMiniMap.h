#pragma once
#include "TDxHelper.h"
#include "TCamera.h"
using namespace DX;

class TMiniMap : public TCamera
{
public:
	shared_ptr<TShape>		m_pPlaneShape;
	shared_ptr<TDxRT>		m_pMiniMapRT;
	D3D11_VIEWPORT			m_vp;
public:
	void		Set(ID3D11Device* pDevice, FLOAT TopLeftX, FLOAT TopLeftY, FLOAT Width, FLOAT Height, FLOAT MinDepth = 0, FLOAT MaxDepth = 1);
	bool		Create(ID3D11Device* pd3dDevice, const TCHAR* pLoadShaderFile = 0,	const TCHAR* pLoadTexture = 0);
	bool		Frame();
	bool		Render(ID3D11DeviceContext*    pContext);
	bool		Release();
	bool		BeginRender(ID3D11DeviceContext*    pContext,TVector4 vColor = TVector4(1,1,1,1));
	bool		EndRender(ID3D11DeviceContext*    pContext);
	HRESULT		CreateResource(ID3D11Device* pd3dDevice, ID3D11DeviceContext*    pContext, UINT iWidth, UINT iHeight);
	HRESULT		DeleteResource();
public:
	TMiniMap(void);
	virtual ~TMiniMap(void);
};
