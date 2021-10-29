#pragma once
#include "TViewRT.h"
#include "TViewDS.h"
class TDevice
{
public:
	//--------------------------------------------------------------------------------------
	// Global Variables
	//--------------------------------------------------------------------------------------	
	ID3D11Device* m_pd3dDevice;		// 디바이스 객체
	IDXGISwapChain* m_pSwapChain;		// 스왑체인 객체
	
	D3D11_VIEWPORT			m_ViewPort;			// 뷰포트 
	D3D_DRIVER_TYPE         m_driverType;		// 디바이스 타입( 디폴트:하드웨어 가속 )
	IDXGIFactory*			m_pGIFactory;		// DXGI 객체
	ID3D11DeviceContext*	m_pImmediateContext;// 디바이스 컨텍스트
	D3D_DRIVER_TYPE			m_DriverType;		// 디바이스 타입
	D3D_FEATURE_LEVEL       m_FeatureLevel;		// DirectX의 기능수준
public:
	TViewRT				m_DefaultRT;
	TViewDS				m_DefaultDS;
public:
	bool	SetDevice();
	//--------------------------------------------------------------------------------------
	// 디바이스 및 스왑체인 생성 : 
	// 1, CreateDevice()
	// 2, CreateGIFactory()
	// 3, CreateSwapChain()
	// 4, SetRenderTargetView()
	// 5, SetDepthStencilView
	// 6, SetViewPort()
	//--------------------------------------------------------------------------------------
	HRESULT		CreateDevice();
	HRESULT		CreateGIFactory();
	HRESULT		CreateSwapChain(HWND hWnd, UINT iWidth, UINT iHeigh);
	//--------------------------------------------------------------------------------------
	//
	//--------------------------------------------------------------------------------------
	HRESULT		SetRenderTargetView();
	HRESULT		SetDepthStencilView();
	HRESULT		SetViewPort();
	bool		CleanupDevice();

};

