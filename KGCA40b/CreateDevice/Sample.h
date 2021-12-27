#pragma once
#include "TCore.h"
#include <d3d11.h>
#pragma comment	(lib, "d3d11.lib")
class Sample : public TCore
{
public:
	ID3D11Device*			m_pd3dDevice;	// 디바이스 객체
	ID3D11DeviceContext*	m_pImmediateContext;// 다비이스 컨텍스트 객체
	IDXGISwapChain*			m_pSwapChain;	// 스왑체인 객체
	ID3D11RenderTargetView* m_pRenderTargetView;// 메인 랜더타켓 뷰
	
	D3D_FEATURE_LEVEL       m_FeatureLevel;	// Direct3D 특성레벨 속성값	
	DXGI_SWAP_CHAIN_DESC	m_SwapChainDesc;	// 스왑체인 속성값
	D3D11_VIEWPORT			m_ViewPort;	// 뷰 포트 속성값
public:
	virtual bool	Init()  override;
	virtual bool	Frame()  override;
	virtual bool	Render()  override;
	virtual bool	Release()  override;
public:
	Sample();
};

