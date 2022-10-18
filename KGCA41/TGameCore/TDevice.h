#pragma once
#include "TWindow.h"
class TDevice : public TWindow
{
public:
	ComPtr<IDXGIFactory>			m_pGIFactory = nullptr;
	ComPtr<ID3D11Device>			m_pd3dDevice = nullptr;// ����̽� ��ü
	ComPtr<ID3D11DeviceContext>		m_pImmediateContext = nullptr;	

	ComPtr<IDXGISwapChain>			m_pSwapChain = nullptr;
	ComPtr<ID3D11RenderTargetView>	m_pRTV = nullptr;
	ComPtr<ID3D11DepthStencilView>	m_pDepthStencilView = nullptr;
	D3D11_VIEWPORT					m_vp;
public:
	// 1)����̽� ����
	HRESULT CreateDevice();
	// 2)���丮 ����
	HRESULT CreateDXGIDevice();
	// 3)����ü�� ����
	HRESULT CreateSwapChain();
	// 4)����Ÿ�Ϻ� ����
	HRESULT CreateRenderTargetView();
	// 5)����Ʈ ����
	HRESULT CreateDepthStencilView();
	// 6)����Ʈ ����
	void  CreateViewport();
	virtual HRESULT		ResizeDevice(UINT width, UINT height);
public:
	virtual bool		Init(); // �ʱ�ȭ
	virtual bool		Frame();// �ǽð� ���
	virtual bool		Render();// �ǽð� ������
	virtual bool		Release();// �Ҹ� �� ����
	virtual HRESULT CreateDXResource();
	virtual HRESULT DeleteDXResource();
};

