#pragma once
#include "TWindow.h"
class TDevice : public TWindow
{
public:
	// �������̽�-> �ϵ���� ���� ���� -> ȹ��
	ID3D11Device*			m_pd3dDevice = nullptr;// ����̽� ��ü
	ID3D11DeviceContext*	m_pImmediateContext = nullptr;
	IDXGIFactory*			m_pGIFactory = nullptr;
	IDXGISwapChain*			m_pSwapChain = nullptr;
	ID3D11RenderTargetView* m_pRTV = nullptr;
	D3D11_VIEWPORT			m_vp;
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
	void  CreateViewport();
public:
	virtual bool		Init(); // �ʱ�ȭ
	virtual bool		Frame();// �ǽð� ���
	virtual bool		Render();// �ǽð� ������
	virtual bool		Release();// �Ҹ� �� ����
};

