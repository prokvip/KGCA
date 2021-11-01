#pragma once
#include "TViewDS.h"
class TViewRT
{
public:
	TViewDS    m_dxDs;
	// Store the old render targets
	D3D11_VIEWPORT			m_vpOld[D3D11_VIEWPORT_AND_SCISSORRECT_MAX_INDEX];
	UINT					m_nViewPorts;
	ID3D11RenderTargetView* m_pOldRTV;
	ID3D11DepthStencilView* m_pOldDSV;
	D3D11_VIEWPORT			m_ViewPort;
	TMatrix					m_matProj;
	HRESULT		SetViewPort(UINT Width, UINT Height);
	TMatrix  	CreateProjMatrix(
		float fNear, float fFar, float fFov, float fAspect);
public:
	ID3D11Texture2D* m_pTexture;
	ID3D11ShaderResourceView* m_pTextureSRV;
	ID3D11RenderTargetView* m_pRenderTargetView;// 메인 랜더타켓 뷰
public:
	bool	Create(UINT Width, UINT Height);
	bool    Begin(ID3D11DeviceContext* pContext);
	bool    End(ID3D11DeviceContext* pContext);
	bool	Release();
	void	Save(ID3D11DeviceContext* pContext, std::wstring saveFileName);
	ID3D11Texture2D*	CreateTexture(UINT Width, UINT Height);
	HRESULT				SetRenderTargetView(ID3D11Texture2D* pTexture);
	HRESULT CreateRenderTargetView(UINT Width, UINT Height);
};

