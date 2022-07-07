#pragma once
#include "TCore.h"
#include <d3d11.h>
#pragma comment	(lib, "d3d11.lib")
class Sample : public TCore
{
public:
	ID3D11Device*			m_pd3dDevice;	// ����̽� ��ü
	ID3D11DeviceContext*	m_pImmediateContext;// �ٺ��̽� ���ؽ�Ʈ ��ü
	IDXGISwapChain*			m_pSwapChain;	// ����ü�� ��ü
	ID3D11RenderTargetView* m_pRenderTargetView;// ���� ����Ÿ�� ��
	
	D3D_FEATURE_LEVEL       m_FeatureLevel;	// Direct3D Ư������ �Ӽ���	
	DXGI_SWAP_CHAIN_DESC	m_SwapChainDesc;	// ����ü�� �Ӽ���
	D3D11_VIEWPORT			m_ViewPort;	// �� ��Ʈ �Ӽ���
public:
	virtual bool	Init()  override;
	virtual bool	Frame()  override;
	virtual bool	Render()  override;
	virtual bool	Release()  override;
public:
	Sample();
};

