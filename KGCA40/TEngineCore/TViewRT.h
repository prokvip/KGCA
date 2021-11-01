#pragma once
#include "TViewDS.h"
class TViewRT
{
	TViewDS    m_dxDs;
	// Store the old render targets
	D3D11_VIEWPORT			m_vpOld[D3D11_VIEWPORT_AND_SCISSORRECT_MAX_INDEX];
	UINT					m_nViewPorts;
	ID3D11RenderTargetView* m_pOldRTV;
	ID3D11DepthStencilView* m_pOldDSV;
public:
	ID3D11Texture2D* m_pDSTexture;
	ID3D11ShaderResourceView* m_pTextureSRV;
	ID3D11RenderTargetView* m_pRenderTargetView;// 메인 랜더타켓 뷰
public:
	bool	Create(UINT Width, UINT Height);
	bool    Begin(ID3D11DeviceContext* pContext);
	bool    End(ID3D11DeviceContext* pContext);
	bool	Release();
	void	Save(std::wstring saveFileName);
	ID3D11Texture2D*	CreateTexture(UINT Width, UINT Height);
	HRESULT				SetRenderTargetView(ID3D11Texture2D* pTexture);
	HRESULT CreateRenderTargetView(UINT Width, UINT Height);
};

