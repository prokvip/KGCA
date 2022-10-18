#pragma once
#include "TWindow.h"
class TDevice : public TWindow
{
public:
	ComPtr<IDXGIFactory>			m_pGIFactory = nullptr;
	ComPtr<ID3D11Device>			m_pd3dDevice = nullptr;// 디바이스 객체
	ComPtr<ID3D11DeviceContext>		m_pImmediateContext = nullptr;	

	ComPtr<IDXGISwapChain>			m_pSwapChain = nullptr;
	ComPtr<ID3D11RenderTargetView>	m_pRTV = nullptr;
	ComPtr<ID3D11DepthStencilView>	m_pDepthStencilView = nullptr;
	D3D11_VIEWPORT					m_vp;
public:
	// 1)디바이스 생성
	HRESULT CreateDevice();
	// 2)팩토리 생성
	HRESULT CreateDXGIDevice();
	// 3)스왑체인 생성
	HRESULT CreateSwapChain();
	// 4)랜더타켓뷰 생성
	HRESULT CreateRenderTargetView();
	// 5)뷰포트 설정
	HRESULT CreateDepthStencilView();
	// 6)뷰포트 설정
	void  CreateViewport();
	virtual HRESULT		ResizeDevice(UINT width, UINT height);
public:
	virtual bool		Init(); // 초기화
	virtual bool		Frame();// 실시간 계산
	virtual bool		Render();// 실시간 랜더링
	virtual bool		Release();// 소멸 및 삭제
	virtual HRESULT CreateDXResource();
	virtual HRESULT DeleteDXResource();
};

