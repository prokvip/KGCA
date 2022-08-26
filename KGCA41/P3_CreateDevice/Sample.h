#pragma once
#include "TWindow.h"
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

class Sample : public TWindow
{
	// �������̽�-> �ϵ���� ���� ���� -> ȹ��
	ID3D11Device*			m_pd3dDevice = nullptr;// ����̽� ��ü
	ID3D11DeviceContext*	m_pImmediateContext = nullptr;
	IDXGIFactory*			m_pGIFactory = nullptr;
	IDXGISwapChain*			m_pSwapChain = nullptr;

	ID3D11RenderTargetView* m_pRTV = nullptr;
public:
	bool		Init(); // �ʱ�ȭ
	bool		Frame();// �ǽð� ���
	bool		Render();// �ǽð� ������
	bool		Release();// �Ҹ� �� ����
};

