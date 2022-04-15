#pragma once
#include "TPlaneObj.h"

#define MAX_MASKCOUNT 25 //625=25*25
struct TKernelMask
{
	float fMask[MAX_MASKCOUNT*MAX_MASKCOUNT];
};
struct VSCB
{
	float		g_fMask;
	float		g_fTexSizeX;
	float		g_fTexSizeY;
	float		a;
};
class TQuadObject : public TPlaneObj
{
public:
	ComPtr<ID3D11Texture2D>             m_pColorTex;
	ComPtr<ID3D11ShaderResourceView>    m_pColorSRV;
	ComPtr<ID3D11RenderTargetView>      m_pColorRTV;
	ComPtr<ID3D11Texture2D>             m_pNormalDepthTex;
	ComPtr<ID3D11ShaderResourceView>    m_pNormalDepthSRV;
	ComPtr<ID3D11RenderTargetView>      m_pNormalDepthRTV;
	ComPtr<ID3D11DepthStencilView>		m_pDepthStencilView;
	ComPtr<ID3D11Buffer>				m_pMaskCB;
	VSCB							    m_MaskValue;

	ComPtr<ID3D11Buffer>                m_pMaskBuffer;
	ComPtr<ID3D11ShaderResourceView>	m_pMaskArray;
	int									m_iMask;
	float*								m_pfKernelMask;
public:
	HRESULT		CreateConstantBuffer(ID3D11Device* pd3dDevice);
	HRESULT		CreateTextures(ID3D11Device* pDevice, UINT iWidth, UINT iHeight);
	bool		SetBuffer(ID3D11Device* pd3dDevice);
	bool		Frame();
	bool		Render();
	void		ComputeKernel(int iMask=3);
	
public:
	TQuadObject(void);
	~TQuadObject(void);
};

