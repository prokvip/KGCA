#pragma once
#include "TBaseObject.h"
class TRenderTarget
{
public:
	ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;
	ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;
	ComPtr<ID3D11ShaderResourceView> m_pSRV;
	ComPtr<ID3D11ShaderResourceView> m_pDsvSRV;// ±Ì¿Ãπˆ∆€ ∏Æº“Ω∫∫‰
	ComPtr<ID3D11Texture2D> m_pTexture;
	D3D11_DEPTH_STENCIL_VIEW_DESC			m_DepthStencilDesc;
	D3D11_TEXTURE2D_DESC					m_TexDesc;
	D3D11_VIEWPORT m_Viewport;

	ID3D11RenderTargetView* m_pOldRTV;
	ID3D11DepthStencilView* m_pOldDSV;
	D3D11_VIEWPORT			m_vpOld[D3D11_VIEWPORT_AND_SCISSORRECT_MAX_INDEX];
public:
	bool Create(ID3D11Device* pd3dDevice,
				ID3D11DeviceContext* pImmediateContext,
				FLOAT fWidth = 1024.0f, FLOAT fHeight=1024.0f);
	bool Begin(ID3D11DeviceContext* pContext);
	void End(ID3D11DeviceContext* pContext);
	bool Release();
};

